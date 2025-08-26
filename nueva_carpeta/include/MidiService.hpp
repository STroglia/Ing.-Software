#pragma once
#include <memory>
#include <string>
#include <vector>
#include "RtMidi.h"

class MidiService
{
public:
    MidiService();
    ~MidiService();

    bool openPort(unsigned int portNumber);
    void closePort();
    bool isPortOpen() const;
    unsigned int getPortCount() const;
    std::string getPortName(unsigned int portNumber) const;
    void sendCcMessage(unsigned char channel, unsigned char cc, unsigned char value);
    std::string getInitializationError() const { return m_errorString; }

private:
    std::unique_ptr<RtMidiOut> m_midiOut;
    std::string m_errorString;
};
