#include "data_handler.hpp"

// C++ standard libraries
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>

// Library for JSON jandling
#include "json.hpp"

DataHandler::DataHandler(std::string path) : path(path + "/statistics.json") {}


int DataHandler::getItemInt(nlohmann::json& data, std::string&& itemName) const {
    // To convert values
    std::stringstream ss;
    int answer = 0;

    // If found record, try to find wins
    auto jsonRecordItem = data.find(itemName);

    if (jsonRecordItem != data.end()) {
        // If found wins, assign them
        ss.clear();
        ss << *jsonRecordItem;

        try {
            answer = std::stoi(ss.str());
        }
        catch (...) {
            answer = 0;
        }
    }


    return answer;
}


void DataHandler::setItem(nlohmann::json& data, std::string&& itemName, auto value) const {
    data[itemName] = value;
}


void DataHandler::getData(std::vector<PictureRecord> &pictures) const {
    std::ifstream infoFile;
    nlohmann::json data;

    // If something goes wrong, return
    // That means all wins and total counter = 0
    try {
        infoFile.open(path);
        data = nlohmann::json::parse(infoFile);
        infoFile.close();
    }
    catch (...) {
        return;
    }


    // Go through each record and fill the info
    for (auto& picture : pictures) {
        // try to find record in json
        auto jsonPictureRecord = data.find(picture.name);

        if (jsonPictureRecord != data.end()) {
            picture.wins = getItemInt(*jsonPictureRecord, "wins");
            picture.total = getItemInt(*jsonPictureRecord, "total");
        }
    }
}


void DataHandler::updateData(const std::vector<PictureRecord> &pictures) {
    std::ifstream infoFileIn(path);
    nlohmann::json data;

    // If something goes wrong, return
    // That means all wins and total counter = 0

    // try {
    //     infoFile.open("statistics.json");
    // }
    // catch(...) {
    //     return;
    // }

    if (infoFileIn.peek() != std::fstream::traits_type::eof()) {
        try {
            data = nlohmann::json::parse(infoFileIn);
        }
        catch (nlohmann::json_abi_v3_11_2::detail::parse_error) {}
        catch (std::exception& ex) {
            std::cout << ex.what() << std::endl;
            return;
        }
    }
    infoFileIn.close();

    // Go through each record and fill the info
    for (auto& picture : pictures) {
        // Temporary object
        nlohmann::json newJsonRecord;

        // Set the values to the temporary
        setItem(newJsonRecord, "wins", picture.wins);
        setItem(newJsonRecord, "total", picture.total);

        // Create or replace record with a new one
        data[picture.name] = newJsonRecord;
    }

    std::ofstream infoFileOut(path);
    infoFileOut << data;
}