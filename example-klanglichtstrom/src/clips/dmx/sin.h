#pragma once
#include "./soundReactiveDmx.h"

namespace clips {
    class sin : public soundReactiveDmx {
    public:
        sin(int channel) : soundReactiveDmx(), _channel(channel) {
            _name = "sin";
            _channel.set("channel", channel, 1, 512);
            _minValue.set("minValue", 50, 0, 255);
            _maxValue.set("maxValue", 100, 0, 255);
            _speed.set("speed", 1, 0, 1);

            _parameters.add(_channel);
            _parameters.add(_minValue);
            _parameters.add(_maxValue);
            _parameters.add(_speed);

        }
        
        void update(){
            int dmxValue = ofMap(std::abs(std::sin(ofGetElapsedTimef()*10*_speed)), 0, 1, _minValue, _maxValue);
            ofLogNotice()<<"dmx value " << dmxValue;
            std::pair<int, int> value(_channel, dmxValue);
            _valueChangeEvent.notify(value);
        }
        
        void setPeakEnergy(int analyserId, float value) {
            _energy = value;
        }
        
        ofParameter<int> _channel;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        ofParameter<float> _speed;
        float _energy;
    };
};
