#pragma once
#include "clips/dmx.h"

// strobe clip for stairville ps1500dmx
// https://images.thomann.de/pics/atg/atgdata/document/manual/168875_c_168875_de_online.pdf

namespace clips {
    class strobe : public ofxLiveSet::clip::dmx {
    public:
        typedef std::shared_ptr<strobe> pointer;
        static pointer create(int frequencyChannel, int velocityChannel)
        {
            return std::make_shared<strobe>(frequencyChannel, velocityChannel);
        }
        strobe(int frequencyChannel, int velocityChannel) : dmx(), _frequencyChannel(frequencyChannel), _velocityChannel(velocityChannel) {
            _name = "strobe";
            _active.setName("strobe");
        
            addParameter(_frequency.set("frequency", 15, 15, 255));
            addParameter(_velocity.set("velocity", 15, 15, 255));
            addParameter(_singleShot.set("singleShot"));
            
            _active.addListener(this, &strobe::onActiveChange);
            _frequency.addListener(this, &strobe::onValueChange);
            _velocity.addListener(this, &strobe::onValueChange);
            _singleShot.addListener(this, &strobe::onSingleShot);
        }
        
        void update(){}
        void onActiveChange(bool & value){
            if(value){
                setValue(_frequencyChannel, _frequency);
                setValue(_velocityChannel, _velocity);
            }else{
                setValue(_frequencyChannel, 0);
                setValue(_velocityChannel, 0);
            }
        }
        void onValueChange(int & value){
            if(!_active) return;
            setValue(_frequencyChannel, _frequency);
            setValue(_velocityChannel, _velocity);
        }
        void onSingleShot(){
            // setValue(_frequencyChannel, 16);
            // setValue(_velocityChannel, 255);
        }

        int _frequencyChannel;
        int _velocityChannel;
        ofParameter<int> _frequency;
        ofParameter<int> _velocity;
        ofParameter<void> _singleShot;
        u_int64_t _singleShotTimestamp;
    };
};
