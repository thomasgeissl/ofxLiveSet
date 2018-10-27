#pragma once
#include "clips/dmx.h"

namespace clips {
    class sines : public ofxLiveSet::clip::dmx {
    public:
        sines() : dmx() {
            _name = "sines";
            _channel.set("channel", 1, 1, 512);
            _amount.set("amount", 16, 1, 512);
            _minValue.set("minValue", 0, 0, 255);
            _maxValue.set("maxValue", 255, 0, 255);
            _speed.set("speed", .1, 0, 1);
            
            _parameters.add(_channel);
            _parameters.add(_minValue);
            _parameters.add(_maxValue);
            _parameters.add(_speed);
        }
        
        void update(){
            for(auto i = 0; i < _amount; i++){
                int dmxValue = ofMap(std::abs(std::sin(ofGetElapsedTimef()*10*_speed+0.4*i)), 0, 1, _minValue, _maxValue);
                std::pair<int, int> value(_channel+i, dmxValue);
                _valueChangeEvent.notify(value);
            }
      
        }
        
        ofParameter<int> _channel;
        ofParameter<int> _amount;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        ofParameter<float> _speed;
    };
};
