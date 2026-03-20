// --- OCCT ---
#include <gp_Pnt.hxx>
// --- custom ---
#include "utils/Utils.hpp"

int main()
{
    /*
    官方的做瓶子入门教程 （Make a Bottle）
     - https://dev.opencascade.org/doc/overview/html/occt__tutorial.html
    */

    // 瓶子的基本参数
    double myHeight = 70.0; // 70 mm 
    double myWidth = 50.0; // 50 mm
    double myThickness = 30.0; // 30 mm

    /*
    * === 1. 创建 Bottle 的轮廓线 ===
    */
    // XOY 平面上的特征点，用于描述轮廓线
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

    // 输出特征点
    Utils::printSeg("Bottle Profile");
    
    Utils::printPnt(aPnt1, "aPnt1");
    Utils::printPnt(aPnt2, "aPnt2");
    Utils::printPnt(aPnt3, "aPnt3");
    Utils::printPnt(aPnt4, "aPnt4");
    Utils::printPnt(aPnt5, "aPnt5");

    return 0;
}