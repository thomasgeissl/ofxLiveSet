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

            _rightSoundAnalyserId.set("right analyser", 2, 0, 32);
            _static.set("static", false);
            _minValueChimes.set("minValueChimes", 0, 0, 255);
            _maxValueChimes.set("maxValueChimes", 0, 0, 255);
            _speedChimes.set("speedChimes", .1, 0, 1);
            addParameter(_soundAnalyserId);
            addParameter(_rightSoundAnalyserId);
            addParameter(_minValueChimes);
            addParameter(_maxValueChimes);
            addParameter(_speedChimes);
            addParameter(_static);
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
            if(analyserId == _soundAnalyserId){
                _leftPeakEnergy = value;
            }else if(analyserId == _rightSoundAnalyserId){
                _rightPeakEnergy = value;
            }
        }

        ofParameter<int> _rightSoundAnalyserId;
        ofParameter<float> _speedChimes;
        ofParameter<int> _minValueChimes;
        ofParameter<int> _maxValueChimes;
        ofParameter<bool> _static;
        
        float _leftPeakEnergy;
        float _rightPeakEnergy;
        int _chimesChannel = 5; //TODO: change
    };
};
