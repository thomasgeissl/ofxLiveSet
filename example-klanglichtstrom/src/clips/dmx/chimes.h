#pragma once
#include "clips/dmx.h"
#include "../../dmx.config.h"

namespace clips
{
    class chimes : public ofxLiveSet::clip::dmx
    {
    public:
        typedef std::shared_ptr<chimes> pointer;
        static pointer create()
        {
            return std::make_shared<chimes>();
        }
        chimes() : dmx(), _blackout(false)
        {
            _name = "chimes";
            _active.setName("chimes");

            addParameter(_minValue.set("minValue", 0, 0, 255));
            addParameter(_maxValue.set("maxValue", 255, 0, 255));
            addParameter(_speed.set("speedChimes", .1, 0, 1));
            addParameter(_blackoutTrigger.set("blackout"));

            _active.addListener(this, &chimes::onActiveChange);
            _speed.addListener(this, &chimes::onSpeedChange);
            _minValue.addListener(this, &chimes::onValueChange);
            _maxValue.addListener(this, &chimes::onValueChange);
            _blackoutTrigger.addListener(this, &chimes::onBlackoutTriggered);
        }

        void update()
        {
            if (_blackout)
            {
                setValue(KLS_CHIMESLIGHTBULBCHANNEL, 0);
            }
            else
            {
                setValue(KLS_CHIMESLIGHTBULBCHANNEL, ofMap(std::abs(std::sin(ofGetElapsedTimef() * 10 * _speed)), 0, 1, _minValue, _maxValue));
            }
        }
        void onBlackoutTriggered()
        {
            _blackout = true;
            setValue(KLS_CHIMESLIGHTBULBCHANNEL, 0);
        }
        void onActiveChange(bool &value)
        {
            unblackout();
        }
        void onSpeedChange(float &value)
        {
            unblackout();
        }
        void onValueChange(int &value)
        {
            unblackout();
        }

    private:
        void unblackout(){
            _blackout = false;
        }
        ofParameter<void> _blackoutTrigger;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        ofParameter<float> _speed;
        bool _blackout;
    };
};
