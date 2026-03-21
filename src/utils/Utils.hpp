#pragma once

#include <gp_Pnt.hxx>
#include <TopoDS_Shape.hxx>
#include <string>

namespace Utils { 
    /**
     * Print a gp_Pnt to iostream
     * @param pnt Point to print
     * @param name Name of the point
     * @param newLine If true, a new line will be printed after the point
     */
    void printPnt(const gp_Pnt& pnt, const std::string& name="", bool newLine=true);

    /**
     * Print a segment to iostream
     * @param name Name of the segment
     * @param seg Character to print
     * @param num Number of segments to print
     */
    void printSeg(const std::string& name="", char seg='=', int num=50);

    /**
     * Count the number of edges in a shape
     * @param shape Shape to count the edges
     */
    int countEdges(const TopoDS_Shape& shape);
}