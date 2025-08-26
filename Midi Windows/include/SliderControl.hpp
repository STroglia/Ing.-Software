#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Output.H>
#include <memory>
#include <string>
#include "MidiService.hpp"

struct SliderConfig
{
    int cc_number;
    std::string description;
    int min_value;
    int max_value;
};

class SliderControl
{
public:
    SliderControl(const SliderConfig& config, std::shared_ptr<MidiService> midiService);
    ~SliderControl() = default;

    void createWidgets(int x, int y, int w, int h, unsigned char* currentMidiChannel);
    Fl_Widget* getWidgetGroup();
    Fl_Slider* getFlSlider() { return m_slider; }

    int getCcNumber() const;
    std::string getDescription() const;
    std::string getRange() const;
    int getCurrentValue() const;
    void setCurrentValue(int value);
    void setActive(bool active);
    bool isActive() const;

    // Callbacks
    static void sliderCallback_static(Fl_Widget* w, void* userdata);
    static void onCheckboxClicked_static(Fl_Widget* w, void* userdata);

private:
    void sliderCallback();
    void onCheckboxClicked();

private:
    SliderConfig m_config;
    std::shared_ptr<MidiService> m_midiService;
    unsigned char* m_currentMidiChannel;

    Fl_Group* m_group;
    Fl_Check_Button* m_checkButton;
    Fl_Box* m_label;
    Fl_Slider* m_slider;
    Fl_Value_Output* m_valueOutput;

    bool m_isActive;
    std::string m_tooltipText;
};
