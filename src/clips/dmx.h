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
            void setValue(int channel, int value){
                 auto val = std::make_pair(channel, value);
                _valueChangeEvent.notify(val);
            }

            ofEvent<std::pair<int, int>> _valueChangeEvent;
        };
    }; // namespace clip
}; // namespace ofxLiveSet
