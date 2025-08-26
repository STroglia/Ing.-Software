#include "MidiPresetParser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

bool MidiPresetParser::load(const std::string& filename, std::map<int, PresetValue>& presetData)
{
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    presetData.clear();
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;

        int cc;
        int value;
        bool active;

        if (std::getline(ss, token, ','))
            cc = std::stoi(token);
        if (std::getline(ss, token, ','))
            value = std::stoi(token);
        if (std::getline(ss, token, ','))
            active = std::stoi(token) != 0;

        presetData[cc] = { value, active };
    }

    file.close();
    return true;
}

bool MidiPresetParser::save(const std::string& filename, const std::vector<std::unique_ptr<SliderControl>>& controls)
{
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    for (const auto& control : controls)
    {
        file << control->getCcNumber() << ","
             << control->getCurrentValue() << ","
             << (control->isActive() ? 1 : 0)
             << "\n";
    }

    file.close();
    return true;
}
