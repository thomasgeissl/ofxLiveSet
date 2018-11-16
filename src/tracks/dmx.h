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
            dmx(std::string name = "") : base(name)
            {
            }
            void setup(ofxDmx *dmx){
                _dmx = dmx;
                for(auto clip : _clips){
                    auto dmxClip = dynamic_cast<ofxLiveSet::clip::dmx *>(clip);
                    if (dmxClip != nullptr){
                        ofAddListener(((ofxLiveSet::clip::dmx *)(clip))->_valueChangeEvent, this, &ofxLiveSet::track::dmx::onValueChange);                    }
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

