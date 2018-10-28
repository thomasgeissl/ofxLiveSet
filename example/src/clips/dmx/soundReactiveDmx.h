#pragma once
#include "clips/dmx.h"

namespace clips {
    class soundReactiveDmx : public ofxLiveSet::clip::dmx {
    public:
        soundReactiveDmx() : dmx(){}
        virtual void setPeakEnergy(float value){}
        virtual void setPitch(float value){}

    };
};
