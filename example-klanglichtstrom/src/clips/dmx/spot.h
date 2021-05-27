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
        spot() : dmx(), _blackout(false)
        {
            _name = "spot";
            _active.setName("spot");
        
            addParameter(_value.set("value", 0, 0, 255));
            addParameter(_blackoutTrigger.set("blackout"));
            _active.addListener(this, &spot::onActiveChange);
            _value.addListener(this, &spot::onValueChange);
            _blackoutTrigger.addListener(this, &spot::onBlackoutTriggered);
        }
        
        void update(){}
        void onBlackoutTriggered(){
            _blackout = true;
            setValue(KSL_SPOTLIGHT_CHANNEL, 0);
        }
        void onActiveChange(bool & value){
            _blackout = false;
            if(value){
                setValue(40, _value);
            }else{
                setValue(40, 0);
            }
        }
        void onValueChange(int & value){
            _blackout = false;
            if(!_active || _blackout) return;
            setValue(KSL_SPOTLIGHT_CHANNEL, _value);
        }

        ofParameter<int> _value;
        ofParameter<void> _blackoutTrigger;
        bool _blackout;
    };
};
