#pragma once
#include "clips/dmx.h"
#include "../../dmx.config.h"

namespace clips
{
    class still : public ofxLiveSet::clip::dmx
    {
    public:
        typedef std::shared_ptr<still> pointer;
        static pointer create()
        {
            return std::make_shared<still>();
        }
        still() : ofxLiveSet::clip::dmx("debug")
        {
            _name = "debug";
            _active.setName(_name);
            _values.resize(KLS_LIGHTBULBSCOUNT);
            addParameter(_spotValue.set("spot", 0, 0, 255));
            for (auto i = 0; i < _values.size(); i++)
            {
                _parameters.add(_values[i].set(ofToString(i), 0, 0, 255));
            }
            _parameters.add(_masterValue.set("master value", 0, 0, 255));
            _masterValue.addListener(this, &still::onMasterValueChange);
        }

        void update()
        {
            for (auto i = 0; i < _values.size(); i++)
            {
                setDmxValue(i + 1, _values[i]);
            }
            setDmxValue(KLS_SPOTLIGHT_CHANNEL, _spotValue);
        }
        void stop()
        {
            for (auto i = 1; i <= KLS_LIGHTBULBSCOUNT; i++)
            {
                setDmxValue(i, 0);
            }
            setDmxValue(KLS_SPOTLIGHT_CHANNEL, 0);
            base::stop();
        }

        void onMasterValueChange(int &value)
        {
            for (auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++)
            {
                _values[i] = value;
            }
        }

        std::vector<ofParameter<int>> _values;
        ofParameter<int> _masterValue;
        ofParameter<int> _spotValue;
    };
};
