#pragma once
#include "./soundReactiveDmx.h"

namespace clips {
    class schwanensee : public soundReactiveDmx {
    public:
        schwanensee() : soundReactiveDmx() {
            _name = "schwanensee";
            _start.set("start", 12, 1, 16);
            _amount.set("amount", 16, 1, 16);
            _minValue.set("minValue", 0, 0, 255);
            _maxValue.set("maxValue", 120, 0, 255);
            _fadeOutTime.set("fadeOutTime", 300, 0, 3000);

            _addPeakEnergy.set("addPeakEnergy", false);
            _pitchThreshold.set("pitchThreshold", .03, 0, 1);

            _active.setName(_name);
            
            _parameters.add(_start);
//            _parameters.add(_minValue);
            _parameters.add(_maxValue);
            _parameters.add(_fadeOutTime);
            _parameters.add(_addPeakEnergy);
            _parameters.add(_pitchThreshold);

            _values.resize(_amount);
            _timestamps.resize(_amount);
        }
        
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(_pitch2 != -1 && _timestamp != -1 && timestamp - _timestamp >= 16){
                auto index = (int)(ofMap(_pitch, 40, 72, 0, 15));
                if(index >= 0 && index <= _amount -1 && index != _lastIndex){
                    if(timestamp - _timestamps[index] > 100){
                        _values[index] = _maxValue;
                        _timestamps[index] = timestamp;
                    }
                    _timestamp = -1;
                    _pitch2 = -1;
                    _lastIndex = index;
                }else{
//                    ofLogError()<<"index out of bounds. "<<index;
                }
 
            }
            
            for(auto i = 0; i < _amount; i++) {
                if(timestamp - _timestamps[i] < _fadeOutTime){
                    _values[i] = ofMap(timestamp, _timestamps[i], _timestamps[i] + _fadeOutTime, _maxValue, _minValue);
                    std::pair<int, int> value(i+1, _values[i]);
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
            if(analyserId != _soundAnalyserId){return;}
            if(value > _pitchThreshold){
                ofLogNotice() << "value > threshold "<<value;
                _peakEnergy = value;
                _pitch2 = _pitch;
                _timestamp = ofGetElapsedTimeMillis();
            }
        }
        void setPitch(int analyserId, float value){
            if(analyserId != _soundAnalyserId){return;}
            _pitch = value;
        }
        
        ofParameter<int> _start;
        ofParameter<int> _amount;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        ofParameter<int> _fadeOutTime;
        ofParameter<bool> _addPeakEnergy;
        ofParameter<float> _pitchThreshold;
        
        u_int64_t _timestamp;
        std::vector<u_int64_t> _timestamps;

        std::vector<int> _values;
        float _peakEnergy;
        float _pitch;
        float _pitch2;
        int _lastIndex;

    };
};
