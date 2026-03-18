// --- OCCT ---
#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepTools.hxx>
// --- C++ ---
#include <iostream>
#include <string>
// --- custom ---
#include "viewer/Viewer.hpp"
#include "io/IO.hpp"

int main(int argc, char* argv[]) 
{
    std::string inputPath = "";
    
    // 简单的命令行解析
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-i" && i + 1 < argc) {
            inputPath = argv[++i];
        }
    }

    Viewer viewer;
    TopoDS_Shape shape;

    if (!inputPath.empty()) {
        viewer.Log("Loading file: " + inputPath);
        shape = IO::ReadShape(inputPath);
        if (shape.IsNull()) {
            viewer.Log("Error: Failed to load shape from " + inputPath);
            return 1;
        }
    } else {
        viewer.Log("No input file specified. Creating a default box.");
        BRepPrimAPI_MakeBox mkbox(10.0, 20.0, 30.0);
        shape = mkbox.Shape();
    }

    // 打印 shape
    BRepTools::Dump(shape, std::cout);

    // 使用 Viewer 进行可视化
    viewer.Show(shape);
    viewer.Start();

    return 0;  
}
