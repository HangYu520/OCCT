#pragma once

#include <string>

/**
 * @brief 日志级别
 */
enum LogLevel {
    Info,
    Warning,
    Error
};

/**
 * @brief 返回信息
*/
struct MSG {
    bool success;
    std::string msg;
    LogLevel level;
};