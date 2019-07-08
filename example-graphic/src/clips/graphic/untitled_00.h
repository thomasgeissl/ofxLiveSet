#pragma once
#include "clips/graphic.h"
#include "../beatReactive.h"

namespace clips {
    class untitled_00 : public ofxLiveSet::clip::graphic {
    public:
        typedef std::shared_ptr<untitled_00> pointer;
        static pointer create(std::string name="untitled_00")
        {
            return std::make_shared<untitled_00>(name);
        }
        untitled_00(std::string name = "untitled_00") : graphic(name){
            _name = name;
            _active.setName(_name);
            _primaryColor.set("primaryColor", ofColor::white);
            _xAmount.addListener(this, &untitled_00::onXAmountOrYAmountChange);
            _yAmount.addListener(this, &untitled_00::onXAmountOrYAmountChange);
            _xAmount.set("xAmount", 5, 1, 50);
            _yAmount.set("yAmount", 5, 1, 50);
            _speed.set("speed", 1, 0, 10);
            _bang.addListener(this, &untitled_00::onBang);
            _bang.set("bang");
            _parameters.add(_primaryColor);
            _parameters.add(_xAmount);
            _parameters.add(_yAmount);
            _parameters.add(_speed);
            _parameters.add(_bang);
        }
        void update(){
            ofEnableAlphaBlending();
            begin();
            ofTranslate(-_width/2,- _height/2, 0);
            float width = _width / _xAmount;
            float height = _height / _yAmount;
            for(auto x = 0; x < _xAmount; x++){
                for(auto y = 0; y < _yAmount; y++){
                    _alphas[y * _xAmount + x] = std::abs(std::sin(ofGetElapsedTimef() * _speed + _timeOffsets[y * _xAmount + x]));
                    auto color = ofColor(_primaryColor->r, _primaryColor->g, _primaryColor->b, _alphas[y * _xAmount + x]*255);
                    ofSetColor(color);
                    if(ofGetElapsedTimeMillis() - _bangTimestamp < 100){
                        ofSetColor(ofColor::black);
                    }
                    ofDrawRectangle(x * width + 1, y * height + 1, width - 2, height -2);
                }       
            }
            end();
            setNewFrame();
        }
        void onXAmountOrYAmountChange(int &amount){
            _alphas.resize(_xAmount*_yAmount);
            _timeOffsets.resize(_xAmount*_yAmount);
            for(auto i = 0; i < _timeOffsets.size(); i++){
                _timeOffsets[i] = ofRandom(1);
            }
        }
        void onBang(){
            _bangTimestamp = ofGetElapsedTimeMillis();
        }

        ofParameter<ofColor> _primaryColor;
        ofParameter<int> _xAmount;
        ofParameter<int> _yAmount;
        ofParameter<float> _speed;
        ofParameter<void> _bang;

        std::vector<float> _alphas;
        std::vector<float> _timeOffsets;
        u_int64_t _bangTimestamp;
    };
};