#pragma once
#include "clips/dmx.h"
#include "../../dmx.config.h"

namespace clips
{
    class spares : public ofxLiveSet::clip::dmx
    {
    public:
        typedef std::shared_ptr<spares> pointer;
        static pointer create()
        {
            return std::make_shared<spares>();
        }
        spares() : ofxLiveSet::clip::dmx("spare sockets")
        {
            _name = "spare sockets";
            _active.setName(_name);
            _values.resize(3);
            for (auto i = 0; i < _values.size(); i++)
            {
                _parameters.add(_values[i].set(ofToString(i), 0, 0, 255));
            }
            _parameters.add(_masterValue.set("master value", 0, 0, 255));
            _masterValue.addListener(this, &spares::onMasterValueChange);
        }

        void update()
        {
            for (auto i = 0; i < _values.size(); i++)
            {
                setValue(KLS_CHIMESLIGHTBULBCHANNEL + i + 1, _values[i]);
            }
        }
        void stop()
        {
            base::stop();
        }

        void onMasterValueChange(int &value)
        {
            for (auto i = 0; i < _values.size(); i++)
            {
                _values[i] = value;
            }
        }

        std::vector<ofParameter<int>> _values;
        ofParameter<int> _masterValue;
    };
};
