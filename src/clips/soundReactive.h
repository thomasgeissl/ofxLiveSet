#pragma once
#include "./base.h"
namespace ofxLiveSet {
namespace clip {
    class soundReactive{
    public:
        soundReactive(){
            _soundAnalyserId.set("analyserId", 1, 0, 32);
        }
        virtual void setPeakEnergy(int analyserId, float value){}
        virtual void setPitch(int analyserId, float value){}
        virtual void setRootMeanSquare(int analyserId, float value){}
        virtual void setFftMagnitudeSpectrum(int analyserId, std::vector<float> value){}
        virtual void setMelFrequencySpectrum(int analyserId, std::vector<float> value){}
        ofParameter<int> _soundAnalyserId;
    };
};
};

