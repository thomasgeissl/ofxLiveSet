#pragma once
#include "clips/graphic.h"

namespace clips {
    class rects : public ofxLiveSet::clip::graphic {
    public:
        rects(){
            _amount.set("amount", 16,0,32);
            _primaryColor = ofColor::red;
            _timestamp = ofGetElapsedTimeMillis();
        }

        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(timestamp - _timestamp < 100){
                return;
            }
            _timestamp = timestamp;
            _fbo.begin();
            ofClear(0,0,0);

            for(auto i = 0; i < _amount; i++){
                auto width = ofRandom(0, _fbo.getWidth()/10);
                auto height = ofRandom(0, _fbo.getHeight()/10);

                auto x = ofRandom(0, _fbo.getWidth() - width);
                auto y = ofRandom(0, _fbo.getHeight() - height);
                
                ofSetColor(_primaryColor);
                ofDrawRectangle(x, y, width, height);
            }
            _fbo.end();
        }

        ofParameterGroup _parameters;
        ofParameter<int> _amount;
        ofParameter<ofColor> _primaryColor;
        
        u_int64_t _timestamp;
    };
};
