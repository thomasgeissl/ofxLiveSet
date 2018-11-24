#pragma once
#include "ofMain.h"
#include "base.h"
#include "ofxPDSP.h"

namespace ofxLiveSet
{
namespace clip
{
class audio : public base, public pdsp::Patchable
{
public:
    audio(std::string name = "") : base(name)
	{
	}
	void update()
	{
		ofLogNotice("clip::audio") << "update";
	}
	void start()
	{
		ofLogNotice("clip::audio") << "start";
	}
	void pause()
	{
		ofLogNotice("clip::audio") << "pause";
	}
	void stop()
	{
		ofLogNotice("clip::audio") << "stop";
	}
};
}; // namespace clip
}; // namespace ofxLiveSet
