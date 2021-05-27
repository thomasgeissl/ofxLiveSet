#pragma once
#include "./base.h"
namespace ofxLiveSet
{
    namespace clip
    {
        class soundReactive
        {
        public:
            soundReactive()
            {
                _soundAnalyserId.set("analyserId", 1, 1, 8);
            }
            virtual void setPeakEnergy(int analyserId, float value) {}
            virtual void setPitch(int analyserId, float value) {}
            virtual void setRootMeanSquare(int analyserId, float value) {}
            virtual void setFftMagnitudeSpectrum(int analyserId, std::vector<float> value) {}
            virtual void setMelFrequencySpectrum(int analyserId, std::vector<float> value) {}
            virtual void setSpectralCentroid(int analyserId, float value) {}
            virtual void setSpectralCrest(int analyserId, float value) {}
            virtual void setSpectralDifference(int analyserId, float value) {}
            virtual void setSpectralFlatness(int analyserId, float value) {}
            virtual void setZeroCrossingRate(int analyserId, float value) {}
            virtual void setChord(int analyserId, float value) {}
            virtual void setChromogram(int analyserId, std::vector<float> value) {}
            ofParameter<int> _soundAnalyserId;
        };
    }; // namespace clip
};     // namespace ofxLiveSet
