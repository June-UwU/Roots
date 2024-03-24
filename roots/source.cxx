#include "source.hpp"
#include "assert.hpp"
#include "roots.hpp"

#include <fstream>
#include <iostream>

namespace file = std::filesystem;

error_code add_source(const char *file_path) {
    ASSERT(file::exists(file_path), "%s doesn't exist ", file_path);

    FILE *file = std::fopen(file_path, "r");
    ASSERT(nullptr != file, "%s error opening", file_path)

    std::string path(file_path);
    bool        added = add_source_to_context(path, file);
    if (false == added) {
        // TODO : diagnostics
        return FILE_NOT_ADDED;
    }

    std::fclose(file);
    return OK;
}
