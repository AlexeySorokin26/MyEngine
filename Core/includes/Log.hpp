#pragma once

#include <spdlog/spdlog.h>

#ifdef NDEBUG

#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_CRIT(...) 

#else
// LOG_INFO wraps spdlog::info, allowing formatted logging with any number of arguments
#define LOG_INFO(...)	spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)  spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)
#define LOG_CRIT(...)  spdlog::critical(__VA_ARGS__)

#endif