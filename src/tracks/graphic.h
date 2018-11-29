#pragma once
#include "ofMain.h"
#include "base.h"
#include "../clips/graphic.h"

namespace ofxLiveSet{
namespace track{
class graphic : public base{
public:
    graphic(std::string name = "") : base(name){
        _xPosition.set("x", 0, 0, ofGetWidth());
        _yPosition.set("y", 0, 0, ofGetHeight());
        _width.set("width", ofGetWidth(), 0, ofGetWidth());
        _height.set("height", ofGetHeight(), 0, ofGetHeight());

        _width.addListener(this, &graphic::onWidthChange);
        _height.addListener(this, &graphic::onHeightChange);

        _shape.setName("shape");
        _shape.add(_xPosition);
        _shape.add(_yPosition);
        _shape.add(_width);
        _shape.add(_height);
        _ioParameters.add(_shape);
        _fbo.allocate(_width, _height);
        _fbo.begin();
        ofClear(255,0);
        _fbo.end();
	}
    virtual void draw(){
        auto clip = ((ofxLiveSet::clip::graphic *)(_clip));

        if(!_mute && clip != nullptr && clip->isFrameNew()) {
            _fbo.begin();
            ofClear(255,0);
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            ofSetColor(255, _gain * 255);
            clip->_fbo.draw(0,0);
            _fbo.end();
        }
        if(clip == nullptr){
            _fbo.begin();
            ofClear(255,0);
            _fbo.end();
        }
        _fbo.draw(_xPosition, _yPosition);
    }
    void resize(float width, float height){
        _width.setWithoutEventNotifications(width);
        _height.setWithoutEventNotifications(height);
        _fbo.allocate(width, height);
        _fbo.begin();
        ofClear(255,0);
        _fbo.end();
        for(auto clip : _clips){
            auto graphicClip = dynamic_cast<ofxLiveSet::clip::graphic *>(clip);
            if (graphicClip != nullptr) {
                graphicClip->setSize(_width, _height);
            }
        }
        _xPosition.setMax(_width);
        _yPosition.setMax(_height);
    }
    void onWidthChange(float & value){
        resize(_width, _height);
    }
    void onHeightChange(float & value){
        resize(_width, _height);
    }
    ofFbo _fbo;
    ofParameterGroup _shape;
    ofParameter<float> _xPosition;
    ofParameter<float> _yPosition;
    ofParameter<float> _width;
    ofParameter<float> _height;
};
}; // namespace track
}; // namespace ofxLiveSet
