#pragma once
#include "ofMain.h"
#include "ofxOsc.h"
#include "base.h"

namespace ofxLiveSet
{
    namespace clip
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
            }
            void sendOscMessage(ofxOscMessage &message)
            {
                _oscMessageEvent.notify(message);
            }

            ofEvent<ofxOscMessage> _oscMessageEvent;
        };
    }; // namespace clip
};     // namespace ofxLiveSet
