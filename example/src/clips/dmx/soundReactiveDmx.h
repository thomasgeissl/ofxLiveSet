#pragma once
#include "clips/dmx.h"

namespace clips {
    class soundReactiveDmx : public ofxLiveSet::clip::dmx {
    public:
        soundReactiveDmx() : dmx(){}
        virtual void setPeakEnergy(float &value){}
        virtual void setPitch(float &value){}
        virtual void setRootMeanSquare(float &value){}
        virtual void setFftMagnitudeSpectrum(std::vector<float> &value){}
        virtual void setMelFrequencySpectrum(std::vector<float> &value){}
    };
};
