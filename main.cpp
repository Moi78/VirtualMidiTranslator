#include <iostream>
#include <string>
#include <memory>

#include "rtmidi/RtMidi.h"

int main() {
    std::unique_ptr<RtMidiIn> midiin = std::make_unique<RtMidiIn>();
    std::unique_ptr<RtMidiOut> midiout = std::make_unique<RtMidiOut>();
    unsigned int inp_select = 0, outp_select = 0;

    unsigned int nbPorts = midiin->getPortCount();
    std::string portName;
    for(int i = 0; i < nbPorts; i++) {
        portName = midiin->getPortName(i);
        std::cout << "[MIDI INPUT] Port no " << i << " : " << portName << std::endl;
    }
    std::cout << "Input port [0]: ";
    std::cin >> inp_select;

    if(inp_select >= nbPorts) {
        std::cerr << "ERROR: This port does not exists" << std::endl;
        exit(-1);
    }

    unsigned int nbPort_out = midiout->getPortCount();
    std::string portName_out;
    for(int i = 0; i < nbPort_out; i++) {
        portName_out = midiout->getPortName(i);
        std::cout << "[MIDI OUTPUT] Port no " << i << " : " << portName_out << std::endl;
    }

    if(outp_select >= nbPort_out) {
        std::cerr << "ERROR: This port does not exists" << std::endl;
        exit(-1);
    }

    midiin->openPort(inp_select);
    midiout->openPort(outp_select);

    while(true) {
        std::vector<uint8_t> data;
        midiin->getMessage(&data);

        if(data.size() > 0) {
            for (int i = 0; i < data.size(); i++) {
                std::cout << (int)data[i] << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
