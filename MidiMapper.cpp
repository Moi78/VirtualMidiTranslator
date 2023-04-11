#include <iostream>
#include "MidiMapper.h"

uint32_t MidiNote::hash() const {
    return note + (channel << 16);
}

void MidiMapper::AddMappedNote(const MidiNote &noteBase, const MidiNote &translated) {
    m_mapping.insert(std::pair<uint32_t, MidiNote>(noteBase.hash(), translated));
}

void MidiMapper::LoadMappingConfig(std::string& filepath) {
    if(!std::filesystem::exists(filepath)) {
        std::cout << "ERROR: This file does not exists." << std::endl;
    }
}

MidiNote MidiMapper::MapNote(const MidiNote &noteIn) {

}
