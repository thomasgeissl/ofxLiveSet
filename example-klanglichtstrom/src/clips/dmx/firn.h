#pragma once
#include "ofxLiveSet.h"
#include "../../dmx.config.h"

namespace clips {
    class firn : public ofxLiveSet::clip::dmx, public ofxLiveSet::clip::soundReactive{
    public:
        typedef std::shared_ptr<firn> pointer;
        static pointer create()
        {
            return std::make_shared<firn>();
        }
        firn() : ofxLiveSet::clip::dmx(), ofxLiveSet::clip::soundReactive(), _lastIndex(0){
            _name = "firn";
            
            _active.setName(_name);
            
            addParameter(_soundAnalyserId);
            addParameter(_minValue.set("minValue", 0, 0, 255));
            addParameter(_maxValue.set("maxValue", 100, 0, 255));
            addParameter(_threshold.set("threshold", .1, 0, 1));
            addParameter(_peakEnergyDebounceTime.set("debounce", 300, 30, 500));
            addParameter(_minDistance.set("minDistance", 12, 1, KLS_LIGHTBULBSCOUNT));
            
            _meters.setName("meters");
            _meters.add(_peakEnergy.set("peakEnergy", 0, 0, 5));
            _parameters.add(_meters);
            
            _timestamp = ofGetElapsedTimeMillis();

            _values.resize(KLS_LIGHTBULBSCOUNT);
            _timestamps.resize(KLS_LIGHTBULBSCOUNT);
        }
        
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            
            for(auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++) {
                if(timestamp - _timestamps[i] < 1000){
                    _values[i] = ofMap(timestamp, _timestamps[i], _timestamps[i] + 1000, _maxValue, _minValue);
                    setValue(i+1, _values[i]);
                }
            }
        }
        void stop(){
            for(auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++){
                setValue(i+1, 0);
            }
            base::stop();
        }
        void setPeakEnergy(int analyserId, float value) {
            if(analyserId != _soundAnalyserId){return;}
            _peakEnergy = value;
            if(value < _threshold){return;}
            auto timestamp = ofGetElapsedTimeMillis();
            if(timestamp - _timestamp < _peakEnergyDebounceTime){
                return;
            }
            _timestamp = timestamp;
            
            auto i = ofRandom(KLS_LIGHTBULBSCOUNT);
            while(abs(_lastIndex - i) < _minDistance){
                i = ofRandom(KLS_LIGHTBULBSCOUNT);
            }
            _lastIndex = i;
            _timestamps[i] = ofGetElapsedTimeMillis();
            _values[i] = _maxValue*value;
        }
        
        
        ofParameter<int> _value;
        std::vector<int> _values;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        
        ofParameter<float> _threshold;
        ofParameter<int> _peakEnergyDebounceTime;
        ofParameter<int> _minDistance;

        ofParameterGroup _meters;
        ofParameter<float> _peakEnergy;
        
        u_int64_t _timestamp;
        std::vector<u_int64_t> _timestamps;
        int _lastIndex;

        
    };
};

