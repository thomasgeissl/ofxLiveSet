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
	void update()
	{
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
	}
	void pause()
	{
	}
	void stop()
	{
	}
	void addClip(clip::base *clip)
	{
		_clips.push_back(clip);
	}
    void setClip(clip::base *clip){
        _clip = clip;
    }
	std::vector<clip::base *> _clips;
	clip::base *_clip;
	ofParameterGroup _parameters;
	ofParameter<std::string> _name;
};
}; // namespace track
}; // namespace ofxLiveSet
