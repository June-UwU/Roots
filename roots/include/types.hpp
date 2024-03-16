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

typedef enum error_code
{
    OK = 0x0,
    FAIL,
    FILE_OPEN_FAIL,
    ERRORCODE
}error_code;

