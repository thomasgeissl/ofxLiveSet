#pragma once
#include "clips/graphic.h"
namespace clips {
    class cubeWithTrails : public ofxLiveSet::clip::graphic {
    public:
        cubeWithTrails() : graphic(){
            _name = "cubeWithTrails";
            _active.setName(_name);
            
            _primaryColor.set("primaryColor", ofColor::white);
            _amount.set("amount", 5, 1, 50);
            _size.set("size", 200, 10, 500);

            _parameters.add(_primaryColor);
            _parameters.add(_amount);
            _parameters.add(_size);

        }
        void update(){
            ofEnableAlphaBlending();
            _fbo.begin();
            ofFill();
            ofSetColor(255,255,255, _amount);
            ofDrawRectangle(0,0,_fbo.getWidth(),_fbo.getHeight());

	        ofNoFill();
	        ofSetColor(_primaryColor);

            ofPushMatrix();
                ofTranslate(_fbo.getWidth()/2,_fbo.getHeight()/2,0);
                ofRotateDeg(ofGetElapsedTimef()*30, 1,0,0.5);
                ofDrawBox(0,0,0,_size);
            ofPopMatrix();

            _fbo.end();
            setNewFrame();
        }

        ofParameter<ofColor> _primaryColor;
        ofParameter<int> _amount;
        ofParameter<int> _size;

    };
};