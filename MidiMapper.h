#ifndef VIRTUALMIDITRANSLATOR_MIDIMAPPER_H
#define VIRTUALMIDITRANSLATOR_MIDIMAPPER_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>

#include "jsoncpp/json/json.h"

struct MidiNote {
    uint8_t channel;
    uint8_t note;
    uint8_t velocity;

    uint32_t hash() const;
};

class MidiMapper {
public:
    MidiMapper() = default;
    ~MidiMapper() = default;

    MidiNote MapNote(const MidiNote& noteIn);

    void AddMappedNote(const MidiNote& noteBase, const MidiNote& translated);
    void LoadMappingConfig(std::string& filepath);

private:
    std::unordered_map<uint32_t, MidiNote> m_mapping;
};


#endif //VIRTUALMIDITRANSLATOR_MIDIMAPPER_H
