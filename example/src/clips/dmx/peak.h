#pragma once
#include "./soundReactiveDmx.h"

namespace clips {
    class peak : public soundReactiveDmx {
    public:
        peak() : soundReactiveDmx() {
            _name = "peak";
            _channel.set("channel", 1, 1, 512);
            _amount.addListener(this, &peak::onAmountChange);
            _amount.set("amount", 16, 1, 16);

            _value.set("value", 0, 0, 255);
            _minValue.set("minValue", 50, 0, 255);
            _maxValue.set("maxValue", 100, 0, 255);
            _speed.set("speed", 1, 0, 1);
            _random.set("random", false);
            _threshold.set("threshold", .5, 0, 1);

            _parameters.add(_channel);
            _parameters.add(_amount);
            _parameters.add(_value);
            _parameters.add(_minValue);
            _parameters.add(_maxValue);
            _parameters.add(_speed);
            _parameters.add(_random);
            _parameters.add(_threshold);

            _timestamp = ofGetElapsedTimeMillis();
        }
        
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();

            for(auto i = 0; i < _amount; i++) {
                if(timestamp - _timestamps[i] < 1000){
                    _values[i] = ofMap(timestamp, _timestamps[i], _timestamps[i] + 1000, _maxValue, _minValue);
                    std::pair<int, int> value(_channel+i, _values[i]);
                    _valueChangeEvent.notify(value);
                }
            }
        }
        
        void setPeakEnergy(float value) {
            if(value < _threshold){
                return;
            }

            if(_random){
                auto i = ofRandom(_amount);
                _timestamps[i] = ofGetElapsedTimeMillis();
                _values[i] = _maxValue*value;
            }else{
                for(auto i = 0; i < _amount; i++){
                    _timestamps[i] = ofGetElapsedTimeMillis();
                    _values[i] = _maxValue*value;
                }
            }
        }
        
        void onAmountChange(int & value){
            _values.resize(value);
            _timestamps.resize(value);
        }
        
        ofParameter<int> _channel;
        ofParameter<int> _amount;
        ofParameter<int> _value;
        std::vector<int> _values;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        
        ofParameter<float> _speed;
        ofParameter<bool> _random;
        ofParameter<float> _threshold;

        u_int64_t _timestamp;
        std::vector<u_int64_t> _timestamps;

    };
};
