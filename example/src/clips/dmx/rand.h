#pragma once
#include "clips/dmx.h"

namespace clips {
    class rand : public ofxLiveSet::clip::dmx {
    public:
        rand(int channel) : _channel(channel) {
            _channel.set("channel", channel, 1, 512);
            _parameters.add(_channel);
        }
        
        void update(){
            std::pair<int, int> value(_channel, ofRandom(0, 255));
            _valueChangeEvent.notify(value);
        }
        
        ofParameter<int> _channel;
    };
};
