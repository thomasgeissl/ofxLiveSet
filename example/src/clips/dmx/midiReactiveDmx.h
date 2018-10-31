#pragma once
#include "clips/dmx.h"

namespace clips {
    class midiReactiveDmx : public ofxLiveSet::clip::dmx {
    public:
        midiReactiveDmx() : dmx(){}
        virtual void setNoteOn(int note, int velocity){}
        virtual void setNoteOff(int note, int velocity){}
    };
};
