#include "../lib/SHTC3.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <sstream> 
#include <chrono>
#include <filesystem> 

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
        ss << std::put_time(std::localtime(&in_time_t), "%H:%M");
    }
    return ss.str();
}

int main(int argc, char** argv) {
    int avgCount = 6;                                   // Default average count
    std::string basePath = "/var/www/html/tm/data"; // Default database

    if (argc > 1) {
        basePath = argv[1];
    }
    if (argc > 2) {
        avgCount = std::atoi(argv[2]);
    }

    SHTC3 sensor(1); 
    float totalTemperature = 0.0, totalHumidity = 0.0;
    for(int i=0;i < avgCount; i++ )
    {
        auto [temperature, humidity] = sensor.getMeasurements();
        totalTemperature += temperature;
        totalHumidity += humidity;
        usleep(1000);
    }
    
    std::string filename = basePath + "/min/" + currentDateTime(true) + ".json";
    
    if (std::filesystem::exists(filename)) {
        // File exists, modify it by removing the last character
        std::ifstream fileIn(filename, std::ios::ate); // Open for reading at the end to find file size
        auto fileSize = fileIn.tellg();
        fileIn.close();
        if (fileSize > 0) {
            std::filesystem::resize_file(filename, fileSize - 1); // Remove the last character (presumably the closing ']')
        }
        std::ofstream fileOut(filename, std::ios::app);
        fileOut << std::setprecision(2)
        << "{\"tmp\": " << totalTemperature / avgCount
        << ", \"hmt\": " << totalHumidity / avgCount
        << ", \"time\": \"" << currentDateTime(false) << "\"}]";
        fileOut.close();
    }
    else {
        std::ofstream fileOut(filename);
        fileOut << "[";
        fileOut << std::setprecision(2)
         << "{\"tmp\": " << totalTemperature / avgCount
         << ", \"hmt\": " << totalHumidity / avgCount
         << ", \"time\": \"" << currentDateTime(false) << "\"}]";
         fileOut.close();
    }

    return 0;
}