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

MSG ReadShape(const std::string& filePath, TopoDS_Shape& shape) {
    MSG msg;
    std::string ext = getExtension(filePath);
    if (ext == "brep") {
        msg = ReadBrep(filePath, shape);
        return msg;
    } else if (ext == "stp" || ext == "step") {
        msg = ReadStep(filePath, shape);
        return msg;
    } else {
        return {false, "Unsupported file extension: ." + ext, LogLevel::Error};
    }
}

MSG ReadBrep(const std::string& filePath, TopoDS_Shape& shape) {
    std::ifstream f(filePath.c_str());
    if (!f.good()) {
        return {false, "File does not exist: " + filePath, LogLevel::Error};
    }

    BRep_Builder builder;
    if (!BRepTools::Read(shape, filePath.c_str(), builder)) {
        return {false, "BRepTools failed to read BREP file: " + filePath, LogLevel::Error};
    }
    return {true, "BREP file read successfully: " + filePath, LogLevel::Info};
}

MSG ReadStep(const std::string& filePath, TopoDS_Shape& shape) {
    STEPControl_Reader reader;
    IFSelect_ReturnStatus status = reader.ReadFile(filePath.c_str());
    if (status != IFSelect_RetDone) {
        return {false, "Failed to read STEP file: " + filePath, LogLevel::Error};
    }

    reader.TransferRoots();
    shape = reader.OneShape();
    return {true, "STEP file read successfully: " + filePath, LogLevel::Info};
}

MSG SaveBrep(const TopoDS_Shape& shape, const std::string& filePath) {
    if (shape.IsNull()) 
        return {false, "Shape is null", LogLevel::Error};
    BRepTools::Write(shape, filePath.c_str());
    return {true, "BREP file saved successfully: " + filePath, LogLevel::Info};
}

MSG SaveStep(const TopoDS_Shape& shape, const std::string& filePath) {
    if (shape.IsNull()) 
        return {false, "Shape is null", LogLevel::Error};
    STEPControl_Writer writer;
    IFSelect_ReturnStatus status = writer.Transfer(shape, STEPControl_AsIs);
    if (status != IFSelect_RetDone) 
        return {false, "Failed to write STEP file: " + filePath, LogLevel::Error};
    writer.Write(filePath.c_str());
    return {true, "STEP file saved successfully: " + filePath, LogLevel::Info};
}

} // namespace IO
