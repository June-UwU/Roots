#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string>

// SIGNED TYPES
typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t     s64;

// UNSIGNED TYPES
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

// FLOATING TYPES
typedef float       f32;
typedef double      f64;

// POINTER SIZE
constexpr const u64 PTR_SIZE = sizeof(void*);

#define ERROR_CODES X(OK)   \
    X(FILE_NOT_ADDED)       \

#define X(code) code,

typedef enum error_code {
    ERROR_PREFIX = -1,
    ERROR_CODES
    ERROR_SIZE
} error_code;

#undef X

extern const char* error_string[];