#pragma once
#include "clips/graphic.h"
namespace clips {
    class arcs : public ofxLiveSet::clip::graphic {
    public:
        arcs(std::string name = "arcs") : graphic(name){
            _name = name;
            _active.setName(_name);
            
            _primaryColor.set("primaryColor", ofColor::white);
            _amount.addListener(this, &arcs::onAmountChange);
            _amount.set("amount", 5, 1, 50);
            _radius.set("radius", 50, 0, 100);
            _lineWidth.set("lineWidth", 30, 10, 100);

            _parameters.add(_primaryColor);
            _parameters.add(_amount);
            _parameters.add(_radius);
            _parameters.add(_lineWidth);
            _parameters.add(_shaderEnabled);
            _shaderEnabled = false;

            _shader.load("bezierVertex/shaders/noise.vert", "bezierVertex/shaders/noise.frag");
        }
        void update(){
            begin();
            ofSetLineWidth(_lineWidth);
            for(auto i = 0; i < _amount; i++){
                ofSetColor(_primaryColor);
                ofPolyline polyline;
                int start = (int)((ofGetElapsedTimef() + _timeOffsets[i])*_speeds[i])  % 360;

                polyline.arc(0, 0, (i+1) * _radius, (i+1) * _radius, (start + 100) % 360, (start + 350) % 360, 60);
                polyline.draw();
            }
            end();
            setNewFrame();
        }
        void onAmountChange(int &value){
            _timeOffsets.resize(value);
            _speeds.resize(value);
            for(auto i = 0; i < _timeOffsets.size(); i++){
                _timeOffsets[i] = ofRandom(1);
                _speeds[i] = 100 + ofRandom(-25, 25);
            }
        }

        ofParameter<ofColor> _primaryColor;
        ofParameter<int> _amount;
        ofParameter<int> _radius;
        ofParameter<int> _lineWidth;
        std::vector<float> _timeOffsets;
        std::vector<float> _speeds;
    };
};