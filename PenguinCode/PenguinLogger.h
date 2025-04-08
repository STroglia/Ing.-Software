#ifndef PENGUIN_LOGGER_H
#define PENGUIN_LOGGER_H

#include <string>

class PenguinLogger {
public:
    static void logFishingActivity(const std::string& message);
};

#endif