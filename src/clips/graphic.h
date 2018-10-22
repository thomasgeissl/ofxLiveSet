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
    graphic(std::string name = "") : base(name)
	{
		_fbo.allocate(1920, 1080);
	}
	void update()
	{
		_fbo.begin();
        ofSetColor(255,0,0);
        ofDrawRectangle(100, 100, 200, 200);
		_fbo.end();
	}
    void draw()
    {
        _fbo.draw(0,0);
    }
	void start()
	{
		ofLogNotice("clip::graphic") << "start";
	}
	void pause()
	{
		ofLogNotice("clip::grapphic") << "pause";
	}
	void stop()
	{
		ofLogNotice("clip::graphic") << "stop";
	}
	ofFbo _fbo;
};
}; // namespace clip
}; // namespace ofxLiveSet
