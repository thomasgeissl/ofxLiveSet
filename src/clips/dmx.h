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

            ofEvent<std::pair<int, int>> _valueChangeEvent;
        };
    }; // namespace clip
}; // namespace ofxLiveSet
