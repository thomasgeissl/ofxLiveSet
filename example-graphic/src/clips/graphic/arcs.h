#pragma once
#include "clips/graphic.h"
namespace clips {
    class arcs : public ofxLiveSet::clip::graphic {
    public:
        arcs(std::string name = "arcs") : graphic(name){
            _name = name;
            _active.setName(_name);
            
            _primaryColor.set("primaryColor", ofColor::white);
            _amount.set("amount", 5, 1, 50);

            _parameters.add(_primaryColor);
            _parameters.add(_amount);
            _parameters.add(_shaderEnabled);
            _shaderEnabled = false;

            _shader.load("bezierVertex/shaders/noise.vert", "bezierVertex/shaders/noise.frag");
        }
        void update(){
            begin();
            auto radius = 20;
            ofSetLineWidth(20);
            for(auto i = 0; i < _amount; i++){
                ofSetColor(_primaryColor);
                ofPolyline polyline;
                int start = ofMap(std::abs(std::sin(ofGetElapsedTimef())), 0, 1, 0, 359);

                polyline.arc(0, 0, (i+1) * 20, (i+1) * 20, (start + 100) % 360, (start + 350) % 360, 60);
                polyline.draw();
            }
            end();
            setNewFrame();
        }

        ofParameter<ofColor> _primaryColor;
        ofParameter<int> _amount;
    };
};