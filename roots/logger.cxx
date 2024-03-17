#include "logger.hpp"
#include <stdarg.h>

constexpr const u32 MESSAGE_BUFFER_SIZE = KiB(32);
static char message_buffer[MESSAGE_BUFFER_SIZE];

void log_message(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vsnprintf(message_buffer, MESSAGE_BUFFER_SIZE, format, args);
    printf(message_buffer);
    printf(ANSI_COLOR_RESET);
    
    va_end(args);
}
