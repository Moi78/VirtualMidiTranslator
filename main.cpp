#include <iostream>
#include <string>
#include <memory>

#include "rtmidi/RtMidi.h"

#include "MidiMapper.h"

int main(int argc, char* argv[]) {
    // Checking CLI arguments
    std::string confFilePath;
    if(argc < 3) {
        std::cerr << "ERROR: No configuration provided." << std::endl;
        exit(-1);
    } else {
        if(std::string(argv[1]) == "--config") {
            confFilePath = std::string(argv[2]);
        } else {
            std::cerr << "ERROR: Unknown argument " << std::string(argv[1]) << "." << std::endl;
            exit(-1);
        }
    }

    // Opening MIDI Port
    std::unique_ptr<RtMidiIn> midiin = std::make_unique<RtMidiIn>();
    std::unique_ptr<RtMidiOut> midiout = std::make_unique<RtMidiOut>();
    unsigned int inp_select = 0, outp_select = 0;

    unsigned int nbPorts = midiin->getPortCount();
    std::string portName;
    for(int i = 0; i < nbPorts; i++) {
        portName = midiin->getPortName(i);
        std::cout << "[MIDI INPUT] Port no " << i << " : " << portName << std::endl;
    }
    std::cout << "Input port : ";
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
    std::cout << "Output port : ";
    std::cin >> outp_select;

    if(outp_select >= nbPort_out) {
        std::cerr << "ERROR: This port does not exists" << std::endl;
        exit(-1);
    }

    midiin->openPort(inp_select);
    midiout->openPort(outp_select);

    // Mapping midi notes
    MidiMapper mapper{};
    mapper.LoadMappingConfig(confFilePath);

    while(true) {
        std::vector<uint8_t> data;
        midiin->getMessage(&data);

        if(data.size() >= 3) {
            MidiNote in = {
                    .channel = (uint8_t)(data[0] & 0x0F),
                    .note = data[1],
                    .velocity = data[2]
            };

            if(!mapper.IsNoteSinked(in)) {
                MidiNote mapped = mapper.MapNote(in);

                std::cout << (int) (data[0] & 0x0F) << " " << (int) data[1] << " " << (int) data[2];
                std::cout << " -> " << (int) (mapped.channel & 0x0F) << " " << (int) mapped.note << " "
                          << (int) mapped.velocity;
                std::cout << std::endl;

                midiout->sendMessage((unsigned char *) &mapped, sizeof(mapped));
            } else {
                std::cout << "Sinked " << (int) (data[0] & 0x0F) << " " << (int) data[1] << " " << (int) data[2] << std::endl;
            }
        }
    }

    return 0;
}
