#pragma once
#include "ofMain.h"
#include "base.h"

namespace ofxLiveSet
{
    namespace clip
    {
        class dmx : public base
        {
        public:
            dmx(std::string name = "") : base(name)
            {
            }
            void update(){
                std::pair<int, int> value(1, ofRandom(255));
                _valueChangeEvent.notify(value);
            }
            void draw(){
            }
            ofEvent<std::pair<int, int>> _valueChangeEvent;
        };
    }; // namespace clip
}; // namespace ofxLiveSet
