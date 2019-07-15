#pragma once
#include "ofxLiveSet.h"

namespace clips {
    class wind : public ofxLiveSet::clip::dmx, public ofxLiveSet::clip::soundReactive{
    public:
        typedef std::shared_ptr<wind> pointer;
        static pointer create()
        {
            return std::make_shared<wind>();
        }
        wind() : ofxLiveSet::clip::dmx(), ofxLiveSet::clip::soundReactive() {
            _name = "wind";
            _active.setName(_name);
            addParameter(_soundAnalyserId);
            addParameter(_rightSoundAnalyserId.set("right analyser", 2, 0, 32));
            addParameter(_minValueChimes.set("minValueChimes", 0, 0, 255));
            addParameter(_maxValueChimes.set("maxValueChimes", 0, 0, 255));
            addParameter(_speedChimes.set("speedChimes", .1, 0, 1));
            addParameter(_static.set("static", false));
            addParameter(_innerLeftLight.set("innerLeftLight", 10, 0, 16));
            addParameter(_middleLeftLight.set("middleLeftLight", 0, 0, 16));
            addParameter(_outerLeftLight.set("outerLeftLight", 0, 0, 16));
            addParameter(_innerRightLight.set("innerRightLight", 7, 0, 16));
            addParameter(_middleRightLight.set("middleRightLight", 0, 0, 16));
            addParameter(_outerRightLight.set("outerRightLight", 0, 0, 16));
            addParameter(_debounceTime.set("debounceTime", 100, 0, 500));
            addParameter(_threshold.set("threshold", 0.8, 0, 1));
        }
        
        void update(){
            if(_static){
                setValue(_chimesChannel, 255);
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
                if(value > _threshold && timestamp - _leftTimestamp  > _debounceTime){
                    _leftTimestamp = ofGetElapsedTimeMillis();
                    ofLogNotice() << "left beat";
                }
            }
            if(analyserId == _rightSoundAnalyserId){
                _rightPeakEnergy = value;
                if(value > _threshold && timestamp - _rightTimestamp  > _debounceTime){
                    _rightTimestamp = ofGetElapsedTimeMillis();
                    ofLogNotice() << "right beat";
                }
            }
        }
        void peak(bool left){

        }

        ofParameter<int> _rightSoundAnalyserId;
        ofParameter<float> _speedChimes;
        ofParameter<int> _minValueChimes;
        ofParameter<int> _maxValueChimes;
        ofParameter<bool> _static;

        ofParameter<int> _innerLeftLight;
        ofParameter<int> _middleLeftLight;
        ofParameter<int> _outerLeftLight;
        ofParameter<int> _innerRightLight;
        ofParameter<int> _middleRightLight;
        ofParameter<int> _outerRightLight;
        ofParameter<float> _threshold;
        ofParameter<int> _debounceTime;

        int _chimesChannel = 17; //TODO: change

        float _leftPeakEnergy;
        float _rightPeakEnergy;
        u_int64_t _leftTimestamp;
        u_int64_t _rightTimestamp;
    };
};
