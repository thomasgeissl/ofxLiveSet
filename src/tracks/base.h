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
        _mute.set("mute", false);
        _solo.set("solo", false);
        _parameters.add(_name);
        _parameters.add(_solo);
        _parameters.add(_mute);

	}
    virtual void setup(){}
	virtual void update(){
		if (_clip != nullptr)
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
    clip::base* addClip(clip::base *clip)
	{
		_clips.push_back(clip);
        return clip;
	}
    void setClip(clip::base *clip){
        _clip = clip;
    }
    void trigger(int index){
        stop();
        if(index < _clips.size()){
            _clip = _clips[index];
        }
    }
	std::vector<clip::base *> _clips;
	clip::base *_clip;
	ofParameterGroup _parameters;
	ofParameter<std::string> _name;
    ofParameter<bool> _mute;
    ofParameter<bool> _solo;
};
}; // namespace track
}; // namespace ofxLiveSet
