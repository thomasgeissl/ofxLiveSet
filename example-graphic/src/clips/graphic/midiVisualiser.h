#pragma once
#include "clips/graphic.h"
namespace clips {
    class midiVisualiser : public ofxLiveSet::clip::graphic, public ofxLiveSet::clip::midiReactive {
    public:
        midiVisualiser(std::string name = "midiVisualiser") : graphic(name){
            _name = name;
            _active.setName(_name);
            
            _primaryColor.set("primaryColor", ofColor::white);
            _parameters.add(_channel);
            _parameters.add(_primaryColor);
            for(auto i = 0; i < 128; i++){
                _velocities[i] = 0;
            }
        }
        void update(){
            begin();
            ofSetColor(_primaryColor);
            auto length = _width/128;
            for(auto i = 0; i < 128; i++){
                ofDrawRectangle(i*length - _width/2, _height/2 - _height/2, length, _velocities[i]);
            }

            end();
            setNewFrame();
        }
        void setNoteOn(int note, int velocity){
            _velocities[note] = velocity;
        }
        void setNoteOff(int note, int velocity){
            _velocities[note] = 0;
        }
        
        ofParameter<ofColor> _primaryColor;
        int _velocities[128];
    };
};