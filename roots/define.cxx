#include "types.hpp"

#define X(code) #code,

const char *error_string[]{ERROR_CODES};

#undef X
