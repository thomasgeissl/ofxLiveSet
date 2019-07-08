#pragma once
#include "clips/graphic.h"
namespace clips {
    class randomRectangles : public ofxLiveSet::clip::graphic {
    public:
        typedef std::shared_ptr<randomRectangles> pointer;
        static pointer create(std::string name="")
        {
            return std::make_shared<randomRectangles>(name);
        }
        randomRectangles(std::string name = "randomRects") : graphic(name){
            _name = name;
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

            begin();
            ofSetColor(_primaryColor);

            for(auto i = 0; i < _amount; i++){
                auto width = ofRandom(0, _width/10);
                auto height = ofRandom(0, _height/10);
                auto x = ofRandom(-_width/2, _width/2);
                auto y = ofRandom(-_height/2, _height/2);

                ofDrawRectangle(x, y, width, height);
            }
            end();
            _timestamp = timestamp;
            setNewFrame();
        }

        ofParameter<ofColor> _primaryColor;
        ofParameter<int> _amount;
        ofParameter<int> _frequency;

        u_int64_t _timestamp;
    };
};