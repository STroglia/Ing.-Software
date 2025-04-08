#ifndef PENGUIN_H
#define PENGUIN_H

#include <string>
#include "IFishable.h"  // 👈 Incluímos la interfaz

class Penguin : public IFishable { // 👈 Hereda de IFishable
private:
    std::string name;

public:
    Penguin(const std::string& name);
    std::string fish() const override;     // 👈 Implementa fish() de la interfaz
    std::string getName() const;
};

#endif
