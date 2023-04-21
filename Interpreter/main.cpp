#include "types.hpp"
#include "logger.hpp"

s32 main(s32 argc, char** argv) 
{
    LOG_INFO("informed");
    LOG_WARN("warned");
    LOG_ERROR("error");
    return 0;
}
