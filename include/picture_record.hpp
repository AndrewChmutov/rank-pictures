#pragma once

#include <string>

struct PictureRecord {
    std::string name;
    std::size_t wins;
    std::size_t total;
    std::string path;
};