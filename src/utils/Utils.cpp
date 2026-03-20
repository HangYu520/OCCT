#include "Utils.hpp"

#include <iostream>

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
}