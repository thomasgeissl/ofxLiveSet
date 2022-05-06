#pragma once
#include "ofxLiveSet.h"
#include "../../dmx.config.h"

#define KLS_NUMBEROFLIGHTBULBSPERSECTOR 3

namespace clips
{
    class generator : public ofxLiveSet::clip::dmx, public ofxLiveSet::clip::hasOscInput
    {
    public:
        typedef std::shared_ptr<generator> pointer;
        static pointer create()
        {
            return std::make_shared<generator>();
        }
        generator() : ofxLiveSet::clip::dmx()
        {
            _name = "generator";
            _active.setName(_name);
            addParameter(_intensity.set("intensity", 0, 0, 127));
            addParameter(_sector.set("sector", 0, 0, 7));
            addParameter(_minValue.set("minValue", 0, 0, 255));
            addParameter(_maxValue.set("maxValue", 255, 0, 255));
            addParameter(_stillMaxValue.set("stillMaxValue", 64, 0, 255));
            addParameter(_speed.set("speed", 0, 0, 1));
            addParameter(_stillSpeed.set("still speed", 0.5, 0, 1));
            addParameter(_stillIntensityThreshold.set("still intensity threshold", 8, 0, 127));
            addParameter(_peakIntensityThreshold.set("peak intensity threshold", 96, 0, 127));

            _meters.setName("meters");
            _meters.add(_peakEnergy.set("peakEnergy", 0, 0, 5));
            _parameters.add(_meters);

            _values.resize(KLS_LIGHTBULBSCOUNT);
        }

        void update()
        {
            auto timestamp = ofGetElapsedTimeMillis();

            auto startChannel = _sector * KLS_NUMBEROFLIGHTBULBSPERSECTOR;
            if (_intensity < _stillIntensityThreshold)
            {
                // TODO: maybe rain clip, or any other subtle clip
                for (auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++)
                {
                    setDmxValue(i + 1, std::abs(std::sin((i + 1) * ofGetElapsedTimef() / ofMap(_stillSpeed, 0, 1, 64, 1))) * _stillMaxValue);
                }
            }
            else if (_intensity < _peakIntensityThreshold)
            {
                for (auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++)
                {
                    setDmxValue(i + 1, 0);
                }
                for (auto i = startChannel; i < startChannel + KLS_NUMBEROFLIGHTBULBSPERSECTOR; i++)
                {
                    auto value = std::abs(std::sin((1 + i) * ofGetElapsedTimef() / (ofMap(_intensity, 0, 127, 15, 1))));
                    value += ofMap(_peakEnergy, 0, 10, 0, 1);
                    setDmxValue(i + 1, ofMap(value, 0, 2, _minValue, _maxValue));
                }
            }
            else
            {
                for (auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++)
                {
                    setDmxValue(i + 1, 0);
                }
                for (auto i = startChannel; i < startChannel + KLS_NUMBEROFLIGHTBULBSPERSECTOR * 2; i++)
                {
                    if (i < KLS_LIGHTBULBSCOUNT)
                    {
                        auto value = std::abs(std::sin((1 + i) * ofGetElapsedTimef() / (ofMap(_intensity, 0, 127, 15, 1))));
                        value += ofMap(_peakEnergy, 0, 10, 0, 1);
                        setDmxValue(i + 1, ofMap(value, 0, 2, _minValue, _maxValue));
                    }
                }
            }
        }
        void stop()
        {
            for (auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++)
            {
                setDmxValue(i + 1, 0);
            }
            base::stop();
        }

        void onOscMessage(ofxOscMessage message)
        {
            if (message.getAddress() == "/kls/io/crank")
            {
                _intensity = message.getArgAsInt(0);
            }
            else if (message.getAddress() == "/kls/io/midi2osc")
            {
                if (message.getArgAsString(0) == "noteon")
                {
                    // TODO: match channel
                    int note;
                    if (message.getArgType(2) == OFXOSC_TYPE_INT32)
                    {
                        note = message.getArgAsInt32(2);
                    }
                    else if (message.getArgType(2) == OFXOSC_TYPE_FLOAT)
                    {
                        note = message.getArgAsFloat(2);
                    }
                    _sector = note % 8;
                }
            }
        }

        std::vector<int> _values;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        ofParameter<int> _stillMaxValue;
        ofParameter<float> _speed;

        ofParameter<float> _intensity;
        ofParameter<int> _sector;
        ofParameterGroup _config;
        ofParameter<int> _stillIntensityThreshold;
        ofParameter<int> _peakIntensityThreshold;
        ofParameter<float> _stillSpeed;

        ofParameterGroup _meters;
        ofParameter<float> _peakEnergy;
    };
};
