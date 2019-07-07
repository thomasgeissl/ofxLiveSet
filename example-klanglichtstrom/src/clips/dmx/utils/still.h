#pragma once
#include "clips/dmx.h"

namespace clips {
    class still : public ofxLiveSet::clip::dmx {
    public:
        typedef std::shared_ptr<still> pointer;
        static pointer create()
        {
            return std::make_shared<still>();
        }
        still(int channel = 1, int amount = 16) : ofxLiveSet::clip::dmx("still"), _amount(amount) {
            _name ="still";
            _channel.set("channel", channel, 1, 512);
            _active.setName(_name);
            _parameters.add(_channel);
            _values.resize(_amount);
            for(auto i = 0; i < _amount; i++) {
                _values[i].set(ofToString(i), 0, 0, 255);
                _parameters.add(_values[i]);
            }
        }
        
        void update() {
            for(auto i = 0; i < _amount; i++) {
                setValue(_channel+i, _values[i]);
            }
        }
        
        ofParameter<int> _channel;
        int _amount;
        std::vector<ofParameter<int>> _values;
    };
};
