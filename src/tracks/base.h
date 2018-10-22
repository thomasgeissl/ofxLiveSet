#pragma once
#include "ofMain.h"
#include "../clips/base.h"

namespace ofxLiveSet
{
namespace track
{
class base
{
public:
    base(std::string name = "") : _clip(nullptr)
	{
        _name.set("name", name);
	}
    virtual void setup(){}
	virtual void update(){
		if (_clip)
		{
            _clip->update();
		}
	}
	void draw()
	{
		if (_clip)
		{
            _clip->draw();
		}
	}
	void start()
	{
        if (_clip)
        {
            _clip->start();
        }
	}
	void pause()
	{
	}
	void stop()
	{
        if (_clip)
        {
            _clip->stop();
        }
	}
	void addClip(clip::base *clip)
	{
		_clips.push_back(clip);
	}
    void setClip(clip::base *clip){
        _clip = clip;
    }
    void trigger(int index){
        stop();
        _clip = _clips[index];
    }
	std::vector<clip::base *> _clips;
	clip::base *_clip;
	ofParameterGroup _parameters;
	ofParameter<std::string> _name;
};
}; // namespace track
}; // namespace ofxLiveSet
