#pragma once
#include "ofxLiveSet.h"
#include "../../dmx.config.h"

namespace clips
{
class newClip : public ofxLiveSet::clip::dmx, public ofxLiveSet::clip::soundReactive
{
public:
    typedef std::shared_ptr<newClip> pointer;
    static pointer create()
    {
        return std::make_shared<newClip>();
    }
    newClip() : ofxLiveSet::clip::dmx(), ofxLiveSet::clip::soundReactive(), _beatAudioAnalyserId(2)
    {
        _name = "com";
        _active.setName(_name);

        _pitchChangeTriggerSoundAnalyserId.set("pitchChangeTrigger", 2, 0, 32);
        _minValue.set("minValue", 0, 0, 255);
        _maxValue.set("maxValue", 168, 0, 255);


        // addParameter(_soundAnalyserId);
        addParameter(_maxPeakEnergy.set("maxPeakEnergy", 0.6, 0, 1));
        addParameter(_speed.set("speed", .2, 0, 1));
        addParameter(_minValue);
        addParameter(_maxValue);

        _meters.setName("meters");
        _meters.add(_peakEnergy.set("peakEnergy", 0, 0, 5));
        _parameters.add(_meters);

        _values.resize(KLS_LIGHTBULBSCOUNT);
        _timestamps.resize(KLS_LIGHTBULBSCOUNT);
    }

    void update()
    {
        auto timestamp = ofGetElapsedTimeMillis();
        int index = ofNoise(timestamp)*_values.size();
        for(auto i = 0; i < _values.size(); i++)
        {
            ofLogNotice() << ofNoise(i)*_values.size();
            auto distance = std::abs(i - index);
            // auto max = 255;
            // if(_peakEnergy < 0.2){
            //     max = ofMap(std::sin(timestamp * i * _speed/100), -1, 1, _minValue, _maxValue);
            // }else{

            // }

            // _values[i] = max - ofMap(_peakEnergy, 0, _maxPeakEnergy, 0, 255);
            _values[i] = ofMap(distance, 0, _values.size(), 0, 255);
            // std::abs( - ofMap(_peakEnergy, 0, 1, 0, 255));
        }
        for (auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++)
        {
            setValue(i+1, _values[i]);
        }
    }
    void stop()
    {
        for (auto i = 1; i <= KLS_LIGHTBULBSCOUNT; i++)
        {
            setValue(i, 0);
        }
        base::stop();
    }
    void setPeakEnergy(int analyserId, float value)
    {
        if (analyserId == _beatAudioAnalyserId)
        {
            // TODO
            // if(_smoothing){
            //     if(energy > threshold)
            // }
            _peakEnergy = value;// ? 0.5 > value*2 : value;
        }
    }

    ofParameter<int> _pitchChangeTriggerSoundAnalyserId;
    ofParameter<int> _minValue;
    ofParameter<int> _maxValue;
    ofParameter<float> _speed;
    ofParameter<bool> _breathe;

    ofParameter<float> _peakEnergyThreshold;
    ofParameter<float> _beatsEnergyThreshold;

    ofParameterGroup _meters;
    ofParameter<float> _peakEnergy;
    ofParameter<float> _pitchChangeTriggerPeakEnergy; 
    ofParameter<float> _maxPeakEnergy;

    int _beatAudioAnalyserId;

    std::vector<int> _values;
    std::vector<u_int64_t> _timestamps;
    u_int64_t _highsQuadrantChangeTimestamp;
};
}; // namespace clips
