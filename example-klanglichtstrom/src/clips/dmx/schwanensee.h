#pragma once
#include "ofxLiveSet.h"

namespace clips {
    class schwanensee : public ofxLiveSet::clip::dmx, public ofxLiveSet::clip::soundReactive {
    public:
        typedef std::shared_ptr<schwanensee> pointer;
        static pointer create()
        {
            return std::make_shared<schwanensee>();
        }
        schwanensee() : ofxLiveSet::clip::dmx(), ofxLiveSet::clip::soundReactive() {
            _name = "schwanensee";
            _amount.set("amount", 16, 1, 16);
            _minValue.set("minValue", 0, 0, 255);

            _blackoutDimmer2.addListener(this, &schwanensee::onBlackoutDimmer2);
            _active.setName(_name);
            
            addParameter(_soundAnalyserId);
//            addParameter.add(_minValue);
            addParameter(_maxValue.set("maxValue", 0, 0, 255));
            addParameter(_fadeOutTime.set("fadeOutTime", 300, 0, 3000));
            addParameter(_debounceTime.set("debounceTime", 20, 0, 1000));
            addParameter(_addPeakEnergy.set("addPeakEnergy", false));
            addParameter(_pitchThreshold.set("pitchThreshold", .01, 0, 1));
            addParameter(_blackoutDimmer2.set("blackoutDimmer2"));
            addParameter(_staticLight.set("staticLight", false));
            addParameter(_staticLightValue.set("staticLightValue", 100, 0, 255));
            _values.resize(_amount);
            _timestamps.resize(_amount);
        }
        
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(_peakEnergy > _pitchThreshold && !_onsetDetected){
                _timestamp = ofGetElapsedTimeMillis() + _debounceTime;
                _onsetDetected = true;
            }
            if(timestamp > _timestamp && _onsetDetected){
                _onsetDetected = false;
                auto index = (int)(ofMap(_pitch, 40, 72, 0, _amount -1));
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
                    setValue(i+1, _values[i]);
                }
            }
            if(_staticLight){
                setValue(5, _staticLightValue);
            }
        }
        void stop(){
            for(auto i = 1; i <= 16; i++){
                setValue(i, 0);
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
                setValue(i+1, 0);
            }
        }
        
        ofParameter<int> _amount;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        ofParameter<int> _fadeOutTime;
        ofParameter<int> _debounceTime;
        ofParameter<bool> _addPeakEnergy;
        ofParameter<float> _pitchThreshold;
        ofParameter<void> _blackoutDimmer2;
        ofParameter<bool> _staticLight;
        ofParameter<int> _staticLightValue;

        u_int64_t _timestamp;
        bool _onsetDetected = false;
        std::vector<u_int64_t> _timestamps;

        std::vector<int> _values;
        float _peakEnergy;
        float _pitch;
        int _lastIndex;
    };
};
