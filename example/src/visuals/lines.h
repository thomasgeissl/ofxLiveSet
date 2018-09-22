#pragma once
#include "clips/graphic.h"

namespace visuals {
    class lines : public ofxLiveSet::clip::graphic {
    public:
        lines(){
            _amount.set("amount", 32,0,32);
            _primaryColor = ofColor::blue;
            _timestamp = ofGetElapsedTimeMillis();
        }
        
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(timestamp - _timestamp < 1000){
                return;
            }
            _timestamp = timestamp;
            _fbo.begin();
            ofClear(0,0,0);
            
            for(auto i = 0; i < _amount; i++){
                auto width = ofRandom(0, _fbo.getWidth()/10);
                auto height = 5;
                
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
        std::vector<glm::vec2> _positions;
        
        u_int64_t _timestamp;
    };
};
