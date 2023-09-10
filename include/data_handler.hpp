#pragma once

// Custom libraries
#include "picture_record.hpp"

// C++ standard libraries
#include <string>
#include <fstream>
#include <vector>

// Library for JSON handling
#include "json.hpp"

class DataHandler {
    std::string path;

    int getItemInt(nlohmann::json& data, std::string&& itemName) const;
    void setItem(nlohmann::json& data, std::string&& itemName, auto value) const;

public:
    DataHandler(std::string path);

    void getData(std::vector<PictureRecord>& pictures) const;

    void updateData(const std::vector<PictureRecord>& pictures);
};