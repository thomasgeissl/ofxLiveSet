#pragma once
#include "ofxLiveSet.h"
#include "ofxMidi.h"
#include "../../dmx.config.h"

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
            _minValue.set("minValue", 0, 0, 255);

            _active.setName(_name);
            
            // addParameter(_soundAnalyserId);
//            addParameter.add(_minValue);
            addParameter(_pitchThreshold.set("pitchThreshold", .01, 0, 1));
            addParameter(_debounceTime.set("debounceTime", 190, 0, 1000));
            addParameter(_fadeOutTime.set("fadeOutTime", 300, 0, 3000));
            addParameter(_maxValue.set("maxValue", 0, 0, 255));
            addParameter(_addPeakEnergy.set("addPeakEnergy", false));
            addParameter(_staticLight.set("staticLight", false));
            _staticLightIndex.set("staticLightIndex", 20, 1, KLS_LIGHTBULBSCOUNT);
            addParameter(_staticLightValue.set("staticLightValue", 100, 0, 255));
            _meters.setName("meters");
            _meters.add(_pitch.set("pitch", 0, 0, 127));
            _meters.add(_peakEnergy.set("peakEnergy", 0, 0, 5));
            _meters.add(_onsetDetected.set("onset", false));
            _parameters.add(_meters);

            _values.resize(KLS_LIGHTBULBSCOUNT);
            _timestamps.resize(KLS_LIGHTBULBSCOUNT);

            _staticLight.addListener(this, &schwanensee::onStaticLightChange);
        }
        
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(_peakEnergy > _pitchThreshold && !_onsetDetected){
                _timestamp = ofGetElapsedTimeMillis() + _debounceTime;
                _onsetDetected = true;
            }
            if(timestamp > _timestamp && _onsetDetected){
                _onsetDetected = false;
                auto index = (int)(ofMap(_pitch, 40, 72, 0, KLS_LIGHTBULBSCOUNT -1));
                if(index >= 0 && index <= KLS_LIGHTBULBSCOUNT -1 && index != _lastIndex){
                    if(timestamp - _timestamps[index] > 100){
                        _values[index] = _maxValue;
                        _timestamps[index] = timestamp;
                    }
                    _lastIndex = index;
                }
            }
            
            // fade out light bulbs
            for(auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++) {
                if(timestamp - _timestamps[i] < _fadeOutTime){
                    _values[i] = ofMap(timestamp, _timestamps[i], _timestamps[i] + _fadeOutTime, _maxValue, _minValue);
                    setDmxValue(i+1, _values[i]);
                }
            }
            if(_staticLight){
                setDmxValue(_staticLightIndex, _staticLightValue);
            }
        }
        void stop(){
            for(auto i = 1; i <= KLS_LIGHTBULBSCOUNT; i++){
                setDmxValue(i, 0);
            }
            base::stop();
        }
        void setPeakEnergy(int analyserId, float value){
            if(analyserId != _soundAnalyserId){return;}
            _peakEnergy = value;
        }
        void setPitch(int analyserId, float value){
            if(analyserId != _soundAnalyserId){return;}
            _pitch = ofxMidi::ftom(value);
        }
        void onStaticLightChange(bool & value){
            if(!value){
                setDmxValue(5, 0);
            }
        }
        
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        ofParameter<int> _fadeOutTime;
        ofParameter<int> _debounceTime;
        ofParameter<bool> _addPeakEnergy;
        ofParameter<float> _pitchThreshold;
        ofParameter<int> _staticLightIndex;
        ofParameter<bool> _staticLight;
        ofParameter<int> _staticLightValue;
        ofParameterGroup _meters;
        ofParameter<float> _pitch;
        ofParameter<float> _peakEnergy;
        ofParameter<bool> _onsetDetected;

        u_int64_t _timestamp;
        std::vector<u_int64_t> _timestamps;

        std::vector<int> _values;
        int _lastIndex;
    };
};
