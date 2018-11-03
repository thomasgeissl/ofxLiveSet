#pragma once
#include "./midiReactiveDmx.h"

namespace clips {
    class midi2dmx : public midiReactiveDmx {
    public:
        midi2dmx() : midiReactiveDmx() {
            _name = "midi2dmx";
            _active.setName(_name);
        }
        
        void setNoteOn(int note, int velocity) {
            std::pair<int, int> value(note, velocity * 2);
            _valueChangeEvent.notify(value);
        }
        void setNoteOff(int note, int velocity) {
            std::pair<int, int> value(note, 0);
            _valueChangeEvent.notify(value);
        }
    };
};
