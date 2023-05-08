#include <iostream>
#include "MidiMapper.h"

uint32_t MidiNote::hash() const {
    return note + (channel << 16);
}

void MidiMapper::AddMappedNote(const MidiNote &noteBase, const MidiNote &translated) {
    m_mapping.insert(std::pair<uint32_t, MidiNote>(noteBase.hash(), translated));
}

void MidiMapper::LoadMappingConfig(std::string filepath) {
    if(!std::filesystem::exists(filepath)) {
        std::cerr << "ERROR: File " << filepath << " does not exists." << std::endl;
        return;
    }

    std::ifstream file(filepath);
    if(!file.is_open()) {
        std::cerr << "ERROR: Cannot open file " << filepath << std::endl;
        return;
    }

    Json::Value root;
    JSONCPP_STRING errs;

    std::cout << "Loading config " << filepath << std::endl;

    Json::CharReaderBuilder builder;
    builder["collectComment"] = false;

    if(!Json::parseFromStream(builder, file, &root, &errs)) {
        std::cerr << "ERROR: Failed to parse " << filepath << ". Error : " << errs << std::endl;
        return;
    }

    Json::Value map = root["map"];
    for(auto& n : map) {
        if(n["type"].asString() == "single_note") {
            CreateSingleNoteMap(n);
        } else if(n["type"].asString() == "through") {
            CreateThroughNoteMap(n);
        } else if(n["type"].asString() == "single_note_sink") {
            CreateSingleNoteSink(n);
        } else if(n["type"].asString() == "through_note_sink") {
            CreateThroughNoteSink(n);
        } else {
            std::cerr << "Unkown mapping type." << std::endl;
        }
    }

    std::cout << "INFO: Mapped " << m_mapping.size() << " notes" << std::endl;
    std::cout << "INFO Sinking " << m_sinked_notes.size() << " notes" << std::endl;
}

void MidiMapper::CreateSingleNoteMap(Json::Value data) {
    MidiNote base = {
            (uint8_t)data["base"][0].asUInt(),
            (uint8_t)data["base"][1].asUInt(),
            0
    };

    MidiNote trans = {
            (uint8_t)data["trans"][0].asUInt(),
            (uint8_t)data["trans"][1].asUInt(),
            0
    };

    AddMappedNote(base, trans);
}

void MidiMapper::CreateThroughNoteMap(Json::Value data) {
    uint8_t b_baseChannel = data["range_low_base"][0].asInt();
    uint8_t b_highChannel = data["range_high_base"][0].asInt();

    uint8_t b_baseNote = data["range_low_base"][1].asInt();
    uint8_t b_highNote = data["range_high_base"][1].asInt();

    uint8_t delta_channel = std::abs(b_baseChannel - b_highChannel) + 1;
    uint8_t delta_notes = std::abs(b_baseNote - b_highNote) + 1;

    uint8_t t_baseChannel = data["trans_at"][0].asInt();
    uint8_t t_baseNote = data["trans_at"][1].asInt();

    for(uint8_t c = 0; c < delta_channel; c++) {
        for(uint8_t n = 0; n < delta_notes; n++) {
            MidiNote base = {
                    .channel = (uint8_t)(c + b_baseChannel),
                    .note = (uint8_t)(n + b_baseNote),
                    .velocity = 0
            };

            MidiNote trans = {
                    .channel = (uint8_t)((c + t_baseChannel) | NOTE_ON),
                    .note = (uint8_t)(n + t_baseNote),
                    .velocity = 0
            };

            AddMappedNote(base, trans);
        }
    }
}

void MidiMapper::CreateSingleNoteSink(Json::Value data) {
    MidiNote sinked = {
            .channel = (uint8_t)data["note"][0].asUInt(),
            .note = (uint8_t)data["note"][1].asUInt(),
            .velocity = 0
    };

    AddSinkedNote(sinked);
}

MidiNote MidiMapper::MapNote(MidiNote &noteIn) {
    uint32_t h = noteIn.hash();

    if(!m_mapping.contains(h)) {
        noteIn.channel |= NOTE_ON;
        return noteIn;
    }

    MidiNote mapped = m_mapping[h];
    mapped.channel |= NOTE_ON;
    mapped.velocity = noteIn.velocity;

    return mapped;
}

bool MidiMapper::IsNoteSinked(MidiNote &noteIn) {
    uint32_t h = noteIn.hash();

    return m_sinked_notes.contains(h);
}

void MidiMapper::AddSinkedNote(const MidiNote &note) {
    m_sinked_notes[note.hash()] = note;
}

void MidiMapper::CreateThroughNoteSink(Json::Value data) {
    uint8_t b_baseChannel = data["range_low"][0].asInt();
    uint8_t b_highChannel = data["range_high"][0].asInt();

    uint8_t b_baseNote = data["range_low"][1].asInt();
    uint8_t b_highNote = data["range_high"][1].asInt();

    uint8_t delta_channel = std::abs(b_baseChannel - b_highChannel) + 1;
    uint8_t delta_notes = std::abs(b_baseNote - b_highNote) + 1;

    for(uint8_t c = 0; c < delta_channel; c++) {
        for(uint8_t n = 0; n < delta_notes; n++) {
            MidiNote base = {
                    .channel = (uint8_t)(c + b_baseChannel),
                    .note = (uint8_t)(n + b_baseNote),
                    .velocity = 0
            };

            AddSinkedNote(base);
        }
    }
}
