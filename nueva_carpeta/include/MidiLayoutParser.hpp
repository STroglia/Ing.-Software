#pragma once
#include <vector>
#include <string>
#include "SliderControl.hpp"

class MidiLayoutParser
{
public:
    // Parsea un CSV de layout MIDI y llena configs
    static bool parse(const std::string& filename, std::vector<SliderConfig>& configs);
};
