#include "Penguin.h"
#include "PenguinLogger.h"

int main() {
    Penguin p1("Tomy the Penguin");
    std::string actividad = p1.fish();  // Principio de Responsabilidad Única: solo pesca

    PenguinLogger::logFishingActivity(actividad);  // Otra clase se encarga de mostrarlo/loguearlo

    return 0;
}
