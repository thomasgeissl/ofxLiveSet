#pragma once
#include "ofxLiveSet.h"
#include "ofxMovingAverage.h"

namespace clips {
    class wind : public ofxLiveSet::clip::dmx, public ofxLiveSet::clip::soundReactive{
    public:
        typedef std::shared_ptr<wind> pointer;
        static pointer create()
        {
            return std::make_shared<wind>();
        }
        wind() : ofxLiveSet::clip::dmx(), ofxLiveSet::clip::soundReactive(), 
            _leftPeakEnergyMovingAverage(ofxMovingAverage<float>(100)),
            _rightPeakEnergyMovingAverage(ofxMovingAverage<float>(100))
            {
            _name = "wind";
            _active.setName(_name);
            addParameter(_soundAnalyserId);
            addParameter(_rightSoundAnalyserId.set("right analyser", 2, 0, 32));
            addParameter(_static.set("static", false));
            addParameter(_minValueChimes.set("minValueChimes", 0, 0, 255));
            addParameter(_maxValueChimes.set("maxValueChimes", 0, 0, 255));
            addParameter(_minValue.set("minValue", 0, 0, 255));
            addParameter(_maxValue.set("maxValue", 255, 0, 255));
            addParameter(_speedChimes.set("speedChimes", .1, 0, 1));
            addParameter(_innerLeftLight.set("innerLeftLight", 10, 0, 16));
            addParameter(_middleLeftLight.set("middleLeftLight", 0, 0, 16));
            addParameter(_outerLeftLight.set("outerLeftLight", 11, 0, 16));
            addParameter(_innerRightLight.set("innerRightLight", 7, 0, 16));
            addParameter(_middleRightLight.set("middleRightLight", 0, 0, 16));
            addParameter(_outerRightLight.set("outerRightLight", 6, 0, 16));
            addParameter(_debounceTime.set("debounceTime", 150, 0, 1000));
            addParameter(_threshold.set("threshold", 0.8, 0, 1));
            addParameter(_peakFactor.set("peakFactor", 1.3, 1, 2));
            addParameter(_pingPong.set("pingPong", false));
            addParameter(_pingPongTime.set("pingPongTime", 500, 0, 1000));
            _meters.setName("meters");
            _meters.add(_leftPeakEnergy.set("leftPeakEnergy", 0, 0, 5));
            _meters.add(_rightPeakEnergy.set("rightPeakEnergy", 0, 0, 5));
            _parameters.add(_meters);
        }
        
        void update(){
            if(_static){
                auto timestamp = ofGetElapsedTimeMillis();
                for(auto i = 0; i < 16; i++){
                    auto value = ofMap(timestamp - _timestamps[i], 0, 500, _maxValue, _minValue, true);
                    _values[i] = value;
                    setValue(i + 1, value);
                }
                setValue(_chimesChannel, 255);
                setValue(1, ofMap(_leftPeakEnergyMovingAverage.avg() * 4, 0, 2, _minValue, _maxValue));
                setValue(16, ofMap(_rightPeakEnergyMovingAverage.avg() * 4, 0, 2, _minValue, _maxValue));
                if(_leftPeakEnergy > _leftPeakEnergyMovingAverage.avg() * 1.5){
                    peak(true);
                }
                if(_rightPeakEnergy > _rightPeakEnergyMovingAverage.avg() * 1.5){
                    peak(false);
                }
                if(_pingPong){
                    if(timestamp - _pingPongTimestamp > _pingPongTime){
                        if(_pingPongLeft){
                            _values[_outerLeftLight -1] = _maxValue;
                            _timestamps[_outerLeftLight -1] = timestamp;
                        }else{
                            _values[_outerRightLight -1] = _maxValue;
                            _timestamps[_outerRightLight -1] = timestamp;
                        }
                        _pingPongTimestamp = timestamp;
                        _pingPongLeft = !_pingPongLeft;
                    }
                }
            }else{
                setValue(_chimesChannel, ofMap(std::abs(std::sin(ofGetElapsedTimef()*10*_speedChimes)), 0, 1, _minValueChimes, _maxValueChimes));
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
            auto timestamp = ofGetElapsedTimeMillis();
            if(analyserId == _soundAnalyserId){
                _leftPeakEnergy = value;
                _leftPeakEnergyMovingAverage.add(value);
            }
            if(analyserId == _rightSoundAnalyserId){
                _rightPeakEnergy = value;
                _rightPeakEnergyMovingAverage.add(value);
            }
        }
        void peak(bool left){
            auto timestamp = ofGetElapsedTimeMillis();
            auto oldTimestamp = left ? _leftTimestamp : _rightTimestamp;
            if(timestamp - oldTimestamp  > _debounceTime){
                if(left){
                    _leftTimestamp = timestamp;
                    _values[_innerLeftLight - 1] = _maxValue;
                    _timestamps[_innerLeftLight -1] = timestamp;
                }else{
                    _rightTimestamp = timestamp;
                    _values[_innerRightLight -1] = _maxValue;
                    _timestamps[_innerRightLight-1] = timestamp;
                }
            }

        }

        ofParameter<int> _rightSoundAnalyserId;
        ofParameter<float> _speedChimes;
        ofParameter<int> _minValueChimes;
        ofParameter<int> _maxValueChimes;
        ofParameter<bool> _static;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;

        ofParameter<int> _innerLeftLight;
        ofParameter<int> _middleLeftLight;
        ofParameter<int> _outerLeftLight;
        ofParameter<int> _innerRightLight;
        ofParameter<int> _middleRightLight;
        ofParameter<int> _outerRightLight;
        ofParameter<float> _threshold;
        ofParameter<int> _debounceTime;
        ofParameter<float> _peakFactor;
        ofParameter<bool> _pingPong;
        ofParameter<int> _pingPongTime;

        ofParameterGroup _meters;
        ofParameter<float> _leftPeakEnergy;
        ofParameter<float> _rightPeakEnergy;
        ofxMovingAverage<float> _leftPeakEnergyMovingAverage;
        ofxMovingAverage<float> _rightPeakEnergyMovingAverage;

        int _chimesChannel = 17; //TODO: change

        // float _leftPeakEnergy;
        // float _rightPeakEnergy;
        u_int64_t _leftTimestamp;
        u_int64_t _rightTimestamp;
        u_int64_t _pingPongTimestamp;
        bool _pingPongLeft;
        int _values[16];
        u_int64_t _timestamps[16];
    };
};
