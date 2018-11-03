#pragma once
#include "./soundReactiveDmx.h"

namespace clips {
    class schwanensee : public soundReactiveDmx {
    public:
        schwanensee() : soundReactiveDmx() {
            _name = "schwanensee";
            _channel.set("channel", 1, 1, 512);
            _start.set("start", 12, 1, 16);
            _amount.set("amount", 16, 1, 16);
            _minValue.set("minValue", 60, 0, 255);
            _maxValue.set("maxValue", 120, 0, 255);
            _addPeakEnergy.set("addPeakEnergy", false);
            _speed.set("speed", .1, 0, 1);
            _pitchThreshold.set("pitchThreshold", .5, 0, 1);

            _add.set("add");
            _remove.set("remove");
            _active.setName(_name);
            
            _parameters.add(_start);
            _parameters.add(_amount);
            _parameters.add(_minValue);
            _parameters.add(_maxValue);
            _parameters.add(_addPeakEnergy);
            _parameters.add(_speed);
            _parameters.add(_pitchThreshold);
            _parameters.add(_add);
            _parameters.add(_remove);
            
            _values.resize(_amount);
            _timestamps.resize(_amount);
        }
        
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(_timestamp != -1 && timestamp - _timestamp >= 10){
                auto index = (int)(ofMap(_pitch, 40, 72, 0, 15));
                ofLogNotice("schwanensee::setPeakEnergy")<< index;
                if(index < 0 || index > _amount -1){
//                TODO
                    ofLogError()<<"index out of bounds. "<<index;
                    return;
                }
                _values[index] = _maxValue;
                _timestamps[index] = timestamp;
                _timestamp = -1;
            }
            
            for(auto i = 0; i < _amount; i++) {
                if(timestamp - _timestamps[i] < 500){
                    _values[i] = ofMap(timestamp, _timestamps[i], _timestamps[i] + 500, _maxValue, _minValue);
                    std::pair<int, int> value(_channel+i, _values[i]);
                    _valueChangeEvent.notify(value);
                }
            }
        }
        void stop(){
            for(auto i = 1; i <= 16; i++){
                std::pair<int, int> value(i, 0);
                _valueChangeEvent.notify(value);
            }
            base::stop();
        }
        void setPeakEnergy(int analyserId, float value){
            _peakEnergy = value;
            if(value > _pitchThreshold){
                _timestamp = ofGetElapsedTimeMillis();
            }
        }
        void setPitch(int analyserId, float value){
            _pitch = value;
        }
        
        void onAdd(){
            _amount.set(_amount + 1);
        }
        void onRemove(){
            std::pair<int, int> value((_start+_amount-1) % 16 + 1, 0);
            _valueChangeEvent.notify(value);
            _amount = _amount-1;
        }
        ofParameter<int> _channel;
        ofParameter<int> _start;
        ofParameter<int> _amount;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        ofParameter<bool> _addPeakEnergy;
        ofParameter<float> _speed;
        ofParameter<float> _pitchThreshold;
        ofParameter<void> _add;
        ofParameter<void> _remove;
        
        u_int64_t _timestamp;
        std::vector<u_int64_t> _timestamps;

        std::vector<int> _values;
        float _peakEnergy;
        float _pitch;
    };
};
