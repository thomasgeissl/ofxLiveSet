#pragma once
#include "ofxLiveSet.h"

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
            _channel.set("channel", 1, 1, 512);
            _amount.addListener(this, &firn::onAmountChange);
            _amount.set("amount", 16, 1, 16);
            
            _minValue.set("minValue", 0, 0, 255);
            _maxValue.set("maxValue", 100, 0, 255);
            _threshold.set("threshold", .1, 0, 1);
            _peakEnergyDebounceTime.set("debounce", 300, 30, 500);
            _minDistance.set("minDistance", 7, 1, 12);
            
            _active.setName(_name);
            
            addParameter(_soundAnalyserId);
            addParameter(_minValue);
            addParameter(_maxValue);
            addParameter(_threshold);
            addParameter(_peakEnergyDebounceTime);
            addParameter(_minDistance);
            
            _timestamp = ofGetElapsedTimeMillis();
        }
        
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            
            for(auto i = 0; i < _amount; i++) {
                if(timestamp - _timestamps[i] < 1000){
                    _values[i] = ofMap(timestamp, _timestamps[i], _timestamps[i] + 1000, _maxValue, _minValue);
                    setValue(_channel + i, _values[i]);
                }
            }
        }
        void stop(){
            for(auto i = 1; i <= 16; i++){
                setValue(i, 0);
            }
            base::stop();
        }
        void setPeakEnergy(int analyserId, float value) {
            if(value < _threshold){return;}
            if(analyserId != _soundAnalyserId){return;}
            auto timestamp = ofGetElapsedTimeMillis();
            if(timestamp - _timestamp < _peakEnergyDebounceTime){
                return;
            }
            _timestamp = timestamp;
            
            auto i = ofRandom(_amount);
            while(abs(_lastIndex - i) < _minDistance){
                i = ofRandom(_amount);
            }
            _lastIndex = i;
            _timestamps[i] = ofGetElapsedTimeMillis();
            _values[i] = _maxValue*value;
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
        
        ofParameter<float> _threshold;
        ofParameter<int> _peakEnergyDebounceTime;
        ofParameter<int> _minDistance;
        
        u_int64_t _timestamp;
        std::vector<u_int64_t> _timestamps;
        int _lastIndex;
    };
};

