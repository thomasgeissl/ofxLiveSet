#pragma once
#include "ofxLiveSet.h"

namespace clips
{
class crankConfig : public ofxLiveSet::clip::dmx
{
public:
    typedef std::shared_ptr<crankConfig> pointer;
    static pointer create()
    {
        return std::make_shared<crankConfig>();
    }
    crankConfig() : ofxLiveSet::clip::dmx()
    {
        _name = "crankConfig";
        _active.setName(_name);
    }
};
}; // namespace clips
