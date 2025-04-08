#include "PenguinLogger.h"
#include <iostream>

void PenguinLogger::logFishingActivity(const std::string& message) {
    std::cout << "[LOG] " << message << std::endl;
}