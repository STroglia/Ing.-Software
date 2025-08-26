#include "MidiLayoutParser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

bool MidiLayoutParser::parse(const std::string& filename, std::vector<SliderConfig>& configs)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Cannot open layout file: " << filename << std::endl;
        return false;
    }

    configs.clear();
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;

        SliderConfig config;
        // Asumimos CSV: cc_number,description,min_value,max_value
        if (std::getline(ss, token, ','))
            config.cc_number = std::stoi(token);
        if (std::getline(ss, token, ','))
            config.description = token;
        if (std::getline(ss, token, ','))
            config.min_value = std::stoi(token);
        if (std::getline(ss, token, ','))
            config.max_value = std::stoi(token);

        configs.push_back(config);
    }

    file.close();
    return true;
}
