#pragma once

#include <TopoDS_Shape.hxx>
#include <string>

/**
 * @brief 输入输出相关工具函数
 */
namespace IO {

/**
 * @brief 通用形状读取函数（支持 .brep, .stp, .step）
 */
TopoDS_Shape ReadShape(const std::string& filePath);

/**
 * @brief 从 BREP 文件读取形状
 */
TopoDS_Shape ReadBrep(const std::string& filePath);

/**
 * @brief 从 STEP 文件读取形状
 */
TopoDS_Shape ReadStep(const std::string& filePath);

/**
 * @brief 将形状保存为 BREP 文件
 */
bool SaveBrep(const TopoDS_Shape& shape, const std::string& filePath);

/**
 * @brief 将形状保存为 STEP 文件
 */
bool SaveStep(const TopoDS_Shape& shape, const std::string& filePath);

} // namespace IO
