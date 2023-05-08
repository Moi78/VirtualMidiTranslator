#ifndef VIRTUALMIDITRANSLATOR_MIDIMAPPER_H
#define VIRTUALMIDITRANSLATOR_MIDIMAPPER_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>

#include "jsoncpp/json/json.h"

#define NOTE_OFF 0x80
#define NOTE_ON 0x90
#define POLYPHONIC_PRESS 0xA0
#define CONTROLE_CHANGE 0xB0
#define PROGRAM_CHANGE 0xC0
#define CHANNEL_PRESSURE 0xD0
#define PITCH_BEND 0xE0
#define SYSTEM_MESSAGE 0xF0

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

    bool IsNoteSinked(MidiNote& noteIn);
    MidiNote MapNote(MidiNote& noteIn);

    void AddMappedNote(const MidiNote& noteBase, const MidiNote& translated);
    void AddSinkedNote(const MidiNote& note);
    void LoadMappingConfig(std::string filepath);

private:
    void CreateSingleNoteMap(Json::Value data);
    void CreateThroughNoteMap(Json::Value data);

    void CreateSingleNoteSink(Json::Value data);
    void CreateThroughNoteSink(Json::Value data);

    std::unordered_map<uint32_t, MidiNote> m_mapping;
    std::unordered_map<uint32_t, MidiNote> m_sinked_notes;
};


#endif //VIRTUALMIDITRANSLATOR_MIDIMAPPER_H
