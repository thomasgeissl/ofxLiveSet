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
            void setup(ofxDmx &dmx){
                _dmx = dmx;
                for(auto clip : _clips){
                    ofAddListener(((ofxLiveSet::clip::dmx *)(clip))->_valueChangeEvent, this, &ofxLiveSet::track::dmx::onValueChange);
                }
            }
            void update(){
                base::update();
                _dmx.update();
            }
            void onValueChange(std::pair<int, int> &value){
                _dmx.setLevel(value.first, value.second);
            }
            ofxDmx _dmx;
        };
    }; // namespace track
}; // namespace ofxLiveSet

