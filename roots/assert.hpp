#pragma once
#include "logger.hpp"
#include <iostream>

#define ASSERT(predicate, format, ...)                                         \
    if (!(predicate)) {                                                        \
        LOG_FATAL(format, __VA_ARGS__);                                        \
        exit(EXIT_FAILURE);                                                    \
    }