#pragma once
#include "clips/graphic.h"
namespace clips {
    class randomPolygon : public ofxLiveSet::clip::graphic {
    public:
        typedef std::shared_ptr<randomPolygon> pointer;
        static pointer create(std::string name="")
        {
            return std::make_shared<randomPolygon>(name);
        }
        
        randomPolygon(std::string name = "randomPolygon") : graphic(name){
            _name = name;
            _active.setName(_name);
            
            _primaryColor.set("primaryColor", ofColor::white);
            _amount.set("amount", 5, 1, 50);

            _parameters.add(_primaryColor);
            _parameters.add(_amount);
            _parameters.add(_shaderEnabled);
            _shaderEnabled = true;

            _shader.load("bezierVertex/shaders/noise.vert", "bezierVertex/shaders/noise.frag");
        }
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(timestamp - _timestamp < 500){
                return;
            }
            _timestamp = timestamp;
            begin();
            ofEnableAlphaBlending();
            ofSetColor(_primaryColor);
            ofSetPolyMode(OF_POLY_WINDING_ODD);
            ofBeginShape();
            for (int i = 0; i < _amount; i++){
                ofVertex(ofRandom(-_width/2,_width/2), ofRandom(-_height/2, _height/2));
            }
            ofEndShape();
            end();
            setNewFrame();
        }

        ofParameter<ofColor> _primaryColor;
        ofParameter<int> _amount;
        u_int64_t _timestamp;
    };
};