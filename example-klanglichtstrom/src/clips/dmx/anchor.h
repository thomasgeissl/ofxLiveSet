#pragma once
#include "./soundReactiveDmx.h"

namespace clips {
    class anchor : public soundReactiveDmx {
    public:
        anchor() : soundReactiveDmx() {
            _name = "anchor";
            _beatsSoundAnalyserId.set("beatsAnalyserId", 2, 0, 32);
            _channel.set("channel", 1, 1, 512);
            _start.set("start", 12, 1, 16);
            _amount.set("amount", 1, 1, 16);
            _minValue.set("minValue", 0, 0, 255);
            _maxValue.set("maxValue", 115, 0, 255); // beatMaxValue
            _addPeakEnergy.set("addPeakEnergy", false);
            _breathe.set("breathe", false);
            _speed.set("speed", .7, 0, 1);
       
            _active.setName(_name);
            
            _beatsQuadrant.set("beatsQuadrant", 0, 0, 3);
            _beatsEnergyThreshold.set("beatsEnergyThreshold", 0.2, 0, 1);

            _highs.set("highs", true);
            _highsQuadrant.set("highsQuadrant", 1, 0, 3);
            _highsMaxValue.set("highsMaxValue", 0, 0, 255);

            _randomiseHighsQuadrant.set("randomiseHighsQuadrant");
            _randomiseHighsQuadrant.addListener(this, &anchor::onRandomiseHighsQuadrant);

            _parameters.add(_beatsSoundAnalyserId);
            _parameters.add(_start);
            _parameters.add(_amount);
//            _parameters.add(_minValue);
            _parameters.add(_maxValue);
            _parameters.add(_addPeakEnergy);
            _parameters.add(_speed);
            
            _parameters.add(_breathe);
            _parameters.add(_beatsQuadrant);
            _parameters.add(_beatsEnergyThreshold);
            _parameters.add(_highs);
            _parameters.add(_highsQuadrant);
            _parameters.add(_randomiseHighsQuadrant);
            _parameters.add(_highsMaxValue);

            _values.resize(16);
            _timestamps.resize(16);
        }
        
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();


            // highs
            if(_highs){
                auto channel = _highsQuadrant * 4 + (int)(ofRandom(0, 4));
                _values[channel] = ofRandom(0, _highsMaxValue);
            }
 
      
            // beat
            auto dmxValue = 0;
            if(_breathe){
                if(_addPeakEnergy){
                    _values[_beatsQuadrant*4 + 3] = ofMap(std::abs(std::sin(ofGetElapsedTimef()*10*0.1+0.4)) + _beatsPeakEnergy, 0, 2, _minValue, _maxValue);
                    
                }else{
                    _values[_beatsQuadrant*4 + 3] = ofMap(std::abs(std::sin(ofGetElapsedTimef()*10*0.1+0.4)), 0, 1, _minValue, _maxValue);
                }
            }else{
                for(auto i = _beatsQuadrant*4; i < _beatsQuadrant*4+4; i++){
                    if(timestamp - _timestamps[i] < 100){
                        _values[i] = ofMap(timestamp, _timestamps[i], _timestamps[i] + 100, _maxValue, _minValue);
                    }
                }
            }
            
            ofLogNotice() << _peakEnergy;
            if(_peakEnergy > .2 && timestamp - _highsQuadrantChangeTimestamp > 100){
                onRandomiseHighsQuadrant();
            }

            
            for(auto i = 0; i < 16; i++){
                std::pair<int, int> value(i+1, _values[i]);
                _valueChangeEvent.notify(value);
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
            if(analyserId == _soundAnalyserId){
                _peakEnergy = value;
            }
            if(analyserId == _beatsSoundAnalyserId){
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
        
                    _beatsPeakEnergy = value;
                }
            }
        }
        
        void onRandomiseHighsQuadrant(){
            _highsQuadrantChangeTimestamp = ofGetElapsedTimeMillis();
            ofLogNotice() << "randomise";
            int oldValue = _highsQuadrant;
            
            _highsQuadrant = ofRandom(0, 4-.00001);
            while(_highsQuadrant == _beatsQuadrant || _highsQuadrant == oldValue){
                _highsQuadrant = ofRandom(0, 4-.00001);
            }
            
            auto channel = oldValue * 4;
            for(auto i = 0; i < 4; i++){
                _values[channel+i] = 0;
            }
        }
        ofParameter<int> _beatsSoundAnalyserId;
        ofParameter<int> _channel;
        ofParameter<int> _start;
        ofParameter<int> _amount;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        ofParameter<bool> _addPeakEnergy;
        ofParameter<float> _speed;
        ofParameter<int> _beatsQuadrant;
        
        ofParameter<bool> _breathe;

        ofParameter<float> _peakEnergyThreshold;
        ofParameter<float> _beatsEnergyThreshold;
        
        ofParameter<bool> _highs;
        ofParameter<int> _highsQuadrant;
        ofParameter<int> _highsMaxValue;

        ofParameter<void> _randomiseHighsQuadrant;

        float _peakEnergy;
        float _beatsPeakEnergy;
        
        std::vector<int> _values;
        std::vector<u_int64_t> _timestamps;
        u_int64_t _highsQuadrantChangeTimestamp;
    };
};
