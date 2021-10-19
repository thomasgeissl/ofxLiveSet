#pragma once
#include "ofMain.h"
#include "base.h"
#include "ofxDmx.h"
#include "../clips/dmx.h"

namespace ofxLiveSet
{
    namespace track
    {
        class dmx : public base
        {
        public:
            typedef std::shared_ptr<dmx> pointer;
            static pointer create(std::string name)
            {
                return std::make_shared<dmx>(name);
            }
            dmx(std::string name = "") : base("OFXLIVESET_TRACK_DMX", name)
            {
            }
            void setup(ofxDmx *dmx){
                _dmx = dmx;
                for(auto clip : _clips){
                    auto dmxClip = dynamic_pointer_cast<ofxLiveSet::clip::dmx>(clip);
                    if (dmxClip != nullptr){
                        ofAddListener(dmxClip->_valueChangeEvent, this, &ofxLiveSet::track::dmx::onValueChange);
                    }
                }
            }
            void onValueChange(std::pair<int, int> &value){
                if(!_mute){
                    _dmx->setLevel(value.first, _gain * value.second);
                }
            }
            ofxDmx * _dmx;
        };
    }; // namespace track
}; // namespace ofxLiveSet

