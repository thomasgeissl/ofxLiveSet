#pragma once
#include "clips/graphic.h"
namespace clips {
    class progressBar : public ofxLiveSet::clip::graphic {
    public:
        typedef std::shared_ptr<progressBar> pointer;
        static pointer create(std::string name="")
        {
            return std::make_shared<progressBar>(name);
        }
        progressBar(std::string name = "progressBar") : graphic(name){
            _name = name;
            _active.setName(_name);

            _primaryColor.set("primaryColor", ofColor::red);
            _backgroundColor.set("backgroundColor", ofColor::black);

            _parameters.add(_primaryColor);
            _parameters.add(_backgroundColor);
        }
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            auto x = _width/4;
            auto height = 50;
            auto y = (_height - height)/2;
            auto width = ofMap(std::abs(std::sin(ofGetElapsedTimef())), 0, 1, 0, _width/2);
            
            begin(false);
            ofClear(_backgroundColor);
            ofSetColor(_primaryColor);

            ofDrawRectangle(x-_width/2, y-_height/2, width, height);
            end();
            setNewFrame();
        }

        ofParameter<ofColor> _primaryColor;
        ofParameter<ofColor> _backgroundColor;
    };
};