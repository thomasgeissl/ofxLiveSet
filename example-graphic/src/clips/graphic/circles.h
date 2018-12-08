#pragma once
#include "clips/graphic.h"
namespace clips {
    class circles : public ofxLiveSet::clip::graphic {
    public:
        circles(std::string name = "circles") : graphic(name){
            _name = name;
            _active.setName(_name);
            
            _primaryColor.set("primaryColor", ofColor::white);
            _amount.set("amount", 1, 1, 50);
            _amount.addListener(this, &circles::onAmountChange);

            _radius.set("_radius", 100, 1, 500);

            _parameters.add(_primaryColor);
            _parameters.add(_amount);
            _parameters.add(_radius);
            _amount = 5;
        }
        void update(){
            begin();
            ofEnableAlphaBlending();
            auto radius = 20;
            ofSetLineWidth(20);
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