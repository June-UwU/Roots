#include "logger.hpp"
#include "types.hpp"
#include <cstring>
#include <stdarg.h>
#include <stdio.h>

constexpr const u32 BUFFERSIZE = 32000;
static char OUTBUFFER[BUFFERSIZE];
static const char* LOGHEADERS[(u64)Loglevel::SIZE] 
{
    "INFO   : ",
    "WARN   : ",
    "ERROR  : "
};

void log(const LogLevel lvl,const char* msg,...)
{
    u32 writtenSize = strlen(LOGHEADERS[lvl]);
    memcpy(OUTBUFFER,LOGHEADERS[lvl],writtenSize);
    va_list args;
    va_start(args,msg);
    writtenSize += vsnprintf((OUTBUFFER + writtenSize),BUFFERSIZE,msg,args);
    va_end(args);
    memcpy((OUTBUFFER + writtenSize),"\n\0",2);
    printf("%s",OUTBUFFER);
}
