#pragma once
#include "clips/graphic.h"
namespace clips {
    class background : public ofxLiveSet::clip::graphic {
    public:
        background() : graphic(){
            _name = "background";
            _active.setName(_name);
            _primaryColor.set("primaryColor", ofColor::purple);
            _parameters.add(_primaryColor);
        }
        void update(){
            _fbo.begin();
            ofClear(_primaryColor);
            _fbo.end();
        }

        ofParameter<ofColor> _primaryColor;
    };
};