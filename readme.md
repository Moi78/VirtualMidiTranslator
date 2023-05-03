# VirtualMidiTranslator
A minimalist midi to midi translator.

## Usage
### Launching VMT 
When prompted select the desired input port and the desired output port. \
Example:
```bash
$ ./virtualmiditranslator --config "config.json"
[MIDI INPUT] Port no 0 : Midi Through:Midi Through Port-0 14:0
[MIDI INPUT] Port no 1 : DDJ-XP2:DDJ-XP2 MIDI 1 20:0
Input port [0]: 1
[MIDI OUTPUT] Port no 0 : Midi Through:Midi Through Port-0 14:0
[MIDI OUTPUT] Port no 1 : DDJ-XP2:DDJ-XP2 MIDI 1 20:0
Output port [0]: 0
```

### Writing configurations
Configurations are written in JSON. The mapping configuration is put in a `map` array as a collection of JSON objects in which you can have two types of objects:
* Single notes. It maps a single note to another
```json
{
  "type": "single_note",
  "base": [channel, note],
  "trans": [translated_channel, translated_note]
}
```
* Through notes. It translates a range of notes to another range. Mapped notes are computed using the following fomrula : `[channel = abs(channel_a - channel_b) + translated_channel + channel_x, note = note_x + translated_note]`
```json
{
  "type": "through",
  "range_low_base": [channel_a, note_a],
  "range_high_base": [channel_b, note_b],
  
  "trans_at": [translated_channel, translated_note]
}
```

#### Example of configuration
```json
{
  "map" : [
    {
      "type": "single_note",
      "base": [9, 50],
      "trans": [1, 1]
    },

    {
      "type": "single_note",
      "base": [9, 51],
      "trans": [1, 2]
    },

    {
      "type": "through",
      "range_low_base": [5, 0],
      "range_high_base": [8, 49],

      "trans_at": [2, 0]
    }
  ]
}
```

In this example, note `[channel = 9, note = 50]` will be mapped to `[channel = 1, note = 1]`, note `[channel = 9, note = 51]` will be mapped to `[channel = 1, note = 2]` and the range of notes from `[channel = 5, note = 0]` through `[channel = 8, note = 49]` will be translated at `[channel = 2, note = 0]`, this means that, for instance, the note `[channel = 6, note = 20]` will be translated to `[channel = 3, note = 20]`.
