
---

# OCCT 拓扑与几何读取标准工作流

在 OCCT 中，从复杂的 3D 模型中提取具体的数学参数（如坐标、半径、法向量）必须严格遵循**“三步走 + 一修正”**的标准流程：

## 第一步：遍历拓扑 (Topological Traversal)

根据不同的业务需求，选择合适的遍历工具拆解 `TopoDS_Shape`。

### 1. 全局暴力遍历 (`TopExp_Explorer`)

* **适用场景**：统计总面积、渲染所有面/边、不需要关心父子层级关系时。
* **特点**：会穿透层级，直接捞出所有指定类型的元素。但**不保证去重**（共享边会被遍历多次）。

```cpp
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>

for (TopExp_Explorer ex(shape, TopAbs_FACE); ex.More(); ex.Next()) {
    TopoDS_Face aFace = TopoDS::Face(ex.Current());
    // 处理 aFace...
}

```

### 2. 有序线框遍历 (`BRepTools_WireExplorer`)

* **适用场景**：刀具轨迹规划、轮廓网格化、计算闭合周长。
* **特点**：**极其重要！** 专门用于遍历 `Wire`，保证拿到的 `Edge` 是**严格首尾相接**的。

```cpp
#include <BRepTools_WireExplorer.hxx>
#include <TopoDS.hxx>

TopoDS_Wire aWire = ...; 
for (BRepTools_WireExplorer wireExp(aWire); wireExp.More(); wireExp.Next()) {
    TopoDS_Edge anEdge = wireExp.Current();
    // 此时的 anEdge 绝对满足一笔画的首尾相连顺序
}

```

### 3. 严格层级遍历 (`TopoDS_Iterator`)

* **适用场景**：重构拓扑树、分析零件装配结构。
* **特点**：只获取当前节点的**直接子节点**，绝不越级穿透。

```cpp
#include <TopoDS_Iterator.hxx>

for (TopoDS_Iterator it(shape); it.More(); it.Next()) {
    TopoDS_Shape child = it.Value(); 
    // 判断 child 是 Shell 还是 Solid 等
}

```

### 4. 自动去重遍历 (`TopExp::MapShapes`)

* **适用场景**：导出无重复的网格线框、统计模型的真实边数/顶点数。
* **特点**：利用哈希表（Map）将拓扑元素装入集合，完美解决多面共享边导致的重复统计问题。**注意：在 OCCT 8.0+ 中，原有的 `TopTools_IndexedMapOfShape` 已被弃用。**

```cpp
#include <NCollection_IndexedMap.hxx>
#include <TopTools_ShapeMapHasher.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>

// 现代写法 (OCCT 8.0+)
NCollection_IndexedMap<TopoDS_Shape, TopTools_ShapeMapHasher> edgeMap;
TopExp::MapShapes(shape, TopAbs_EDGE, edgeMap);

for (int i = 1; i <= edgeMap.Extent(); ++i) {
    TopoDS_Edge uniqueEdge = TopoDS::Edge(edgeMap(i));
    // 处理唯一边...
}

```

---

## 第二步：拓扑转几何桥梁 (`BRep_Tool`)

拓扑层本身没有坐标和形状。必须通过 `BRep_Tool` 提取底层的抽象几何体。

| 输入拓扑类型 | BRep_Tool 提取语法 | 返回的抽象几何基类 | 附加产物 |
| --- | --- | --- | --- |
| **Face** | `BRep_Tool::Surface(aFace)` | `Handle(Geom_Surface)` | 无 |
| **Edge** | `BRep_Tool::Curve(anEdge, first, last)` | `Handle(Geom_Curve)` | 截断参数 `first`, `last` |
| **Edge + Face** | `BRep_Tool::CurveOnSurface(anEdge, aFace, f, l)` | `Handle(Geom2d_Curve)` | 2D 参数 `f`, `l` (PCurve) |
| **Vertex** | `BRep_Tool::Pnt(aVertex)` | `gp_Pnt` (绝对坐标) | 无 |

---

## 第三步：向下转型与读取 (`DownCast`)

`BRep_Tool` 返回的 `Geom_Surface` 或 `Geom_Curve` 只是抽象基类。需要使用 `DownCast` 将其安全转化为具体的数学实体，才能调用独有参数（如半径、法向量）。

### 示例：解析面 (Face) 的几何属性

```cpp
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>

Handle(Geom_Surface) aSurf = BRep_Tool::Surface(aFace);

// 尝试转化为平面
Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurf);
if (!aPlane.IsNull()) {
    gp_Dir normal = aPlane->Pln().Axis().Direction(); // 拿到原始法向量
} 
else {
    // 若不是平面，尝试转化为圆柱面
    Handle(Geom_CylindricalSurface) aCyl = Handle(Geom_CylindricalSurface)::DownCast(aSurf);
    if (!aCyl.IsNull()) {
        Standard_Real radius = aCyl->Radius(); // 拿到圆柱半径
    }
}

```

---

## 第四步：⚠️ 核心避坑指南 (方向修正)

**底层几何 (Geom) 完全忽略了拓扑层中的翻转标签！** 如果你要计算真实的物理朝向，**必须手动结合 `Orientation()` 进行修正。**

### 1. 修正面法线 (寻找真实的向外法线)

如果 `Face` 在拓扑中被标记为 `REVERSED`，说明真实的物理法线与底层几何法线是相反的。

```cpp
gp_Dir realNormal = aPlane->Pln().Axis().Direction(); // 1. 取几何法线
if (aFace.Orientation() == TopAbs_REVERSED) {
    realNormal.Reverse(); // 2. 拓扑为 REVERSED 时，强制翻转！
}
// realNormal 此时绝对指向空气/实体外部

```

### 2. 修正边的起止点 (寻找真实的走向)

`Curve(first)` 永远是几何上的起点。如果 `Edge` 被标记为 `REVERSED`，说明它在这个 `Wire` 里是“倒着走”的。

```cpp
gp_Pnt geomStart = aCurve->Value(first);
gp_Pnt geomEnd   = aCurve->Value(last);

gp_Pnt topoStart, topoEnd;
if (anEdge.Orientation() == TopAbs_FORWARD) {
    topoStart = geomStart; 
    topoEnd   = geomEnd;
} else if (anEdge.Orientation() == TopAbs_REVERSED) {
    topoStart = geomEnd;   // 倒着走：几何的终点才是真实的起点
    topoEnd   = geomStart;
}

```

---