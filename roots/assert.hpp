#pragma once

#define ASSERT(predicate, format, ...) \
	if (!(predicate))                                                                                            \
    {                                                                                                            \
        std::cout << format;                                                                                     \
    }