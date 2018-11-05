#pragma once
#include "clips/dmx.h"

namespace clips {
    class soundReactiveDmx : public ofxLiveSet::clip::dmx {
    public:
        soundReactiveDmx() : dmx(){
            _soundAnalyserId.set("analyserId", 0, 0, 32);
            _parameters.add(_soundAnalyserId);
        }
        virtual void setPeakEnergy(int analyserId, float value){}
        virtual void setPitch(int analyserId, float value){}
        virtual void setRootMeanSquare(int analyserId, float value){}
        virtual void setFftMagnitudeSpectrum(int analyserId, std::vector<float> value){}
        virtual void setMelFrequencySpectrum(int analyserId, std::vector<float> value){}
        ofParameter<int> _soundAnalyserId;
    };
};

