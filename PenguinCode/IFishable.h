#ifndef IFISHABLE_H
#define IFISHABLE_H

#include <string>

class IFishable {
public:
    virtual std::string fish() const = 0; // Método puro
    virtual ~IFishable() = default;       // Destructor virtual (buena práctica)
};

#endif
