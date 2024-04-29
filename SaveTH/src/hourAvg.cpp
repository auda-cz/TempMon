#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include <chrono>
#include <sstream>

using json = nlohmann::json;
namespace fs = std::filesystem;

std::string currentDateTime(bool isdata) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    if(isdata)
    {
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    }
    else
    {
        ss << std::put_time(std::localtime(&in_time_t), "%H");
    }
    return ss.str();
}


std::string formatDateForFile(bool isNewDay) {
    if(isNewDay){
        return "00:00";
    }
    else{
        return currentDateTime(false) + ":00";
    }
}

void processHourlyData(const std::string& basePath) {
    std::string filename = basePath + "/min/" + currentDateTime(true) + ".json";
    
    if (!fs::exists(filename)) {
        std::cerr << "No data file found for the current time slot." << std::endl;
        return;
    }
    
    std::ifstream fileIn(filename);
    json data;
    fileIn >> data;
    fileIn.close();

    float totalTemperature = 0.0, totalHumidity = 0.0;
    bool isNewDay = data.size() < 12;
    int start = isNewDay ? 0 : data.size() - 12;
    int stop  = data.size();
    for (int i = start; i < stop; ++i) {
        totalTemperature += data[i]["tmp"].get<float>();
        totalHumidity += data[i]["hmt"].get<float>();
    }
    int count = isNewDay ? stop : 12;

    std::ostringstream tempStream, humidStream;
    tempStream << std::fixed << std::setprecision(2) << totalTemperature / count;
    humidStream << std::fixed << std::setprecision(2) << totalHumidity / count;
    json newEntry = {
        {"tmp", tempStream.str()},
        {"hmt", humidStream.str()},
        {"time", formatDateForFile(isNewDay)}
    };

    json hourData;
    std::string hourFilename = basePath + "/hour/" + currentDateTime(true) + ".json";
    if (fs::exists(hourFilename)) {
        std::ifstream hourFileIn(hourFilename);
        hourFileIn >> hourData;
        hourFileIn.close();
    } else {
        hourData = json::array();
    }

    hourData.push_back(newEntry);
    std::ofstream hourFileOut(hourFilename);
    hourFileOut << std::setw(4) << hourData;
    hourFileOut.close();
}

int main(int argc, char** argv) {
    std::string basePath = "/var/www/html/tm/data";
    if (argc > 1) {
        basePath = argv[1];
    }
    sleep(5);

    processHourlyData(basePath);

    return 0;
}