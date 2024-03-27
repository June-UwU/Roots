#include "assert.hpp"
#include "types.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

std::unordered_map<std::string, std::string> source_map;

bool add_source_to_context(std::string &path) {
    std::stringstream source;
    std::ifstream     file(path,std::ios::in);
    ASSERT(false != file.is_open(),"error in opening file : ", path.c_str());

    source << file.rdbuf();

    source_map[path] = source.str();
    return true;
}

std::string &get_source(std::string &path) {
    ASSERT(source_map.end() != source_map.find(path), "%s file was not found",
           path.c_str());

    return source_map[path];
}