#pragma once

typedef enum LogLevel
{
    INFO = 0x0,
    WARN,
    ERROR,
    LOGLEVEL
}LogLevel;


void log(const LogLevel lvl,const char* msg, ...);

#define LOG_INFO(message,...) log(INFO,message,##__VA_ARGS__)
#define LOG_WARN(message,...) log(WARN,message,##__VA_ARGS__)
#define LOG_ERROR(message,...) log(ERROR,message,##__VA_ARGS__)

