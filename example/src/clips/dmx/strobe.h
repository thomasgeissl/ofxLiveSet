#pragma once
#include "./soundReactiveDmx.h"

namespace clips {
    class strobe : public soundReactiveDmx {
    public:
        strobe() : soundReactiveDmx() {
            _name = "strobe";
            _channel.set("channel", 1, 1, 512);
            _amount.set("amount", 16, 1, 16);
            _minValue.set("minValue", 50, 0, 255);
            _maxValue.set("maxValue", 100, 0, 255);
            _speed.set("speed", 1, 0, 1);

            _parameters.add(_channel);
            _parameters.add(_amount);
            _parameters.add(_minValue);
            _parameters.add(_maxValue);
            _parameters.add(_speed);

            _timestamp = ofGetElapsedTimeMillis();
        }

        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(timestamp - _timestamp < ofMap(_speed, 0, 1, 1000, 20)){ return; }
            
            _timestamp = timestamp;
            _value = !_value;

            for(auto i = 0; i < _amount; i++){
                std::pair<int, int> value(_channel+i, _value ? _maxValue : _minValue);
                _valueChangeEvent.notify(value);
            }
        }

        void setPeakEnergy(int analyserId, float value){}

        ofParameter<int> _channel;
        ofParameter<int> _amount;
        bool _value;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        ofParameter<float> _speed;

        u_int64_t _timestamp;
        int _group;
    };
};
