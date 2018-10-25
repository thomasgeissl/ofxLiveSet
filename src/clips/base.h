#pragma once
#include "ofMain.h"
#include "session.h"

namespace ofxLiveSet
{
namespace clip
{
class base
{
public:
    base(std::string name = "")
	{
        _active.addListener(this, &base::onActiveChange);
	}
    virtual void setup()
    {
        ofLogNotice("clip") << "setup";
    }
	virtual void update()
	{
		ofLogNotice("clip") << "update";
	}
	virtual void draw()
	{
		ofLogNotice("clip") << "draw";
	}
	virtual void start()
	{
        _active = true;
	}
	virtual void pause()
	{
		ofLogNotice("clip") << "pause";
	}
	virtual void stop()
	{
        _active = false;
	}
    
    void onActiveChange(bool & value) {
        bool valueToBeNotified = true;
        if(value){
            ofNotifyEvent(_started, valueToBeNotified, this);
        } else {
            ofNotifyEvent(_stopped, valueToBeNotified, this);
        }
    }
	ofParameterGroup _parameters;
	ofParameter<std::string> _name;
	ofParameter<bool> _active;

     ofEvent<bool> _started;
     ofEvent<bool> _finished;
     ofEvent<bool> _stopped;
};
}; // namespace clip
}; // namespace ofxLiveSet
