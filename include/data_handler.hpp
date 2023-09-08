#pragma once

// Custom libraries
#include "picture_record.hpp"

// C++ standard libraries
#include <string>
#include <fstream>
#include <vector>

class DataHandler {
    std::string name;

public:
    DataHandler(std::string name);

    void getData(std::vector<PictureRecord>& pictures) const;

    void updateData(const std::vector<PictureRecord>& pictures);
};