#pragma once
#include "ofxLiveSet.h"
#include "../../dmx.config.h"

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

            _values.resize(KLS_LIGHTBULBSCOUNT);
        }

        void update()
        {
            auto timestamp = ofGetElapsedTimeMillis();

            for (auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++)
            {
                // if (timestamp - _timestamps[i] < 1000)
                // {
                //     _values[i] = ofMap(timestamp, _timestamps[i], _timestamps[i] + 1000, _maxValue, _minValue);
                //     setDmxValue(i + 1, _values[i]);
                // }
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

        ofParameter<float> _intensity;
        ofParameter<int> _sector;
    };
};
