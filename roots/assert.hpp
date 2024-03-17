#pragma once
#include <iostream>

#define ASSERT(predicate, format, ...) \
	if (!(predicate))                                                                                            \
    {                                                                                                            \
        std::cout << format << "\n";                                                                                     \
    }