#include "source.hpp"
#include "assert.hpp"
#include "roots.hpp"

#include <fstream>

namespace file = std::filesystem;

error_code add_source(const char *file_path) {
    ASSERT(file::exists(file_path), "%s doesn't exist ", file_path);

    std::string path(file_path);
    bool        added = add_source_to_context(path);
    if (false == added) {
        ASSERT(false, "%s include to compiler failure",file_path);
        return FILE_NOT_ADDED;
    }

    return OK;
}
