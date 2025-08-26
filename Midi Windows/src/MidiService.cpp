#include "MidiService.hpp"
#include <iostream>

MidiService::MidiService() 
{
    try {
        m_midiOut = std::make_unique<RtMidiOut>();
    } catch (const RtMidiError& error) {
        m_errorString = error.getMessage();
        std::cerr << "RtMidi Initialization Error: " << m_errorString << std::endl;
        m_midiOut = nullptr;
    }
}

MidiService::~MidiService()
{
    closePort();
}

bool MidiService::openPort(unsigned int portNumber)
{
    if (!m_midiOut || isPortOpen() || portNumber >= m_midiOut->getPortCount())
        return false;

    try {
        m_midiOut->openPort(portNumber);
        return true;
    } catch (const RtMidiError& error) {
        std::cerr << "Error opening MIDI port: " << error.getMessage() << std::endl;
        return false;
    }
}

void MidiService::closePort()
{
    if (m_midiOut && m_midiOut->isPortOpen())
        m_midiOut->closePort();
}

bool MidiService::isPortOpen() const
{
    return m_midiOut && m_midiOut->isPortOpen();
}

unsigned int MidiService::getPortCount() const
{
    return m_midiOut ? m_midiOut->getPortCount() : 0;
}

std::string MidiService::getPortName(unsigned int portNumber) const
{
    if (!m_midiOut || portNumber >= getPortCount())
        return "";
    return m_midiOut->getPortName(portNumber);
}

void MidiService::sendCcMessage(unsigned char channel, unsigned char cc, unsigned char value)
{
    if (!isPortOpen()) return;
    std::vector<unsigned char> message = { static_cast<unsigned char>(0xB0 | channel), cc, value };
    try { m_midiOut->sendMessage(&message); }
    catch (const RtMidiError& error) { std::cerr << "Error sending MIDI message: " << error.getMessage() << std::endl; }
}
