#include "assert.hpp"
#include "types.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

std::unordered_map<std::string, std::string> source_map;

bool add_source_to_context(std::string &path, FILE *stream) {
    ASSERT(nullptr != stream, "%s stream was nullptr");
    std::stringstream source;
       
    std::ifstream file(stream);

    source << file.rdbuf();

    source_map[path] = source.str();
    return true;
}

std::string& get_source(std::string &path) {
    ASSERT(source_map.end() != source_map.find(path), "%s file was not found", path.c_str());

    return source_map[path];
}