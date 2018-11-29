#pragma once
#include "clips/graphic.h"
#include "../beatReactive.h"

namespace clips {
    class cubeWithTrails : public ofxLiveSet::clip::graphic, public clips::beatReactive {
    public:
        cubeWithTrails(std::string name = "cubeWithTrails") : graphic(name){
            _name = name;
            _active.setName(_name);
            _beatReactive.set("beatReactive", false);
            _primaryColor.set("primaryColor", ofColor::white);
            _amount.set("amount", 5, 1, 50);
            _size.set("size", 200, 10, 500);
            _parameters.add(_beatReactive);
            _parameters.add(_primaryColor);
            _parameters.add(_amount);
            _parameters.add(_size);

        }
        void update(){
            ofEnableAlphaBlending();
            begin(false);
            ofFill();
            ofSetColor(255,255,255, _amount);
            ofDrawRectangle(-_width/2, -_height/2, _width, _height);

	        ofNoFill();
	        ofSetColor(_primaryColor);

            ofRotateDeg(ofGetElapsedTimef()*30, 1,0,0.5);
            if(_beatReactive){
                ofDrawBox(0,0,0,_size*(1+_snare));
            }else{
                ofDrawBox(0,0,0,_size);
            }

            end();
            setNewFrame();
        }

        void setKick(float value){
            _kick = value;
        }
        void setSnare(float value){
            _snare = value;
        }
        void setHihat(float value){
            _hihat = value;
        }

        float _kick;
        float _snare;
        float _hihat;

        ofParameter<bool> _beatReactive;
        ofParameter<ofColor> _primaryColor;
        ofParameter<int> _amount;
        ofParameter<int> _size;
    };
};