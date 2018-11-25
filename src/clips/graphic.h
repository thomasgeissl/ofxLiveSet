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

		_shaderEnabled.set("shaderEnabled", false);
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
	void setNewFrame(bool value = true){
		_newFrame = value;
	}
	void beginFboWithShaderIfActive(){
		_fbo.begin();
		if(_shaderEnabled){
			_shader.begin();
		}
	}
	void endFboWithShaderIfActive(){
		if(_shaderEnabled){
			_shader.end();
		}
		_fbo.end();
	}
	ofFbo _fbo;
	ofShader _shader;
	ofParameter<bool> _shaderEnabled;
	ofParameter<float> _width;
	ofParameter<float> _height;
	
	bool _newFrame;

};
}; // namespace clip
}; // namespace ofxLiveSet
