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
	virtual void draw()
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
        _parameters.add(clip->_active);
        ofAddListener(clip->_started, this, &base::onClipStarted);
        ofAddListener(clip->_stopped, this, &base::onClipStopped);

        return clip;
	}
    void setClip(clip::base *clip){
        _clip = clip;
    }
    void trigger(int index){
        stop();
        if(index < _clips.size()){
            _clip = _clips[index];
            _clip->start();
        }
    }
    
    void onClipStarted(const void* sender, bool & value) {
        for(auto clip : _clips) {
            if(clip != sender) {
                clip->stop();
            }
        }
        _clip = (clip::base *) (sender);
    }
    void onClipStopped(const void* sender, bool & value) {
        if(_clip == sender) {
            _clip->stop();
            _clip = nullptr;
        }
    }
   
	std::vector<clip::base *> _clips;
	clip::base *_clip;
	ofParameterGroup _parameters;
	ofParameter<std::string> _name;
    ofParameter<bool> _mute;
    ofParameter<bool> _solo;
    ofParameterGroup _clipTriggers;
};
}; // namespace track
}; // namespace ofxLiveSet