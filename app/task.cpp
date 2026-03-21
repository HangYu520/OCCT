// --- OCCT ---
#include "OCCT_Headers.hpp"
// --- C++ ---
#include <iostream>
#include <string>
// --- custom ---
#include "utils/Utils.hpp"

namespace { 

    /*
    🚀 任务一："点"的审查 —— 顶点去重：
    
    长方体有 6 个面，每个面 4 个顶点，如果直接暴力遍历，你会得到 24 个顶点。但实际上长方体只有 8 个物理顶点。
    
    任务：从 myBox 中提取出所有不重复的顶点（Vertex）。 将这 8 个顶点的绝对 3D 坐标 (X, Y, Z) 打印到控制台。
    
    进阶要求：计算这 8 个顶点的包围盒中心点（把 8 个点的 X、Y、Z 分别求和除以 8），
    
    💡 提示：
    
    - 需要用到 NCollection_IndexedMap 和 TopExp::MapShapes。
    
    - 需要用到 BRep_Tool::Pnt()。
    */
    void task01(TopoDS_Shape& box) {
        Utils::printSeg("Task 01");
        
        NCollection_IndexedMap<TopoDS_Shape, TopTools_ShapeMapHasher> vertexMap;
        TopExp::MapShapes(box, TopAbs_VERTEX, vertexMap);

        double sumX = 0, sumY = 0, sumZ = 0;
        int count = vertexMap.Extent();

        for (int i = 1; i <= count; i++) {
            const TopoDS_Shape& vertex = vertexMap(i);
            gp_Pnt pnt = BRep_Tool::Pnt(TopoDS::Vertex(vertex));
            Utils::printPnt(pnt, "Vertex" + std::to_string(i));

            sumX += pnt.X();
            sumY += pnt.Y();
            sumZ += pnt.Z();
        }

        if (count > 0) {
            Utils::printPnt(
                gp_Pnt(sumX / count, sumY / count, sumZ / count),
                "Center of " + std::to_string(count) + " vertices :"
            );
        }
    }

    /*
    🚀 任务二："面"的侦察 —— 寻找真正的“顶面”：
    
    底层的几何平面（Geom_Plane）的法线是死板的，只有结合了拓扑层的 Orientation 才能知道真实的物理朝向。
    
    任务：

    - 遍历 Box 中的所有 6 个面（Face）

    - 提取每个面的底层几何，并将其安全转型（DownCast）为 Geom_Plane

    - 提取平面的底层法线，并根据该 Face 的拓扑正负标签（FORWARD/REVERSED）进行法线修正

    - 找出那个真实法线严格朝上（即修正后的法向量为 (0, 0, 1)）的面

    - 打印出这个“顶面”的几何原点（Location）
    
    💡 提示：
    
    - 需要用到 TopExp_Explorer， BRep_Tool::Surface() 和 Handle(Geom_Plane)::DownCast()
    
    - 如果 face.Orientation() == TopAbs_REVERSED，记得调用 .Reverse() 翻转法线向量
    */
   void task02(TopoDS_Shape& box) {
        Utils::printSeg("Task 02");

        TopExp_Explorer exp(box, TopAbs_FACE);
        for(; exp.More(); exp.Next()) {
            TopoDS_Face face = TopoDS::Face(exp.Current());
            Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
            Handle(Geom_Plane) plane = Handle(Geom_Plane)::DownCast(surface);
            if(!plane.IsNull()) {
                gp_Dir dir = plane->Pln().Axis().Direction();
                if(face.Orientation() == TopAbs_REVERSED) {
                    dir.Reverse();
                }
                if(dir.X() == 0 && dir.Y() == 0 && dir.Z() == 1) {
                    Utils::printPnt(plane->Location(), "Top face location: ");
                    break;
                }
            }
        }
   }

    /*
    🚀 任务三："线"的巡逻 —— 验证“一笔画”闭合定律：
    
    已经知道 Wire 在底层 Dump 里是无序存放的。现在我们要用代码证明它是可以被“理顺”的。
    
    任务：

    - 从 Box 中随便拿出一个线框（Wire）（比如通过 TopExp_Explorer 遍历找第一个 Wire）

    - 使用专用的线框迭代器去遍历这个 Wire 里面的 Edge

    - 提取每一条 Edge 的底层曲线（Geom_Curve）和参数范围（first, last）

    - 最核心的一步：根据 Edge 的 Orientation，正确判断出这条边在当前循环中的真实起点和真实终点的 3D 坐标

    - 打印出每条边的首尾坐标，格式如：Edge 1: (0,0,30) -> (10,0,30)
    
    💡 提示：
    
    - 必须使用 BRepTools_WireExplorer 才能保证顺序
    */
   void task03(TopoDS_Shape& box) { 
        Utils::printSeg("Task 03");

        TopExp_Explorer exp(box, TopAbs_WIRE);
        TopoDS_Wire wire = TopoDS::Wire(exp.Current());

        BRepTools_WireExplorer wexp(wire);
        int edgeCount = 0;
        for(; wexp.More(); wexp.Next()) {
            double first, last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(wexp.Current(), first, last);
            if(wexp.Orientation() == TopAbs_REVERSED) {
                std::swap(first, last);
            }
            std::cout << "Edge " << ++edgeCount << ": ";
            Utils::printPnt(curve->Value(first), "", false);
            std::cout << " -> ";
            Utils::printPnt(curve->Value(last));
        }
   }

} // namespace

int main()
{
    // 创建一个盒子 [0, 10] * [0, 20] * [0, 30]
    BRepPrimAPI_MakeBox mkbox(10.0, 20.0, 30.0);
    TopoDS_Shape box = mkbox.Shape();

    // 运行任务
    task01(box);
    task02(box);
    task03(box);

    return 0;
}