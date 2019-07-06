#pragma once
#include "ofxLiveSet.h"

namespace clips {
    class midi2dmx : public ofxLiveSet::clip::dmx, public ofxLiveSet::clip::midiReactive {
    public:
        typedef std::shared_ptr<midi2dmx> pointer;
        static pointer create()
        {
            return std::make_shared<midi2dmx>();
        }
        midi2dmx() : ofxLiveSet::clip::dmx(), ofxLiveSet::clip::midiReactive() {
            _name = "midi2dmx";
            _active.setName(_name);
            _parameters.add(_channel);
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
