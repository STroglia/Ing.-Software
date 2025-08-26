#include <FL/Fl.H>
#include <memory>
#include "MainWindow.hpp"
#include "MidiService.hpp"

int main(int argc, char** argv)
{
    // Crear la instancia de MidiService (simulada por ahora)
    auto midiService = std::make_shared<MidiService>();

    // Crear la ventana principal
    MainWindow mainWindow(800, 600, "MIDI Controller", midiService);

    // Mostrar la ventana
    mainWindow.show(argc, argv);

    // Ejecutar el loop principal de FLTK
    return Fl::run();
}
