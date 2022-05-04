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
            addParameter(_maxValue.set("maxValue", 100, 0, 255));
            addParameter(_speed.set("speed", 0, 0, 1));

            _config.setName("config");
            _config.add(_stillIntensityThreshold.set("still intensity", 8, 0, 127));
            _parameters.add(_config);
            _values.resize(KLS_LIGHTBULBSCOUNT);
        }

        void update()
        {
            auto timestamp = ofGetElapsedTimeMillis();

            auto startChannel = _sector * KLS_NUMBEROFLIGHTBULBSPERSECTOR;
            if (_intensity < _stillIntensityThreshold)
            {
                // TODO: maybe rain clip, or any other subtle clip
            }
            else
            {
                for (auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++)
                {
                    setDmxValue(i + 1, 0);
                }
                for (auto i = startChannel; i < startChannel + KLS_NUMBEROFLIGHTBULBSPERSECTOR; i++)
                {
                    setDmxValue(i + 1, std::abs(std::sin((1 + i) * ofGetElapsedTimef() / (10 + _speed * 13))) * _maxValue);
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
        ofParameter<float> _speed;

        ofParameter<float> _intensity;
        ofParameter<int> _sector;
        ofParameterGroup _config;
        ofParameter<int> _stillIntensityThreshold;
    };
};
