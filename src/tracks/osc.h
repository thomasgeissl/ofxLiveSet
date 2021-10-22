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
            osc(std::string name = "") : base("OFXLIVESET_TRACK_OSC", name)
            {
                _ioParameters.add(_host.set("host", "localhost"));
                _ioParameters.add(_port.set("port", 8010, 8001, 12000));

                _host.addListener(this, &osc::onHostChange);
                _port.addListener(this, &osc::onPortChange);
            }
            void setup(int port = 8000, std::string host = "localhost")
            {
                _port = port;
                _host = host;
                _oscSender.setup(_host, _port);
                for (auto clip : _clips)
                {
                    auto oscClip = dynamic_pointer_cast<ofxLiveSet::clip::osc>(clip);
                    if (oscClip != nullptr)
                    {
                        ofAddListener(oscClip->_oscMessageEvent, this, &ofxLiveSet::track::osc::onOscMessageEvent);
                    }
                }
            }
            void onOscMessageEvent(ofxOscMessage &value)
            {
                if (!_mute)
                {
                    _oscSender.sendMessage(value);
                }
            }
            void onHostChange(std::string &value){
                _oscSender.setup(_host, _port);
            }
            void onPortChange(int &value){
                _oscSender.setup(_host, _port);
            }


        protected:
            ofxOscSender _oscSender;
            ofParameter<std::string> _host;
            ofParameter<int> _port;
        };
    }; // namespace track
};     // namespace ofxLiveSet
