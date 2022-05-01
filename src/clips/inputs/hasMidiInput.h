#pragma once
#include "./base.h"
namespace ofxLiveSet {
namespace clip {
    class hasMidiInput{
    public:
        hasMidiInput(){
            _channel.set("channel", 1, 1, 16);
        }
        virtual void setNoteOn(int note, int velocity){}
        virtual void setNoteOff(int note, int velocity){}
        
        ofParameter<int> _channel;

    };
};
};