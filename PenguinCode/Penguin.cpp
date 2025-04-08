#include "Penguin.h"

Penguin::Penguin(const std::string& name) : name(name) {}

std::string Penguin::fish() const {
    return name + " ha pescado un pez 🐟";
}

std::string Penguin::getName() const {
    return name;
}