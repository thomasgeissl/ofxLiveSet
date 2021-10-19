#pragma once
#include "ofMain.h"
#include "ofxMidi.h"

namespace ofxLiveSet
{
    namespace track
    {
        class hasMidiInput : public ofxMidiListener
        {
        public:
            hasMidiInput()
            {
                _midiInputParameters.setName("midi");
                _midiInputParameters.add(_active.set("active", true));
                _midiInputParameters.add(_channel.set("channel", 0, 0, 16));
            }
            ofParameterGroup _midiInputParameters;
            void newMidiMessage(ofxMidiMessage & message) = 0;

        pdsp::midi::Input _midiIn;
        pdsp::midi::Keys _midiKeys;
        pdsp::midi::Controls _midiCCs;
        private:
            ofParameter<bool> _active;
            ofParameter<int> _channel;
        };
    }
};