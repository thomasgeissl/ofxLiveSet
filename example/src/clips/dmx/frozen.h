#pragma once
#include "./soundReactiveDmx.h"

namespace clips {
    class frozen : public soundReactiveDmx {
    public:
        frozen(int channel = 1, int amount = 16) : soundReactiveDmx(), _amount(amount) {
            _name ="frozen";
            _channel.set("channel", channel, 1, 512);
            _parameters.add(_channel);
            _values.resize(_amount);
            for(auto i = 0; i < _amount; i++) {
                _values[i].set(ofToString(i), 0, 0, 255);
                _parameters.add(_values[i]);
            }
        }
        
        void update() {
            for(auto i = 0; i < _amount; i++) {
                std::pair<int, int> value(_channel+i, _values[i]);
                _valueChangeEvent.notify(value);
            }
        }
        
        ofParameter<int> _channel;
        int _amount;
        std::vector<ofParameter<int>> _values;
    };
};
