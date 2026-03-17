#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Shape.hxx>
#include <iostream>
#include <string>
#include "Viewer.hpp"
#include "IO.hpp"

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

    TopoDS_Shape shape;

    if (!inputPath.empty()) {
        std::cout << "Loading file: " << inputPath << std::endl;
        shape = IO::ReadShape(inputPath);
        if (shape.IsNull()) {
            std::cerr << "Error: Failed to load shape from " << inputPath << std::endl;
            return 1;
        }
    } else {
        // 如果没有输入路径，默认创建一个长方体
        std::cout << "No input file specified (-i [path]). Creating a default box." << std::endl;
        BRepPrimAPI_MakeBox mkbox(10.0, 20.0, 30.0);
        shape = mkbox.Shape();
    }

    // 使用 Viewer 进行可视化
    Viewer viewer;
    viewer.Show(shape);
    viewer.Start();

    return 0;  
}
