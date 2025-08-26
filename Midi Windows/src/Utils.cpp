#include "Utils.hpp"
#include <algorithm>

namespace Utils
{
    std::string getFileNameFromPath(const std::string& filepath)
    {
        size_t last_slash = filepath.find_last_of('/');
        size_t last_backslash = filepath.find_last_of('\\');

        size_t last_separator = std::string::npos;
        if (last_slash != std::string::npos)
            last_separator = last_slash;
        if (last_backslash != std::string::npos)
        {
            if (last_separator == std::string::npos || last_backslash > last_separator)
                last_separator = last_backslash;
        }

        if (last_separator == std::string::npos)
            return filepath;
        else
            return filepath.substr(last_separator + 1);
    }

    std::string getDirectoryFromPath(const std::string& filePath)
    {
        size_t found = filePath.find_last_of("/\\");
        if (found != std::string::npos)
            return filePath.substr(0, found);
        return ".";
    }
}
