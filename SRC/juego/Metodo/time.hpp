#pragma once
#include <ctime>
#include <string>

inline std::string getTimestamp() {
    std::time_t ahora = std::time(nullptr);
    std::tm* tm_info = std::localtime(&ahora);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    return std::string(buffer);
}