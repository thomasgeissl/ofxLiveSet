#pragma once
#include "clips/dmx.h"

namespace clips {
    class peak : public ofxLiveSet::clip::dmx {
    public:
        peak() {
            _channel.set("channel", 1, 1, 512);
            _amount.set("amount", 1, 16, 512);

            _value.set("value", 0, 0, 255);
            _minValue.set("minValue", 50, 0, 255);
            _maxValue.set("maxValue", 100, 0, 255);
            _speed.set("speed", 1, 0, 1);
            _random.set("random", false);

            _parameters.add(_channel);
            _parameters.add(_amount);
            _parameters.add(_value);
            _parameters.add(_minValue);
            _parameters.add(_maxValue);
            _parameters.add(_speed);
            _parameters.add(_random);

            _timestamp = ofGetElapsedTimeMillis();
        }
        
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(timestamp - _timestamp > 1000){
                return;
            }
            _value = ofMap(timestamp, _timestamp, _timestamp + 1000, _maxValue, _minValue);
            if(_random){
                std::pair<int, int> value(ofRandom(1, _amount), _value);
                _valueChangeEvent.notify(value);
            }else{
                for(auto i = 0; i < _amount; i++) {
                    std::pair<int, int> value(_channel+i, _value);
                    _valueChangeEvent.notify(value);
                }
            }

  
        }
        
        void bang(float value) {
            _timestamp = ofGetElapsedTimeMillis();
            _value = _maxValue*value;
        }
        
        ofParameter<int> _channel;
        ofParameter<int> _amount;
        ofParameter<int> _value;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        
        ofParameter<float> _speed;
        ofParameter<bool> _random;

        u_int64_t _timestamp;
    };
};
