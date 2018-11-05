#pragma once
#include "./soundReactiveDmx.h"

namespace clips {
    class rand : public soundReactiveDmx {
    public:
        rand(int channel) : soundReactiveDmx(), _channel(channel) {
            _name = "rand";
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
