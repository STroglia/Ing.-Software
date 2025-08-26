#pragma once
#include <string>
#include <map>
#include "SliderControl.hpp"

struct PresetValue
{
    int value;
    bool active;
};

class MidiPresetParser
{
public:
    static bool load(const std::string& filename, std::map<int, PresetValue>& presetData);
    static bool save(const std::string& filename, const std::vector<std::unique_ptr<SliderControl>>& controls);
};
