#pragma once
#include "ofMain.h"
#include "base.h"
#include "ofxOsc.h"
#include "../clips/osc.h"

namespace ofxLiveSet
{
    namespace track
    {
        class osc : public base
        {
        public:
            typedef std::shared_ptr<osc> pointer;
            static pointer create(std::string name)
            {
                return std::make_shared<osc>(name);
            }
            osc(std::string name = "") : base(name)
            {
        	// _ioParameters.add(_shape);
            }
	    void setup(int port = 8000, std::string host = "localhost")
	    {
		    _oscSender.setup(host, port);
		                    for(auto clip : _clips){
                    auto oscClip = dynamic_pointer_cast<ofxLiveSet::clip::osc>(clip);
                    if (oscClip != nullptr){
                        ofAddListener(oscClip->_oscMessageEvent, this, &ofxLiveSet::track::osc::onOscMessageEvent);
                    }
                    }
	    }
	void onOscMessageEvent(ofxOscMessage &value){
                if(!_mute){
			// ofLogNotice() << "sending osc message";
			_oscSender.sendMessage(value);
                }
            }
	protected:
		ofxOscSender _oscSender;
        };
    }; // namespace track
}; // namespace ofxLiveSet

