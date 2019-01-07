#pragma once
#include "clips/graphic.h"
#include "../beatReactive.h"

namespace clips {
    class snake : public ofxLiveSet::clip::graphic {
    public:
        snake(std::string name = "snake") : graphic(name){
            _name = name;
            _active.setName(_name);
            _primaryColor.set("primaryColor", ofColor::white);
            _speed.set("speed", 1, 0, 10);
            _parameters.add(_primaryColor);
            _parameters.add(_speed);

            _position = glm::vec2(0,0);
            _direction = glm::vec2(1,0);
        }
        void update(){
            begin();

            _position += _direction;
            float length = _width / 100;
            ofSetColor(_primaryColor);
            ofDrawRectangle(_position.x * length - length/2, _position.y * length - length/2, length, length);
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

        glm::vec2 _position;
        std::vector<glm::vec2> _tail;
        glm::vec2 _direction;
    };
};