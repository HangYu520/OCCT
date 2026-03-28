// --- OCCT ---
#include "OCCT_Headers.hpp"
// --- C++ ---
#include <iostream>
#include <string>
// --- custom ---
#include "utils/Utils.hpp"
#include "viewer/Viewer.hpp"

//#define COUT

namespace {
    void BottleProfile(TopoDS_Wire& myWireProfile, double myWidth, double myThickness)
    { 
         /*
        * === 1. 创建 Bottle 的轮廓线 ===
        */
        // * 1.1 XOY 平面上的特征点，用于描述轮廓线
        /*
                    ^ Y
                    |
        pnt1 ----------------- pnt5
        |          |          |
        pnt2 ----------------- pnt4 ——> X
        |          |          |
        |   ------pnt3------- |
        */
        gp_Pnt aPnt1(-myWidth / 2., 0, 0);
        gp_Pnt aPnt2(-myWidth / 2., -myThickness / 4., 0);
        gp_Pnt aPnt3(0, -myThickness / 2., 0);
        gp_Pnt aPnt4(myWidth / 2., -myThickness / 4., 0);
        gp_Pnt aPnt5(myWidth / 2., 0, 0);

        #if defined(COUT) // 输出特征点
        Utils::printSeg("Bottle Profile");
        
        Utils::printPnt(aPnt1, "aPnt1");
        Utils::printPnt(aPnt2, "aPnt2");
        Utils::printPnt(aPnt3, "aPnt3");
        Utils::printPnt(aPnt4, "aPnt4");
        Utils::printPnt(aPnt5, "aPnt5");
        #endif

        // * 1.2 构建直线 (Segment) 和 圆弧 (Arc of Circle)
        Handle(Geom_TrimmedCurve) aArcOfCircle = GC_MakeArcOfCircle(aPnt2,aPnt3,aPnt4); // 圆弧
        Handle(Geom_TrimmedCurve) aSegment1    = GC_MakeSegment(aPnt1, aPnt2); // 直线
        Handle(Geom_TrimmedCurve) aSegment2    = GC_MakeSegment(aPnt4, aPnt5);  // 直线

        #if defined(COUT) // 输出直线和圆弧
        std::cout << "\n--- Curves Info ---" << std::endl;
        std::cout << "[Arc of Circle] Param range: [" << aArcOfCircle->FirstParameter() << ", " << aArcOfCircle->LastParameter() << "]" << std::endl;
        Utils::printPnt(aArcOfCircle->Value(aArcOfCircle->FirstParameter()), " -> Start");
        Utils::printPnt(aArcOfCircle->Value(aArcOfCircle->LastParameter()),  " -> End");

        std::cout << "[Segment 1]     Param range: [" << aSegment1->FirstParameter() << ", " << aSegment1->LastParameter() << "]" << std::endl;
        Utils::printPnt(aSegment1->Value(aSegment1->FirstParameter()), " -> Start");
        Utils::printPnt(aSegment1->Value(aSegment1->LastParameter()),  " -> End");

        std::cout << "[Segment 2]     Param range: [" << aSegment2->FirstParameter() << ", " << aSegment2->LastParameter() << "]" << std::endl;
        Utils::printPnt(aSegment2->Value(aSegment2->FirstParameter()), " -> Start");
        Utils::printPnt(aSegment2->Value(aSegment2->LastParameter()),  " -> End");
        #endif

        // * 1.3 根据 “几何 (Geom)“ 定义 ”线框 (Wire)“

        // curve -> edge
        TopoDS_Edge aEdge1 = BRepBuilderAPI_MakeEdge(aSegment1);
        TopoDS_Edge aEdge2 = BRepBuilderAPI_MakeEdge(aArcOfCircle);
        TopoDS_Edge aEdge3 = BRepBuilderAPI_MakeEdge(aSegment2);

        // edge -> wire
        TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(aEdge1, aEdge2, aEdge3); // 必须按照链接顺序

        // 镜像 (Mirror) 另一半 Wire
        gp_Trsf aTrsf;
        gp_Ax1 xAxis = gp::OX(); // 关于 X 轴镜像
        aTrsf.SetMirror(xAxis);
        
        BRepBuilderAPI_Transform aMirror(aWire, aTrsf);
        TopoDS_Wire aMirrorWire = TopoDS::Wire(aMirror.Shape());

        // 创建完整的 Wire
        BRepBuilderAPI_MakeWire mkWire;
        mkWire.Add(aWire);
        mkWire.Add(aMirrorWire);
        myWireProfile = mkWire.Wire();

        #if defined(COUT)
        std::cout << "\n--- Wires Info ---" << std::endl;
        std::cout << "[aWire]          Edges: " << Utils::countEdges(aWire) << ", Closed: " << (aWire.Closed() ? "Yes" : "No") << std::endl;
        std::cout << "[myWireProfile]  Edges: " << Utils::countEdges(myWireProfile) << ", Closed: " << (myWireProfile.Closed() ? "Yes" : "No") << std::endl;
        #endif

    }

    void BottleBody(TopoDS_Shape& myBody, const TopoDS_Wire& myWireProfile, double myHeight, double myThickness) {
        /*
        * === 2. 创建 Bottle 的实体 (Body) ===
        */
        // * 2.1 拉伸 (Extrude) 平面轮廓线构建 Body
        // wire -> Face
        TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(myWireProfile);
        myBody = BRepPrimAPI_MakePrism(myFaceProfile, gp_Vec(0, 0, myHeight));

        // * 2.2 边缘过于锐利，进行圆角处理 (Fillet)
        BRepFilletAPI_MakeFillet mkFillet(myBody);
        TopExp_Explorer anEdgeExplorer(myBody, TopAbs_EDGE);
        for (; anEdgeExplorer.More(); anEdgeExplorer.Next()) {
            TopoDS_Edge anEdge = TopoDS::Edge(anEdgeExplorer.Current());
            mkFillet.Add(myThickness / 12., anEdge);
        }
        myBody = mkFillet.Shape();

        // * 2.3 创建颈部 (Neck)
        gp_Pnt neckLoaction = gp_Pnt(0, 0, myHeight);
        gp_Dir neckAxis = gp::DZ();
        gp_Ax2 neckAx2(neckLoaction, neckAxis); // 局部坐标系

        // 创建圆柱体
        TopoDS_Shape myNeck = BRepPrimAPI_MakeCylinder(
            neckAx2, myThickness / 4., myHeight / 10.
        ).Shape();

        // 融合 neck 和 body
        myBody = BRepAlgoAPI_Fuse(myBody, myNeck);

        // * 2.4 挖空实体 (Hollowed)
        TopoDS_Face faceToRemove;
        double zMax = -DBL_MAX; // 找到最顶面

        TopExp_Explorer aFaceExplorer(myBody, TopAbs_FACE);
        for(; aFaceExplorer.More(); aFaceExplorer.Next()) {
            TopoDS_Face aFace = TopoDS::Face(aFaceExplorer.Current());
            Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
            if(aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)) {
                Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
                if(aPlane->Location().Z() > zMax) {
                    zMax = aPlane->Location().Z();
                    faceToRemove = aFace;
                }
            }
        }

        NCollection_List<TopoDS_Shape>  shapesToRemove;
        shapesToRemove.Append(faceToRemove);
        BRepOffsetAPI_MakeThickSolid aSolidMaker;
        aSolidMaker.MakeThickSolidByJoin(myBody, shapesToRemove, -myThickness / 50., 1.e-3);
        myBody = aSolidMaker.Shape();
    }
}

int main()
{
    /*
    官方的做瓶子入门教程 （Make a Bottle）
     - https://dev.opencascade.org/doc/overview/html/occt__tutorial.html
    */

    // * 瓶子的基本参数
    double myHeight = 70.0; // 70 mm 
    double myWidth = 50.0; // 50 mm
    double myThickness = 30.0; // 30 mm

    TopoDS_Wire myWireProfile; // Step 1
    BottleProfile(myWireProfile, myWidth, myThickness);

    TopoDS_Shape myBody; // Step 2
    BottleBody(myBody, myWireProfile, myHeight, myThickness);

    // * 可视化最终结果
    Viewer viewer;
    viewer.Show(myBody);
    viewer.Start();

    return 0;
}