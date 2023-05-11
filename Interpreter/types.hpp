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

typedef enum ErrorCode
{
    OK = 0x0,
    FAIL,
    FILE_OPEN_FAIL,
    ERRORCODE
}ErrorCode;

typedef struct Error
{
    ErrorCode code;
    std::string msg;
    
    inline Error& operator=(const Error& rhs)
    {
        if(this == &rhs)
        {
            return *this;
        }

        this->code = rhs.code;
        this->msg = rhs.msg;
    }

    inline bool operator==(const ErrorCode& errorCode)
    {
        return (code == errorCode);
    }

    inline bool operator!=(const ErrorCode& errorCode)
    {
        return !(code == errorCode);
    }
}Error;

constexpr const Error NOERROR = {OK, ""};

inline void printError(const Error& errorRef)
{
    if(errorRef.code != OK)
    {
        printf("Interpreter Error Code %X : %s",errorRef.code,errorRef.msg);
    }
}
