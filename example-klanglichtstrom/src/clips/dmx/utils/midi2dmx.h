#pragma once
#include "ofxLiveSet.h"

namespace clips
{
class midi2dmx : public ofxLiveSet::clip::dmx, public ofxLiveSet::clip::midiReactive
{
public:
    typedef std::shared_ptr<midi2dmx> pointer;
    static pointer create()
    {
        return std::make_shared<midi2dmx>();
    }
    midi2dmx() : ofxLiveSet::clip::dmx(), ofxLiveSet::clip::midiReactive()
    {
        _name = "midi2dmx";
        _active.setName(_name);
        // addParameter(_channel);
        _channelOffset.set("channelOffset", 23, 0, 255);
        addParameter(_channelOffset);
    }

    void setNoteOn(int note, int velocity)
    {
        ofLogNotice("midi2dmx") << "note on " << note << ", " << velocity;
        setDmxValue(note - _channelOffset, velocity * 2);
    }
    void setNoteOff(int note, int velocity)
    {
        setDmxValue(note - _channelOffset, 0);
    }
    ofParameter<int> _channelOffset;
};
}; // namespace clips
