#pragma once
#include "ofxLiveSet.h"
#include "../../dmx.config.h"

namespace clips {
    class anchor : public ofxLiveSet::clip::dmx, public ofxLiveSet::clip::soundReactive {
    public:
        typedef std::shared_ptr<anchor> pointer;
        static pointer create()
        {
            return std::make_shared<anchor>();
        }
        anchor() : ofxLiveSet::clip::dmx(), ofxLiveSet::clip::soundReactive() {
            _name = "anchor";
            _pitchChangeTriggerSoundAnalyserId.set("pitchChangeTrigger", 2, 0, 8);
            _channel.set("channel", 1, 1, 512);
            _minValue.set("minValue", 0, 0, 255);
            _maxValue.set("maxValue", 168, 0, 255); // beatMaxValue
            _addPeakEnergy.set("addPeakEnergy", false);
       
            _active.setName(_name);
            
            _beatsQuadrant.set("beatsQuadrant", 0, 0, 3);
            _beatsEnergyThreshold.set("beatsEnergyThreshold", 0.2, 0, 1);

            _highs.set("highs", true);
            _highsQuadrant.set("highsQuadrant", 1, 0, 3);
            _highsMaxValue.set("highsMaxValue", 0, 0, 255);

            _randomiseHighsQuadrant.set("randomiseHighsQuadrant");
            _randomiseHighsQuadrant.addListener(this, &anchor::onRandomiseHighsQuadrant);

            addParameter(_soundAnalyserId);
            addParameter(_pitchChangeTriggerSoundAnalyserId);
            // addParameter(_minValue);
            addParameter(_maxValue);
            addParameter(_addPeakEnergy);
            
            addParameter(_breathe.set("breathe", false));
            addParameter(_beatsQuadrant);
            addParameter(_beatsEnergyThreshold);
            addParameter(_highs);
            addParameter(_highsQuadrant);
            addParameter(_randomiseHighsQuadrant);
            addParameter(_highsMaxValue);

            _meters.setName("meters");
            _meters.add(_peakEnergy.set("peakEnergy", 0, 0, 5));
            _meters.add(_pitchChangeTriggerPeakEnergy.set("pitchChangeTriggerPeakEnergy", 0, 0, 5));
            _parameters.add(_meters);

            _values.resize(16);
            _timestamps.resize(16);
        }
        
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();


            // highs
            if(_highs){
                auto channel = _highsQuadrant * 6 + (int)(ofRandom(0, 6));
                _values[channel] = ofRandom(0, _highsMaxValue);
            }
 
      
            // beat
            auto dmxValue = 0;
            if(_breathe){
                if(_addPeakEnergy){
                    _values[_beatsQuadrant*4 + 3] = ofMap(std::abs(std::sin(ofGetElapsedTimef()*10*0.1+0.4)) + _peakEnergy, 0, 2, _minValue, _maxValue);
                    
                }else{
                    _values[_beatsQuadrant*4 + 3] = ofMap(std::abs(std::sin(ofGetElapsedTimef()*10*0.1+0.4)), 0, 1, _minValue, _maxValue);
                }
            }else{
                for(auto i = _beatsQuadrant*6; i < _beatsQuadrant*6+6; i++){
                    if(timestamp - _timestamps[i] < 50){
                        _values[i] = ofMap(timestamp, _timestamps[i], _timestamps[i] + 50, _maxValue, _minValue);
                    }
                }
            }
            
            if(_pitchChangeTriggerPeakEnergy > .1 && timestamp - _highsQuadrantChangeTimestamp > 100){
                onRandomiseHighsQuadrant();
            }

            // finally update dmx channels
            for(auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++){
                setValue(i+1, _values[i]);
            }
        }
        void stop(){
            for(auto i = 1; i <= KLS_LIGHTBULBSCOUNT; i++){
                setValue(i+1, 0);
            }
            base::stop();
        }
        void setPeakEnergy(int analyserId, float value){
            if(analyserId == _pitchChangeTriggerSoundAnalyserId){
                if(value > 0.05){
                ofLogNotice() << ofToString(value, 2);
                }
                _pitchChangeTriggerPeakEnergy = value;
            }
            if(analyserId == _soundAnalyserId){
                if(value > _beatsEnergyThreshold){
                    if(_breathe){
                        _breathe = false;
                        _values[_beatsQuadrant*4 + 3] = 0;
                    }else{
                        for(auto i = _beatsQuadrant*4; i < _beatsQuadrant*4+4; i++){
                            _timestamps[i] = ofGetElapsedTimeMillis();
                            _values[i] = _maxValue*value;
                        }
                    }
                    _peakEnergy = value;
                }
            }
        }
        
        void onRandomiseHighsQuadrant(){
            _highsQuadrantChangeTimestamp = ofGetElapsedTimeMillis();
            int oldValue = _highsQuadrant;
            
            _highsQuadrant = ofRandom(0, 4-.00001);
            while(_highsQuadrant == _beatsQuadrant || _highsQuadrant == oldValue){
                _highsQuadrant = ofRandom(0, 4-.00001);
            }
            
            auto channel = oldValue * 6;
            for(auto i = 0; i < 6; i++){
                _values[channel+i] = 0;
            }
        }

        ofParameter<int> _pitchChangeTriggerSoundAnalyserId;
        ofParameter<int> _channel;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        ofParameter<bool> _addPeakEnergy;
        ofParameter<int> _beatsQuadrant;
        
        ofParameter<bool> _breathe;

        ofParameter<float> _peakEnergyThreshold;
        ofParameter<float> _beatsEnergyThreshold;
        
        ofParameter<bool> _highs;
        ofParameter<int> _highsQuadrant;
        ofParameter<int> _highsMaxValue;

        ofParameter<void> _randomiseHighsQuadrant;

        ofParameterGroup _meters;
        ofParameter<float> _peakEnergy;
        ofParameter<float> _pitchChangeTriggerPeakEnergy;
        
        std::vector<int> _values;
        std::vector<u_int64_t> _timestamps;
        u_int64_t _highsQuadrantChangeTimestamp;
    };
};
