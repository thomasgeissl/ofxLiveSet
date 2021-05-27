#pragma once
#include "ofMain.h"
#include "ofxLiveSet.h"
// #include "ofxMQTT.h"

namespace ofxLiveSet
{
    namespace track
    {
        class mqtt : public base
        {
        public:
            typedef std::shared_ptr<mqtt> pointer;
            static pointer create(std::string name)
            {
                return std::make_shared<mqtt>(name);
            }
            mqtt(std::string name = "") : base(name)
            {
            }
            void setup(){
            }
        };
    }; // namespace track
}; // namespace ofxLiveSet

