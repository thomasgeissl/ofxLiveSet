#pragma once
#include "clips/dmx.h"
#include "../../dmx.config.h"

// spot clip for stairville ps1500dmx

namespace clips
{
    class spot : public ofxLiveSet::clip::dmx
    {
    public:
        typedef std::shared_ptr<spot> pointer;
        static pointer create()
        {
            return std::make_shared<spot>();
        }
        spot() : dmx()
        {
            _name = "rainmaker";
            _active.setName("rainmaker");

            addParameter(_value.set("spot", 0, 0, 255));
            addParameter(_speed.set("speed", 0, 0, 255));
            addParameter(_fadeout.set("fadeout", false));
            addParameter(_fadeoutTime.set("fadeoutTime", 30, 1, 120));
            addParameter(_blackoutTrigger.set("blackout"));
            _active.addListener(this, &spot::onActiveChange);
            _value.addListener(this, &spot::onValueChange);
            _speed.addListener(this, &spot::onSpeedChange);
            _blackoutTrigger.addListener(this, &spot::onBlackoutTriggered);
            //ausfade trigger
        }

     

        void update()
        {
            if (_value > 0 && _fadeout && ofGetFrameNum() % (int)(ofGetFrameRate()/2) == 0)
            {
                _value--;
            }
            else if (_value < 0 && _fadeout)
            {
                _fadeout = false;
                _value = 0;
            }
        }
        void onBlackoutTriggered()
        {
            _value = 0;
            _speed = 0;
        }
        void onActiveChange(bool &value)
        {
            if (value)
            {
                setDmxValue(KLS_SPOTLIGHT_CHANNEL, _value);
                setDmxValue(KLS_RAINMAKERMOTORCHANNEL, _speed);
            }
            else
            {
                setDmxValue(KLS_SPOTLIGHT_CHANNEL, 0);
                setDmxValue(KLS_RAINMAKERMOTORCHANNEL, 0);
            }
        }
        void onValueChange(int &value)
        {
            setDmxValue(KLS_SPOTLIGHT_CHANNEL, _value);
        }
        void onSpeedChange(int &value)
        {
            setDmxValue(KLS_RAINMAKERMOTORCHANNEL, _speed);
        }

        ofParameter<int> _value;
        ofParameter<int> _speed;
        ofParameter<void> _blackoutTrigger;
        ofParameter<bool> _fadeout;
        ofParameter<int> _fadeoutTime;
    };
};
