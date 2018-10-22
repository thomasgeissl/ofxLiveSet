#pragma once
#include "ofMain.h"
#include "base.h"

namespace ofxLiveSet
{
namespace clip
{
class audio : public base
{
public:
    audio(std::string name = "") : base(name)
	{
	}
    audio(std::string path, std::string name) : base(name)
	{
		_player.load(path);
	}
	void update()
	{
		ofLogNotice("clip::audio") << "update";
	}
	void start()
	{
		ofLogNotice("clip::audio") << "start";
		_player.play();
	}
	void pause()
	{
		ofLogNotice("clip::audio") << "pause";
	}
	void stop()
	{
		ofLogNotice("clip::audio") << "stop";
	}
	ofSoundPlayer _player;
};
}; // namespace clip
}; // namespace ofxLiveSet
