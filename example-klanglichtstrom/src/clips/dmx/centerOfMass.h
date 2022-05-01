#pragma once
#include "ofxLiveSet.h"
#include "../../dmx.config.h"

namespace clips
{
class newClip : public ofxLiveSet::clip::dmx, public ofxLiveSet::clip::hasSoundAnalyserInput
{
public:
    typedef std::shared_ptr<newClip> pointer;
    static pointer create()
    {
        return std::make_shared<newClip>();
    }
    newClip() : ofxLiveSet::clip::dmx(), ofxLiveSet::clip::hasSoundAnalyserInput(), _beatAudioAnalyserId(2)
    {
        _name = "com";
        _active.setName(_name);

        _pitchChangeTriggerSoundAnalyserId.set("pitchChangeTrigger", 2, 0, 32);


        // addParameter(_soundAnalyserId);
        addParameter(_maxPeakEnergy.set("maxPeakEnergy", 0.1, 0, 1));
        addParameter(_speed.set("speed", .2, 0, 1));
        addParameter(_minValue.set("minValue", 0, 0, 255));
        addParameter(_maxValue.set("maxValue", 168, 0, 255));
        addParameter(_compressor.set("compressor", true));
        addParameter(_motion.set("motion", false));
        addParameter(_motionSpeed.set("motionSpeed", 0.1, 0, 1));

        _meters.setName("meters");
        _meters.add(_peakEnergy.set("peakEnergy", 0, 0, 5));
        _meters.add(_centerIndex.set("center", 0, 0, KLS_LIGHTBULBSCOUNT));
        _parameters.add(_meters);

        _values.resize(KLS_LIGHTBULBSCOUNT);
        _timestamps.resize(KLS_LIGHTBULBSCOUNT);
    }

    void update()
    {
        auto timestamp = ofGetElapsedTimeMillis();
        _centerIndex = ofNoise((float)(timestamp)/1000 * _motionSpeed)*_values.size();
        auto distanceThreshold = 6;
        auto compression = 0;
        if(_compressor){
            compression = ofMap(_peakEnergy, 0, _maxPeakEnergy, 0, 255);
        }
        for(auto i = 0; i < _values.size(); i++)
        {
            auto distance = std::abs(i - _centerIndex);

            int max = _maxValue;
            if(_motion && distance > distanceThreshold){
            }
            if(_motion && distance >= distanceThreshold){
                max = _maxValue/2;
            }
            _values[i] = ofClamp(ofMap(std::abs(std::sin(ofGetElapsedTimef()*10*_speed+0.4*i)), 0, 1, _minValue, max) - compression, 0, 255);
        }

        // finally send all values
        for (auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++)
        {
            setDmxValue(i+1, _values[i]);
        }
    }
    void stop()
    {
        for (auto i = 1; i <= KLS_LIGHTBULBSCOUNT; i++)
        {
            setDmxValue(i, 0);
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
    ofParameter<bool> _compressor;
    ofParameter<bool> _motion;
    ofParameter<float> _motionSpeed;

    ofParameter<float> _peakEnergyThreshold;
    ofParameter<float> _beatsEnergyThreshold;

    ofParameterGroup _meters;
    ofParameter<float> _peakEnergy;
    ofParameter<float> _pitchChangeTriggerPeakEnergy; 
    ofParameter<float> _maxPeakEnergy;
    ofParameter<int> _centerIndex;

    int _beatAudioAnalyserId;

    std::vector<int> _values;
    std::vector<u_int64_t> _timestamps;
    u_int64_t _highsQuadrantChangeTimestamp;
};
}; // namespace clips
