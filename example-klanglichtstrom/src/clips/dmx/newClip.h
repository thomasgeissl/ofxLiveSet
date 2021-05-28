#pragma once
#include "ofxLiveSet.h"

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
    newClip() : ofxLiveSet::clip::dmx(), ofxLiveSet::clip::soundReactive()
    {
        _name = "newClip";
        _pitchChangeTriggerSoundAnalyserId.set("pitchChangeTrigger", 2, 0, 32);
        _minValue.set("minValue", 0, 0, 255);
        _maxValue.set("maxValue", 168, 0, 255); // beatMaxValue
        _addPeakEnergy.set("addPeakEnergy", false);

        _active.setName(_name);

        _beatsQuadrant.set("beatsQuadrant", 0, 0, 3);
        _beatsEnergyThreshold.set("beatsEnergyThreshold", 0.2, 0, 1);

        _highs.set("highs", true);
        _highsQuadrant.set("highsQuadrant", 1, 0, 3);
        _highsMaxValue.set("highsMaxValue", 0, 0, 255);

        _randomiseHighsQuadrant.set("randomiseHighsQuadrant");
        _randomiseHighsQuadrant.addListener(this, &newClip::onRandomiseHighsQuadrant);

        addParameter(_soundAnalyserId);
        addParameter(_pitchChangeTriggerSoundAnalyserId);
        // addParameter(_minValue);
        addParameter(_maxValue);
        addParameter(_addPeakEnergy);
        addParameter(_speed.set("speed", .7, 0, 1));

        addParameter(_breathe.set("breathe", false));
        addParameter(_beatsQuadrant);
        addParameter(_beatsEnergyThreshold);
        addParameter(_highs);
        addParameter(_highsQuadrant);
        addParameter(_randomiseHighsQuadrant);
        addParameter(_highsMaxValue);

        _meters.setName("meters");
        _meters.add(_peakEnergy.set("peakEnergy", 0, 0, 5));
        _meters.add(_pitchChangeTriggerPeakEnergy.set("pitchChangeTriggerPeakEnergy", 0, 0, 5));
        _parameters.add(_meters);

        _values.resize(16);
        _timestamps.resize(16);
    }

    void update()
    {
        auto timestamp = ofGetElapsedTimeMillis();
    }
    void stop()
    {
        for (auto i = 1; i <= 16; i++)
        {
            setValue(i, 0);
        }
        base::stop();
    }
    void setPeakEnergy(int analyserId, float value)
    {
        if (analyserId == _pitchChangeTriggerSoundAnalyserId)
        {
            if (value > 0.05)
            {
                ofLogNotice() << ofToString(value, 2);
            }
            _pitchChangeTriggerPeakEnergy = value;
        }
        if (analyserId == _soundAnalyserId)
        {
            if (value > _beatsEnergyThreshold)
            {
                if (_breathe)
                {
                    _breathe = false;
                    _values[_beatsQuadrant * 4 + 3] = 0;
                }
                else
                {
                    for (auto i = _beatsQuadrant * 4; i < _beatsQuadrant * 4 + 4; i++)
                    {
                        _timestamps[i] = ofGetElapsedTimeMillis();
                        _values[i] = _maxValue * value;
                    }
                }
                _peakEnergy = value;
            }
        }
    }

    void onRandomiseHighsQuadrant()
    {
        _highsQuadrantChangeTimestamp = ofGetElapsedTimeMillis();
        int oldValue = _highsQuadrant;

        _highsQuadrant = ofRandom(0, 4 - .00001);
        while (_highsQuadrant == _beatsQuadrant || _highsQuadrant == oldValue)
        {
            _highsQuadrant = ofRandom(0, 4 - .00001);
        }

        auto channel = oldValue * 4;
        for (auto i = 0; i < 4; i++)
        {
            _values[channel + i] = 0;
        }
    }

    ofParameter<int> _pitchChangeTriggerSoundAnalyserId;
    ofParameter<int> _minValue;
    ofParameter<int> _maxValue;
    ofParameter<bool> _addPeakEnergy;
    ofParameter<float> _speed;
    ofParameter<int> _beatsQuadrant;

    ofParameter<bool> _breathe;

    ofParameter<float> _peakEnergyThreshold;
    ofParameter<float> _beatsEnergyThreshold;

    ofParameter<bool> _highs;
    ofParameter<int> _highsQuadrant;
    ofParameter<int> _highsMaxValue;

    ofParameter<void> _randomiseHighsQuadrant;

    ofParameterGroup _meters;
    ofParameter<float> _peakEnergy;
    ofParameter<float> _pitchChangeTriggerPeakEnergy;

    std::vector<int> _values;
    std::vector<u_int64_t> _timestamps;
    u_int64_t _highsQuadrantChangeTimestamp;
};
}; // namespace clips
