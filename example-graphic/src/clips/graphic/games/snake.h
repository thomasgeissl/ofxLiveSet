#pragma once
#include "clips/graphic.h"

namespace clips {
    class snake : public ofxLiveSet::clip::graphic {
    public:
        typedef std::shared_ptr<snake> pointer;
        static pointer create(std::string name="snake")
        {
            return std::make_shared<snake>(name);
        }
        snake(std::string name = "snake") : graphic(name), _foodPosition(glm::vec2(-5, 5)){
            _name = name;
            _active.setName(_name);
            _xAmount.set("xAmount", 75, 20, 100);
            _primaryColor.set("primaryColor", ofColor::white);
            _secondaryColor.set("secondaryColor", ofColor::lightBlue);
            _backgroundColor.set("backgroundColor", ofColor::darkRed);
            _speed.set("speed", 1, 0, 10);

            _left.set("left");
            _up.set("up");
            _right.set("right");
            _down.set("down");

            _left.addListener(this, &snake::onLeft);
            _up.addListener(this, &snake::onUp);
            _right.addListener(this, &snake::onRight);
            _down.addListener(this, &snake::onDown);

            _parameters.add(_xAmount);
            _parameters.add(_primaryColor);
            _parameters.add(_secondaryColor);
            _parameters.add(_backgroundColor);
            _parameters.add(_speed);

            _parameters.add(_left);
            _parameters.add(_up);
            _parameters.add(_right);
            _parameters.add(_down);

            _position = glm::vec2(0,0);
            _direction = glm::vec2(1,0);
            _tail.push_front(_position-glm::vec2(1,0));
            _tail.push_front(_position-glm::vec2(2,0));
            _tail.push_front(_position-glm::vec2(3,0));
            _tail.push_front(_position-glm::vec2(4,0));
            _tail.push_front(_position-glm::vec2(5,0));
            _tail.push_front(_position-glm::vec2(6,0));
        }
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(timestamp - _timestamp < 500){
                return;
            }
            _timestamp = timestamp;
            begin();

            _tail.push_back(_position);
            _position += _direction;
            auto lastElement = _tail.front();
            _tail.pop_front();
            float length = _width / _xAmount;
            int width = _width/length;
            int height = _height/length;

            ofTranslate(-_width/2, -_height/2, 0);
            ofSetColor(_backgroundColor);
            for(auto x = 0; x < width; x++){
                for(auto y = 0; y < height; y++){
                    ofDrawRectangle(x*length+1, y*length+1, length - 2, length -2);
                }
            }
            ofTranslate(_width/2, _height/2, 0);

            ofSetColor(_primaryColor);
            // draw head
            ofDrawRectangle(_position.x * length, _position.y * length, length, length);
            // draw tail
            for(auto item : _tail){
                ofDrawRectangle(item.x * length, item.y * length, length, length);
            }
            // draw food
            ofSetColor(_secondaryColor);
            ofDrawRectangle(_foodPosition.x * length, _foodPosition.y * length, length, length);

            // check boundaries
            if(_position.x >= width/2 || _position.x <= -width/2 || _position.y >= height/2 || _position.y <= -height/2){
                _tail = std::deque<glm::vec2>();
                _position = glm::vec2(0,0);
                _direction = glm::vec2(1,0);
            }

            // check food
            if(_position == _foodPosition){
                _tail.push_front(lastElement);
                _foodPosition = glm::vec2((int)(ofRandom(-width/2, width/2)),(int)(ofRandom(-height/2, height/2))) ;
            }
            end();
            setNewFrame();
        }
        void onLeft(){
            _direction = glm::vec2(-1, 0);
        }
        void onUp(){
            _direction = glm::vec2(0, -1);
        }
        void onRight(){
            _direction = glm::vec2(1, 0);
        }
        void onDown(){
            _direction = glm::vec2(0, 1);
        }

        ofParameter<int> _xAmount;
        ofParameter<ofColor> _primaryColor;
        ofParameter<ofColor> _secondaryColor;
        ofParameter<ofColor> _backgroundColor;
        ofParameter<float> _speed;
        ofParameter<void> _left;
        ofParameter<void> _up;
        ofParameter<void> _right;
        ofParameter<void> _down;

        u_int64_t _timestamp;
        glm::vec2 _foodPosition;
        glm::vec2 _position;
        std::deque<glm::vec2> _tail;
        glm::vec2 _direction;
    };
};