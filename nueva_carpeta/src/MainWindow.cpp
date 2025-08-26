#include "MainWindow.hpp"
#include "MidiLayoutParser.hpp"
#include "MidiPresetParser.hpp"
#include "Utils.hpp"
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <algorithm>
#include <map>

MainWindow::MainWindow(int width, int height, const char* title, std::shared_ptr<MidiService> midiService)
    : m_midiService(midiService), m_lastLayoutPath("."), m_lastPresetPath(".")
{
    m_window = new Fl_Window(width, height, title);
    m_window->begin();

    int current_y = 10;

    // Status Box
    m_statusBox = new Fl_Box(10, current_y, width - 20, 25, "Status: Initializing...");
    m_statusBox->box(FL_THIN_UP_BOX);
    m_statusBox->align(FL_ALIGN_LEFT | FL_ALIGN_WRAP | FL_ALIGN_INSIDE);
    current_y += 35;

    // MIDI Port
    new Fl_Box(10, current_y, 80, 25, "MIDI Port:");
    m_portChoice = new Fl_Choice(100, current_y, 280, 25);
    m_portChoice->callback(onPortSelected_static, this);
    populateMidiPorts();
    current_y += 35;

    // MIDI Channel
    new Fl_Box(10, current_y, 80, 25, "MIDI Channel:");
    m_channelChoice = new Fl_Choice(100, current_y, 100, 25);
    for (int i = 1; i <= 16; ++i) m_channelChoice->add(std::to_string(i).c_str());
    m_channelChoice->value(0);
    m_currentMidiChannel = 0;
    m_channelChoice->callback(onChannelSelected_static, this);
    current_y += 35;

    // Buttons
    int button_x = 10;
    int button_width = 107;
    int button_height = 25;
    int button_spacing = 10;

    m_loadLayoutButton = new Fl_Button(button_x, current_y, button_width, button_height, "Load Layout");
    m_loadLayoutButton->callback(onLoadLayout_static, this);
    button_x += button_width + button_spacing;

    m_loadPresetButton = new Fl_Button(button_x, current_y, button_width, button_height, "Load Preset");
    m_loadPresetButton->callback(onLoadPreset_static, this);
    button_x += button_width + button_spacing;

    m_savePresetButton = new Fl_Button(button_x, current_y, button_width, button_height, "Save Preset");
    m_savePresetButton->callback(onSavePreset_static, this);
    button_x += button_width + button_spacing;

    m_resetAllButton = new Fl_Button(button_x, current_y, button_width, button_height, "Reset All");
    m_resetAllButton->callback(onResetAll_static, this);
    button_x += button_width + button_spacing;

    m_sendAllButton = new Fl_Button(button_x, current_y, button_width, button_height, "Send All");
    m_sendAllButton->callback(onSendAll_static, this);
    current_y += 35;

    // Scroll Group
    m_scrollGroup = new Fl_Scroll(10, current_y, width - 20, height - current_y - 10);
    m_scrollGroup->type(Fl_Scroll::VERTICAL);
    m_scrollGroup->begin();
    m_scrollGroup->end();

    m_window->end();
    m_window->resizable(m_window);
    m_window->size_range(width, 250);
}

MainWindow::~MainWindow()
{
    clearDynamicControls();
}

void MainWindow::clearDynamicControls()
{
    if (m_scrollGroup) m_scrollGroup->clear();
    m_controls.clear();
}

void MainWindow::addSliderControl(const SliderConfig& config, int y_position)
{
    auto sliderControl = std::make_unique<SliderControl>(config, m_midiService);
    sliderControl->createWidgets(10, y_position, m_scrollGroup->w() - 20, 45, &m_currentMidiChannel);
    sliderControl->getFlSlider()->callback(SliderControl::sliderCallback_static, sliderControl.get());
    m_controls.push_back(std::move(sliderControl));
}

bool MainWindow::loadMidiLayoutFromFile(const std::string& filename)
{
    std::string display_name = Utils::getFileNameFromPath(filename);
    std::vector<SliderConfig> configs;
    if (!MidiLayoutParser::parse(filename, configs))
    {
        updateStatus("Error loading MIDI layout from " + display_name);
        fl_alert(("Error al cargar el layout MIDI desde:\n" + display_name).c_str());
        return false;
    }

    clearDynamicControls();

    if (configs.empty())
    {
        updateStatus("Warning: No slider configurations found in " + display_name);
        fl_alert(("Advertencia: No se encontraron configuraciones de sliders en:\n" + display_name).c_str());
        return true;
    }

    m_scrollGroup->begin();
    int current_y_in_scroll = 0;
    int slider_height = 45;
    int slider_spacing = 5;
    for (const auto& config : configs)
    {
        addSliderControl(config, current_y_in_scroll);
        current_y_in_scroll += slider_height + slider_spacing;
    }
    m_scrollGroup->end();
    m_scrollGroup->init_sizes();

    int min_height = m_scrollGroup->y() + current_y_in_scroll + 10;
    m_window->size(m_window->w(), std::max(m_window->h(), min_height));
    m_window->redraw();

    updateStatus("MIDI layout loaded from " + display_name + ". " + std::to_string(configs.size()) + " sliders created.");
    return true;
}

void MainWindow::show(int argc, char** argv)
{
    m_window->show(argc, argv);
    onPortSelected();
}

void MainWindow::updateStatus(const std::string& message)
{
    if (m_statusBox)
    {
        fl_font(m_statusBox->labelfont(), m_statusBox->labelsize());
        int text_width = 0, text_height = 0;
        fl_measure(message.c_str(), text_width, text_height);
        int box_width = text_width + 20;
        m_statusBox->size(box_width, m_statusBox->h());
        m_statusBox->copy_label(message.c_str());
        m_window->redraw();
    }
}

// --- Callbacks estáticos ---
void MainWindow::onPortSelected_static(Fl_Widget* w, void* userdata) { static_cast<MainWindow*>(userdata)->onPortSelected(); }
void MainWindow::onChannelSelected_static(Fl_Widget* w, void* userdata) { static_cast<MainWindow*>(userdata)->onChannelSelected(); }
void MainWindow::onLoadLayout_static(Fl_Widget* w, void* userdata) { static_cast<MainWindow*>(userdata)->onLoadLayout(); }
void MainWindow::onLoadPreset_static(Fl_Widget* w, void* userdata) { static_cast<MainWindow*>(userdata)->onLoadPreset(); }
void MainWindow::onSavePreset_static(Fl_Widget* w, void* userdata) { static_cast<MainWindow*>(userdata)->onSavePreset(); }
void MainWindow::onResetAll_static(Fl_Widget* w, void* userdata) { static_cast<MainWindow*>(userdata)->onResetAll(); }
void MainWindow::onSendAll_static(Fl_Widget* w, void* userdata) { static_cast<MainWindow*>(userdata)->onSendAll(); }
