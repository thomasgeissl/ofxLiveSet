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

            _beatDetection.setName("beat detection");
            _kickThreshold.set("kick threshold", 0.5, 0, 1);
            _snareThreshold.set("snare threshold", 0.5, 0, 1);
            _hihatThreshold.set("hihat threshold", 0.5, 0, 1);

            _beatDetection.add(_kickThreshold);
            _beatDetection.add(_snareThreshold);
            _beatDetection.add(_hihatThreshold);
            _parameters.add(_beatDetection);

        }
        void update(){
            if(_kick < _kickThreshold && _snare < _snareThreshold && _hihat < _hihatThreshold){
                return;
            }
            begin();
            if(_kick > _kickThreshold){
                ofSetColor(ofColor(255,255,255, _kick*255));
                ofDrawRectangle(-100, -100, 100, 100);
            }

            if(_snare > _snareThreshold){
                ofSetColor(ofColor(255,255,255, _snare*255));
                ofDrawRectangle(0, -100, 50, 50);
            }
            if(_hihat > _hihatThreshold){
                ofSetColor(ofColor(255,255,255, _hihat*255));
                ofDrawRectangle(100, -200, 50, 50);
            }
            setNewFrame();
            end();
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
        ofParameterGroup _beatDetection;
        ofParameter<float> _kickThreshold;
        ofParameter<float> _snareThreshold;
        ofParameter<float> _hihatThreshold;

        float _kick;
        float _snare;
        float _hihat;
    };
};