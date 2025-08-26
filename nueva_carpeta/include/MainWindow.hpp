#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Scroll.H>
#include <memory>
#include <vector>
#include <string>
#include "SliderControl.hpp"
#include "MidiService.hpp"

struct PresetValue
{
    int value;
    bool active;
};

class MainWindow
{
public:
    MainWindow(int width, int height, const char* title, std::shared_ptr<MidiService> midiService);
    ~MainWindow();

    void show(int argc, char** argv);

private:
    void clearDynamicControls();
    void addSliderControl(const SliderConfig& config, int y_position);
    bool loadMidiLayoutFromFile(const std::string& filename);
    void updateStatus(const std::string& message);

    void populateMidiPorts();

    // Callbacks
    void onPortSelected();
    void onChannelSelected();
    void onLoadLayout();
    void onLoadPreset();
    void onSavePreset();
    void onResetAll();
    void onSendAll();

    static void onPortSelected_static(Fl_Widget* w, void* userdata);
    static void onChannelSelected_static(Fl_Widget* w, void* userdata);
    static void onLoadLayout_static(Fl_Widget* w, void* userdata);
    static void onLoadPreset_static(Fl_Widget* w, void* userdata);
    static void onSavePreset_static(Fl_Widget* w, void* userdata);
    static void onResetAll_static(Fl_Widget* w, void* userdata);
    static void onSendAll_static(Fl_Widget* w, void* userdata);

private:
    std::shared_ptr<MidiService> m_midiService;

    Fl_Window* m_window;
    Fl_Box* m_statusBox;
    Fl_Choice* m_portChoice;
    Fl_Choice* m_channelChoice;
    Fl_Button* m_loadLayoutButton;
    Fl_Button* m_loadPresetButton;
    Fl_Button* m_savePresetButton;
    Fl_Button* m_resetAllButton;
    Fl_Button* m_sendAllButton;
    Fl_Scroll* m_scrollGroup;

    std::vector<std::unique_ptr<SliderControl>> m_controls;

    unsigned char m_currentMidiChannel;

    std::string m_lastLayoutPath;
    std::string m_lastPresetPath;
};
