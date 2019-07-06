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
    typedef std::shared_ptr<graphic> pointer;
    static pointer create(std::string name)
    {
        return std::make_shared<graphic>(name);
	}
    graphic(std::string name = "") : base(name), _newFrame(true){
		_width.addListener(this, &graphic::onWidthChange);
		_height.addListener(this, &graphic::onHeightChange);
		_width = ofGetWidth();
		_height = ofGetHeight();
        _x.set("x", 0, -1, 1);
        _y.set("y", 0, -1, 1);
		_scale.set("scale", 0, -1, 1);
		_rotate.set("rotate", 0, 0, 359);

		_shaderEnabled.set("shaderEnabled", false);

		_transform.setName("transform");
		_transform.add(_x);
		_transform.add(_y);
		_transform.add(_scale);
		_transform.add(_rotate);
		_parameters.add(_transform);
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
	void setSize(float width, float height){
		_width = width;
		_height = height;
	}

	void begin(bool clear = true){
		_fbo.begin();
		if(_shaderEnabled){
			_shader.begin();
		}
		if(clear){
			ofClear(255,0);
		}
		ofPushMatrix();
		ofPushStyle();
		ofTranslate(ofMap(_x, -1, 1, 0, _width), ofMap(_y, -1, 1, 0, _height));
        ofScale(ofMap(_scale, -1, 1, 0, 4), ofMap(_scale, -1, 1, 0, 4), 0);
		ofRotateDeg(_rotate);
	}
	void end(){
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
	ofParameterGroup _transform;
	ofParameter<float> _x;
    ofParameter<float> _y;
	ofParameter<float> _scale;
	ofParameter<float> _rotate;


	ofParameter<float> _width;
	ofParameter<float> _height;
	
	bool _newFrame;

};
}; // namespace clip
}; // namespace ofxLiveSet
