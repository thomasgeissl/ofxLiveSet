#pragma once
#include "./soundReactiveDmx.h"

namespace clips {
    class schwanensee : public soundReactiveDmx {
    public:
        schwanensee() : soundReactiveDmx() {
            _name = "schwanensee";
            _amount.set("amount", 16, 1, 16);
            _minValue.set("minValue", 0, 0, 255);
            _maxValue.set("maxValue", 0, 0, 255);
            _fadeOutTime.set("fadeOutTime", 300, 0, 3000);

            _addPeakEnergy.set("addPeakEnergy", false);
            _pitchThreshold.set("pitchThreshold", .01, 0, 1);
            _blackoutDimmer2.set("blackoutDimmer2");
            _blackoutDimmer2.addListener(this, &schwanensee::onBlackoutDimmer2);

            _active.setName(_name);
            
//            _parameters.add(_minValue);
            _parameters.add(_maxValue);
            _parameters.add(_fadeOutTime);
            _parameters.add(_addPeakEnergy);
            _parameters.add(_pitchThreshold);
            _parameters.add(_blackoutDimmer2);

            _values.resize(_amount);
            _timestamps.resize(_amount);
        }
        
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(_peakEnergy > _pitchThreshold && !_onsetDetected){
                _timestamp = ofGetElapsedTimeMillis() + 50;
                _onsetDetected = true;
            }
            if(timestamp > _timestamp && _onsetDetected){
                _onsetDetected = false;
                auto index = (int)(ofMap(_pitch, 40, 72, 0, 15));
                if(index >= 0 && index <= _amount -1 && index != _lastIndex){
                    if(timestamp - _timestamps[index] > 100){
                        _values[index] = _maxValue;
                        _timestamps[index] = timestamp;
                    }
                    _lastIndex = index;
                }
            }
            
            // fade out light bulbs
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
            _peakEnergy = value;
        }
        void setPitch(int analyserId, float value){
            if(analyserId != _soundAnalyserId){return;}
            _pitch = value;
        }
        void onBlackoutDimmer2(){
            for(auto i = 2 * 4; i < _amount; i++) {
                std::pair<int, int> value(i+1, 0);
                _valueChangeEvent.notify(value);
            }
        }
        
        ofParameter<int> _amount;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        ofParameter<int> _fadeOutTime;
        ofParameter<bool> _addPeakEnergy;
        ofParameter<float> _pitchThreshold;
        ofParameter<void> _blackoutDimmer2;

        u_int64_t _timestamp;
        bool _onsetDetected = false;
        std::vector<u_int64_t> _timestamps;

        std::vector<int> _values;
        float _peakEnergy;
        float _pitch;
        int _lastIndex;

    };
};
