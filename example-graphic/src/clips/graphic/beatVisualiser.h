#pragma once
#include "clips/graphic.h"
#include "../beatReactive.h"

namespace clips {
    class beatVisualiser : public ofxLiveSet::clip::graphic, public clips::beatReactive {
    public:
        beatVisualiser(std::string name = "beatVisualiser") : graphic(name){
            _name = name;
            _active.setName(_name);
            
            _primaryColor.set("primaryColor", ofColor::white);
            _parameters.add(_primaryColor);
        }
        void update(){
            ofEnableAlphaBlending();
            beginFboWithShaderIfActive();
            ofClear(255,0);
            if(_kick > .5){
                ofSetColor(ofColor(255,255,255, _kick*255));
                ofDrawRectangle(0, _height - 100, 100, 100);
            }

            if(_snare > .5){
                ofSetColor(ofColor(255,255,255, _snare*255));
                ofDrawRectangle(100, _height - 150, 50, 50);
            }
            if(_hihat > .5){
                ofSetColor(ofColor(255,255,255, _hihat*255));
                ofDrawRectangle(150, _height - 200, 50, 50);
            }

            endFboWithShaderIfActive();
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

        ofParameter<ofColor> _primaryColor;
        float _kick;
        float _snare;
        float _hihat;
    };
};