#pragma once
#include "clips/graphic.h"
namespace clips {
    class parametric2dEquation : public ofxLiveSet::clip::graphic {
    public:
        parametric2dEquation() : graphic(){
            _name = "parametric2dEquation";
            _active.setName(_name);
            
            _primaryColor.set("primaryColor", ofColor::white);
            _amount.set("amount", 15, 1, 100);
            _speed.set("speed", 0.5, 0, 1);

            _parameters.add(_primaryColor);
            _parameters.add(_shaderEnabled);
            _parameters.add(_amount);
            _parameters.add(_speed);
            _x0Frequency.set("x0Frequency", 15, 1, 100);
            _x1Frequency.set("x1Frequency", 10, 1, 100);

            _x0Amplitude.set("x0Amplitude", 100, 1, 200);
            _x1Amplitude.set("x1Amplitude", 50, 1, 200);

            _y0Frequency.set("y0Frequency", 15, 1, 100);
            _y1Frequency.set("y1Frequency", 10, 1, 100);

            _y0Amplitude.set("y0Amplitude", 100, 1, 200);
            _y1Amplitude.set("y1Amplitude", 50, 1, 200);

            _lineWidth.set("lineWidth", 2, 1, 10);

            _parameters.add(_x0Frequency);
            _parameters.add(_x0Amplitude); 
            _parameters.add(_y0Frequency);
            _parameters.add(_y0Amplitude);
            _parameters.add(_x1Frequency);
            _parameters.add(_x1Amplitude);
            _parameters.add(_y1Frequency);
            _parameters.add(_y1Amplitude);
            _parameters.add(_lineWidth);

            _shaderEnabled = false;

            _shader.load("bezierVertex/shaders/noise.vert", "bezierVertex/shaders/noise.frag");
        }
        void update(){
            ofEnableAlphaBlending();
            beginFboWithShaderIfActive();
            ofClear(255,0);
            ofSetColor(_primaryColor);
            ofSetLineWidth(_lineWidth);
            ofPushMatrix();
                ofTranslate(_fbo.getWidth()/2,_fbo.getHeight()/2,0);
                for(auto i = 0; i < _amount; i++){
                    ofDrawLine(x0(_t+i), y0(_t+i), x1(_t+i), y1(_t+i));
                }
            ofPopMatrix();

            endFboWithShaderIfActive();
            _t += _speed * 2;
            setNewFrame();
        }

        float x0(float t){
            return std::sin(t / _x0Frequency) * _x0Amplitude;
        }
        float y0(float t){
            return std::sin(t/_y0Frequency) * _y0Amplitude;
        }

        float x1(float t){
            return std::sin(t / _x1Frequency) * _x1Amplitude;
        }
        float y1(float t){
            return std::sin(t/_y1Frequency) * _y1Amplitude;
        }

        ofParameter<ofColor> _primaryColor;
        ofParameter<int> _amount;
        ofParameter<float> _speed;
        ofParameter<float> _x0Frequency;
        ofParameter<float> _x0Amplitude;
        ofParameter<float> _y0Frequency;
        ofParameter<float> _y0Amplitude;
        ofParameter<float> _x1Frequency;
        ofParameter<float> _x1Amplitude;
        ofParameter<float> _y1Frequency;
        ofParameter<float> _y1Amplitude;
        ofParameter<int> _lineWidth;

        float _t;
    };
};