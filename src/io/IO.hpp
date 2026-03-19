#pragma once

#include <TopoDS_Shape.hxx>
#include <string>

#include "log/MSG.hpp"

/**
 * @brief 输入输出相关工具函数
 */
namespace IO {

/**
 * @brief 通用形状读取函数（支持 .brep, .stp, .step）
 */
MSG ReadShape(const std::string& filePath, TopoDS_Shape& shape);

/**
 * @brief 从 BREP 文件读取形状
 */
MSG ReadBrep(const std::string& filePath, TopoDS_Shape& shape);

/**
 * @brief 从 STEP 文件读取形状
 */
MSG ReadStep(const std::string& filePath, TopoDS_Shape& shape);

/**
 * @brief 将形状保存为 BREP 文件
 */
MSG SaveBrep(const TopoDS_Shape& shape, const std::string& filePath);

/**
 * @brief 将形状保存为 STEP 文件
 */
MSG SaveStep(const TopoDS_Shape& shape, const std::string& filePath);

} // namespace IO
