#pragma once
#include "clips/dmx.h"
#include "../../dmx.config.h"

// spot clip for stairville ps1500dmx

namespace clips {
    class spot : public ofxLiveSet::clip::dmx {
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
        
            addParameter(_value.set("value", 0, 0, 255));
            addParameter(_speed.set("speed", 0, 0, 255));
            addParameter(_blackoutTrigger.set("blackout"));
            _active.addListener(this, &spot::onActiveChange);
            _value.addListener(this, &spot::onValueChange);
            _speed.addListener(this, &spot::onSpeedChange);
            _blackoutTrigger.addListener(this, &spot::onBlackoutTriggered);
        }
        
        void update(){}
        void onBlackoutTriggered(){
            _value = 0;
            _speed = 0;
        }
        void onActiveChange(bool & value){
            if(value){
                setValue(KLS_SPOTLIGHT_CHANNEL, _value);
                setValue(KLS_RAINMAKERMOTORCHANNEL, _speed);
            }else{
                setValue(KLS_SPOTLIGHT_CHANNEL, 0);
                setValue(KLS_RAINMAKERMOTORCHANNEL, 0);
            }
        }
        void onValueChange(int & value){
            setValue(KLS_SPOTLIGHT_CHANNEL, _value);
        }
        void onSpeedChange(int & value){
            setValue(KLS_RAINMAKERMOTORCHANNEL, _speed);
        }

        ofParameter<int> _value;
        ofParameter<int> _speed;
        ofParameter<void> _blackoutTrigger;
    };
};
