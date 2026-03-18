#include "IO.hpp"
#include <BRepTools.hxx>
#include <BRep_Builder.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <Interface_Static.hxx>
#include <iostream>
#include <fstream>
#include <algorithm>

namespace IO {

// 辅助函数：获取文件后缀名
static std::string getExtension(const std::string& filePath) {
    size_t lastDot = filePath.find_last_of('.');
    if (lastDot == std::string::npos) return "";
    std::string ext = filePath.substr(lastDot + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

TopoDS_Shape ReadShape(const std::string& filePath) {
    std::string ext = getExtension(filePath);
    if (ext == "brep") {
        return ReadBrep(filePath);
    } else if (ext == "stp" || ext == "step") {
        return ReadStep(filePath);
    } else {
        std::cerr << "Unsupported file extension: ." << ext << std::endl;
        return TopoDS_Shape();
    }
}

TopoDS_Shape ReadBrep(const std::string& filePath) {
    std::ifstream f(filePath.c_str());
    if (!f.good()) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return TopoDS_Shape();
    }

    TopoDS_Shape shape;
    BRep_Builder builder;
    if (!BRepTools::Read(shape, filePath.c_str(), builder)) {
        std::cerr << "BRepTools failed to read BREP file: " << filePath << std::endl;
        return TopoDS_Shape();
    }
    return shape;
}

TopoDS_Shape ReadStep(const std::string& filePath) {
    STEPControl_Reader reader;
    IFSelect_ReturnStatus status = reader.ReadFile(filePath.c_str());
    if (status != IFSelect_RetDone) {
        std::cerr << "Failed to read STEP file: " << filePath << " (Status: " << status << ")" << std::endl;
        return TopoDS_Shape();
    }

    // 传输数据并获取形状
    reader.TransferRoots();
    return reader.OneShape();
}

bool SaveBrep(const TopoDS_Shape& shape, const std::string& filePath) {
    if (shape.IsNull()) return false;
    return BRepTools::Write(shape, filePath.c_str());
}

bool SaveStep(const TopoDS_Shape& shape, const std::string& filePath) {
    if (shape.IsNull()) return false;
    STEPControl_Writer writer;
    IFSelect_ReturnStatus status = writer.Transfer(shape, STEPControl_AsIs);
    if (status != IFSelect_RetDone) return false;
    
    return (writer.Write(filePath.c_str()) == IFSelect_RetDone);
}

} // namespace IO
