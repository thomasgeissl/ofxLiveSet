#pragma once
#include "clips/graphic.h"
namespace clips {
    class randomRectangles : public ofxLiveSet::clip::graphic {
    public:
        randomRectangles() : graphic(){
            _name = "randomRects";
            _active.setName(_name);
            
            _primaryColor.set("primaryColor", ofColor::red);
            _amount.set("amount", 4, 1, 64);
            _frequency.set("frequency", 500, 0, 5000);

            _parameters.add(_primaryColor);
            _parameters.add(_amount);
            _parameters.add(_frequency);

        }
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(timestamp < _timestamp + _frequency) return;

            _fbo.begin();
            ofClear(255,255,255,0);
            ofSetColor(_primaryColor);
            auto width = ofRandom(0, _width/10);
            auto height = ofRandom(0, _height/10);
            for(auto i = 0; i < _amount; i++){

                auto x = ofRandom(0, _width - width);
                auto y = ofRandom(0, _height - height);

                ofDrawRectangle(x, y, width, height);
            }
            _fbo.end();
            _timestamp = timestamp;
            _newFrame = true;
        }

        ofParameter<ofColor> _primaryColor;
        ofParameter<int> _amount;
        ofParameter<int> _frequency;

        u_int64_t _timestamp;


    };
};