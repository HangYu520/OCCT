#include "Utils.hpp"

#include <iostream>
#include <TopExp_Explorer.hxx>

namespace Utils { 
    void printPnt(const gp_Pnt& pnt, const std::string& name, bool newLine) {
        std::cout << name << " ("
                << pnt.X() << ", " 
                << pnt.Y() << ", " 
                << pnt.Z() << ")";
        if(newLine) {
            std::cout << std::endl;
        }
    }

    void printSeg(const std::string& name, char seg, int num) {
        std::cout << std::string(num/2, seg) 
                << " " << name << " " 
                << std::string(num/2, seg) 
                << std::endl;
    }

    int countEdges(const TopoDS_Shape& shape) {
        int count = 0;
        TopExp_Explorer explorer(shape, TopAbs_EDGE);
        for(; explorer.More(); explorer.Next())
            count++;
        return count;
    };
}