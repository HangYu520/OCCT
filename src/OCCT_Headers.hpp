#pragma once

/**
 * @file OCCT_Headers.hpp
 * @brief Precompiled Header for the project, including frequently used OpenCASCADE (OCCT) 
 * and potentially VTK headers to improve compilation speed and convenience.
 */

// ==========================================
//              OCCT 核心基础
// ==========================================
#include <Standard_Handle.hxx>
#include <Standard_Version.hxx>
#include <TCollection_AsciiString.hxx>
#include <Message_ProgressRange.hxx> // [新增] OCCT 7.5+ 许多算法需要进度条参数

// --- OCCT Math / Geometry Types (gp) ---
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>        // [新增] 局部坐标系(非常常用)
#include <gp_Vec.hxx>
#include <gp_Trsf.hxx>
#include <gp_Pln.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Hypr.hxx>
#include <gp_Parab.hxx>
#include <gp_Lin.hxx>
#include <gp_Pnt2d.hxx>      // [新增] 2D 坐标点 (处理 PCurve 必备)
#include <gp_Dir2d.hxx>      // [新增] 2D 方向
#include <gp_Vec2d.hxx>      // [新增] 2D 向量

// ==========================================
//              OCCT 拓扑与几何
// ==========================================
// --- OCCT Topology Types ---
#include <TopAbs_ShapeEnum.hxx>
#include <TopAbs_Orientation.hxx> // [新增] FORWARD/REVERSED 判断必备
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>      // [新增] 补全拓扑层级：点
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_CompSolid.hxx>   // [新增] 补全拓扑层级：复合实体
#include <TopoDS_Compound.hxx>

// --- OCCT Geometry Curves and Surfaces (3D) ---
#include <Geom_Geometry.hxx>      // [新增] 所有几何实体的基类
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>          // [新增] DownCast 为直线必备
#include <Geom_Circle.hxx>        // [新增] DownCast 为圆必备
#include <Geom_TrimmedCurve.hxx>
#include <Geom_Surface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_BSplineCurve.hxx>  // [新增] NURBS 曲线 (工业核心)
#include <Geom_BSplineSurface.hxx>// [新增] NURBS 曲面 (工业核心)

// --- OCCT Geometry Curves (2D / PCurve) ---
#include <Geom2d_Curve.hxx>       // [新增] 2D 参数曲线基类
#include <Geom2d_Line.hxx>        // [新增] 2D 直线
#include <Geom2d_TrimmedCurve.hxx>// [新增] 2D 裁剪曲线

// ==========================================
//              OCCT 算法与构建
// ==========================================
// --- OCCT BRep Building / API ---
#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <BRep_Builder.hxx>       // [新增] 手动将 Face 塞进 Shell，或 Edge 塞进 Wire 必备
#include <BRepBuilderAPI_MakeVertex.hxx> // [新增] 用 gp_Pnt 造 TopoDS_Vertex
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepAlgoAPI_BooleanOperation.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepLib.hxx>            // [新增] BRepLib::BuildPCurveForEdgeOnFace 补救 PCurve 必备
#include <BRepFilletAPI_MakeFillet.hxx> // [新增] 边缘圆角

// --- OCCT Construction Helpers ---
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>
#include <GC_MakeCircle.hxx>

// ==========================================
//              OCCT 遍历与分析 (重点补全)
// ==========================================
// --- OCCT Exploration ---
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Iterator.hxx>            // [新增] 严格父子层级遍历
#include <BRepTools_WireExplorer.hxx>     // [新增] 有序遍历 Wire 里的 Edge 必备！
#include <TopTools_IndexedMapOfShape.hxx> // [新增] TopExp::MapShapes 拓扑去重必备！
#include <NCollection_IndexedMap.hxx>
#include <TopTools_ShapeMapHasher.hxx>

// --- OCCT Properties (包围盒、体积、面积、重心) ---
#include <Bnd_Box.hxx>            // [新增] 3D 轴向包围盒 (AABB)
#include <BRepBndLib.hxx>         // [新增] 计算 Shape 的包围盒
#include <GProp_GProps.hxx>       // [新增] 质量/几何属性容器
#include <BRepGProp.hxx>          // [新增] 计算 Shape 的体积、面积、重心

// --- OCCT Meshing (离散化：渲染/转出必备) ---
#include <BRepMesh_IncrementalMesh.hxx> // [新增] 将 BRep 曲面网格化为三角形
#include <Poly_Triangulation.hxx>       // [新增] 存储三角形网格数据的容器
#include <TopLoc_Location.hxx>          // [新增] 读取网格坐标时的局部矩阵