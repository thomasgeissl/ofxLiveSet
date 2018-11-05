#pragma once
#include "clips/dmx.h"

namespace clips {
    class midiReactiveDmx : public ofxLiveSet::clip::dmx {
    public:
        midiReactiveDmx() : dmx(){
            _channel.set("analyserId", 1, 1, 16);
            _parameters.add(_channel);
        }
        virtual void setNoteOn(int note, int velocity){}
        virtual void setNoteOff(int note, int velocity){}
        
        ofParameter<int> _channel;

    };
};
