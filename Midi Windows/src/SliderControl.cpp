#include "SliderControl.hpp"
#include <FL/fl_draw.H>
#include <sstream>

SliderControl::SliderControl(const SliderConfig& config, std::shared_ptr<MidiService> midiService)
    : m_config(config), m_midiService(midiService), m_currentMidiChannel(nullptr),
      m_group(nullptr), m_checkButton(nullptr), m_label(nullptr),
      m_slider(nullptr), m_valueOutput(nullptr), m_isActive(true)
{
    m_tooltipText = m_config.description + " (" + std::to_string(m_config.min_value) +
                    " - " + std::to_string(m_config.max_value) + ")";
}

void SliderControl::createWidgets(int x, int y, int w, int h, unsigned char* currentMidiChannel)
{
    m_currentMidiChannel = currentMidiChannel;
    m_group = new Fl_Group(x, y, w, h);
    m_group->begin();

    // Checkbox para activar/desactivar
    m_checkButton = new Fl_Check_Button(x, y, 20, 20);
    m_checkButton->value(1);
    m_checkButton->callback(onCheckboxClicked_static, this);

    // Label descriptivo
    m_label = new Fl_Box(x + 25, y, w / 2, 20, m_config.description.c_str());
    m_label->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

    // Slider
    m_slider = new Fl_Hor_Slider(x + 25, y + 25, w - 100, 20);
    m_slider->type(FL_HOR_SLIDER);
    m_slider->bounds(m_config.min_value, m_config.max_value);
    m_slider->value(m_config.min_value);
    m_slider->callback(sliderCallback_static, this);

    // Output del valor
    m_valueOutput = new Fl_Value_Output(x + w - 70, y + 25, 60, 20);
    m_valueOutput->value(m_config.min_value);

    m_group->end();
}

Fl_Widget* SliderControl::getWidgetGroup()
{
    return m_group;
}

int SliderControl::getCcNumber() const
{
    return m_config.cc_number;
}

std::string SliderControl::getDescription() const
{
    return m_config.description;
}

std::string SliderControl::getRange() const
{
    return std::to_string(m_config.min_value) + " - " + std::to_string(m_config.max_value);
}

int SliderControl::getCurrentValue() const
{
    return static_cast<int>(m_slider->value());
}

void SliderControl::setCurrentValue(int value)
{
    if (m_slider)
    {
        m_slider->value(value);
        m_valueOutput->value(value);

        if (m_isActive && m_midiService && m_currentMidiChannel)
        {
            m_midiService->sendCcMessage(*m_currentMidiChannel, static_cast<unsigned char>(m_config.cc_number), value);
        }
    }
}

void SliderControl::setActive(bool active)
{
    m_isActive = active;
    if (m_checkButton)
        m_checkButton->value(active ? 1 : 0);
}

bool SliderControl::isActive() const
{
    return m_isActive;
}

// --- Callbacks estáticos ---
void SliderControl::sliderCallback_static(Fl_Widget* w, void* userdata)
{
    static_cast<SliderControl*>(userdata)->sliderCallback();
}

void SliderControl::onCheckboxClicked_static(Fl_Widget* w, void* userdata)
{
    static_cast<SliderControl*>(userdata)->onCheckboxClicked();
}

// --- Callbacks de instancia ---
void SliderControl::sliderCallback()
{
    if (m_valueOutput && m_slider)
    {
        m_valueOutput->value(m_slider->value());
    }

    if (m_isActive && m_midiService && m_currentMidiChannel)
    {
        m_midiService->sendCcMessage(*m_currentMidiChannel, static_cast<unsigned char>(m_config.cc_number), static_cast<unsigned char>(m_slider->value()));
    }
}

void SliderControl::onCheckboxClicked()
{
    if (m_checkButton)
    {
        m_isActive = m_checkButton->value() != 0;
    }
}
