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
        _x.set("x", 0.5, 0, 1);
        _y.set("y", 0.5, 0, 1);
		_scale.set("scale", 0.5, 0, 1);
		_shaderEnabled.set("shaderEnabled", false);

		_parameters.add(_x);
		_parameters.add(_y);
		_parameters.add(_scale);
	}
	bool isFrameNew(){
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
		ofPushMatrix();
		ofPushStyle();
		ofScale(ofMap(_scale, 0, 1, 0, 2), ofMap(_scale, 0, 1, 0, 2), 0);

	}
	void endFboWithShaderIfActive(){
		ofPopStyle();
		ofPopMatrix();
		if(_shaderEnabled){
			_shader.end();
		}
		_fbo.end();
	}

	ofFbo _fbo;
	ofShader _shader;
	ofParameter<bool> _shaderEnabled;
	ofParameter<float> _x;
    ofParameter<float> _y;
	ofParameter<float> _scale;


	ofParameter<float> _width;
	ofParameter<float> _height;
	
	bool _newFrame;

};
}; // namespace clip
}; // namespace ofxLiveSet
