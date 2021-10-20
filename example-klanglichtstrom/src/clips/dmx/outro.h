#pragma once
#include "ofxLiveSet.h"
#include "../../dmx.config.h"

namespace clips
{
    class outro : public ofxLiveSet::clip::dmx, public ofxLiveSet::clip::soundReactive
    {
    public:
        typedef std::shared_ptr<outro> pointer;
        static pointer create()
        {
            return std::make_shared<outro>();
        }
        outro() : ofxLiveSet::clip::dmx(), ofxLiveSet::clip::soundReactive()
        {
            _name = "rain";
            _active.setName(_name);
            _minValue.set("minValue", 0, 0, 255);
            _maxValue.set("maxValue", 168, 0, 255);

            // addParameter(_soundAnalyserId);
            addParameter(_intensity.set("intensity", .2, 0, 1));
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
            if (timestamp - _timestamp > ofRandom(ofMap(_intensity, 0, 1, 1000,100)))
            {
                auto index = ofRandom(KLS_LIGHTBULBSCOUNT);
                _values[index] = ofRandom(_minValue, _maxValue);
                _timestamps[index] = timestamp;
                _timestamp = timestamp;
            }
            for (auto i = 0; i < _values.size(); i++)
            {
                if (_values[i] > 0 && (timestamp - _timestamp > ofMap(_speed, 0, 1, ofRandom(200), 0)))
                {
                    _values[i]--;
                }
            }
            for (auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++)
            {
                setDmxValue(i + 1, _values[i]);
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
            if (analyserId == _soundAnalyserId)
            {
                _peakEnergy = value; // ? 0.5 > value*2 : value;
            }
        }

        ofParameter<int> _pitchChangeTriggerSoundAnalyserId;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        ofParameter<float> _intensity;
        ofParameter<float> _speed;

        ofParameterGroup _meters;
        ofParameter<float> _peakEnergy;

        std::vector<int> _values;
        std::vector<u_int64_t> _timestamps;
        u_int64_t _timestamp;
    };
}; // namespace clips
