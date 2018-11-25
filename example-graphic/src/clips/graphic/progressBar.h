#pragma once
#include "clips/graphic.h"
namespace clips {
    class progressBar : public ofxLiveSet::clip::graphic {
    public:
        progressBar() : graphic(){
            _name = "progressBar";
            _active.setName(_name);

            _primaryColor.set("primaryColor", ofColor::red);
            _backgroundColor.set("backgroundColor", ofColor::black);

            _parameters.add(_primaryColor);
            _parameters.add(_backgroundColor);
        }
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            auto x = _width/4;
            auto height = 50;
            auto y = (_height - height)/2;
            auto width = ofMap(std::abs(std::sin(ofGetElapsedTimef())), 0, 1, 0, _width/2);
            _fbo.begin();
            ofClear(_backgroundColor);
            ofSetColor(_primaryColor);

            ofDrawRectangle(x, y, width, height);
            _fbo.end();
            setNewFrame();
        }

        ofParameter<ofColor> _primaryColor;
        ofParameter<ofColor> _backgroundColor;
    };
};