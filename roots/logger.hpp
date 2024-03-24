#pragma once
#include "types.hpp"

#define ANSI_COLOR_RESET       "\x1b[0m"
#define ANSI_COLOR_BLACK       "\x1b[30m"
#define ANSI_COLOR_RED         "\x1b[31m"
#define ANSI_COLOR_GREEN       "\x1b[32m"
#define ANSI_COLOR_YELLOW      "\x1b[33m"
#define ANSI_COLOR_BLUE        "\x1b[34m"
#define ANSI_COLOR_MAGENTA     "\x1b[35m"
#define ANSI_COLOR_CYAN        "\x1b[36m"
#define ANSI_COLOR_WHITE       "\x1b[37m"

#define LOG_TRACE(format, ...) log_message(ANSI_COLOR_WHITE format, __VA_ARGS__)
#define LOG_INFO(format, ...)  log_message(ANSI_COLOR_GREEN format, __VA_ARGS__)
#define LOG_WARN(format, ...)  log_message(ANSI_COLOR_YELLOW format, __VA_ARGS__)
#define LOG_FATAL(format, ...) log_message(ANSI_COLOR_RED format, __VA_ARGS__)

void log_message(const char *format, ...);