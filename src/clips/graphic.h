#pragma once
#include "ofMain.h"
#include "base.h"

namespace ofxLiveSet
{
namespace clip
{
class graphic : public base
{
public:
    graphic(std::string name = "") : base(name), _newFrame(true){
		_width.addListener(this, &graphic::onWidthChange);
		_height.addListener(this, &graphic::onHeightChange);
		_width = ofGetWidth();
		_height = ofGetHeight();
	}
	virtual void update(){
	}
    void draw(){
//        if(_active){
//            _fbo.draw(0,0);
//        }
    }
	void start()
	{
        base::start();
	}
	void pause()
	{
        base::pause();
	}
	void stop()
	{
        base::stop();
	}
	virtual bool isFrameNew(){
		if(_newFrame){
			_newFrame = false;
			return true;
		}
		return false;
	}
	void onWidthChange(float & value){
		_fbo.allocate(_width, _height);
	}
	void onHeightChange(float & value){
		_fbo.allocate(_width, _height);
	}
	ofFbo _fbo;
	ofParameter<float> _width;
	ofParameter<float> _height;
	bool _newFrame;

};
}; // namespace clip
}; // namespace ofxLiveSet
