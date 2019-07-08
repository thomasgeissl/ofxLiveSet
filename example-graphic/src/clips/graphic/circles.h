#pragma once
#include "clips/graphic.h"
namespace clips {
    class circles : public ofxLiveSet::clip::graphic {
    public:
        typedef std::shared_ptr<circles> pointer;
        static pointer create(std::string name="")
        {
            return std::make_shared<circles>(name);
        }
        circles(std::string name = "circles") : graphic(name){
            _name = name;
            _active.setName(_name);
            
            _primaryColor.set("primaryColor", ofColor::white);
            _amount.addListener(this, &circles::onAmountChange);
            _amount.set("amount", 5, 1, 50);

            _radius.set("_radius", 100, 1, 500);

            _parameters.add(_primaryColor);
            _parameters.add(_amount);
            _parameters.add(_radius);
        }
        void update(){
            begin();
            ofEnableAlphaBlending();
            for(auto i = 0; i < _amount; i++){
                ofSetColor(_colours[i]);
                // TODO: move center for each circle
                ofDrawCircle(0,0, _radius + i * 20);
            }
            end();
            setNewFrame();
        }
        void onAmountChange(int & value){
            _colours.resize(value);
            for(auto i = 0; i < _colours.size(); i++){
                // TODO: should depend on primary colour
                _colours[i] = ofColor(ofRandom(255), ofRandom(255), ofRandom(255), ofRandom(200, 255) );
            }
        }
        std::vector<ofColor> _colours;
        ofParameter<ofColor> _primaryColor;
        ofParameter<int> _amount;
        ofParameter<int> _radius;
    };
};