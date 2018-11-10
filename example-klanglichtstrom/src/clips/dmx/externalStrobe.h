#pragma once
#include "clips/dmx.h"

namespace clips {
    class externalStrobe : public ofxLiveSet::clip::dmx {
    public:
        externalStrobe(int frequencyChannel, int velocityChannel) : dmx() {
            _name = "externalStrobe";
            _frequencyChannel.set("frequencyChannel", frequencyChannel, 0, 511);
            _velocityChannel.set("velocityChannel", velocityChannel, 0, 511);
            _frequency.set("frequency", 0, 0, 255);
            _velocity.set("velocity", 0, 0, 255);
            
            _parameters.add(_frequency);
            _parameters.add(_velocity);
            
            _timestamp = ofGetElapsedTimeMillis();
            
            _active.addListener(this, &externalStrobe::onActiveChange);
            _frequency.addListener(this, &externalStrobe::onValueChange);
            _velocity.addListener(this, &externalStrobe::onValueChange);
        }
        
        void update(){}
        void onActiveChange(bool & value){
            if(value){
                std::pair<int, int> frequency(_frequencyChannel, _frequency);
                _valueChangeEvent.notify(frequency);
                std::pair<int, int> velocity(_velocityChannel, _velocity);
                _valueChangeEvent.notify(velocity);
            }else{
                std::pair<int, int> frequency(_frequencyChannel, 0);
                _valueChangeEvent.notify(frequency);
                std::pair<int, int> velocity(_velocityChannel, 0);
                _valueChangeEvent.notify(velocity);
            }
        }
        void onValueChange(int & value){
            if(!_active) return;
            std::pair<int, int> frequency(_frequencyChannel, _frequency);
            _valueChangeEvent.notify(frequency);
            std::pair<int, int> velocity(_velocityChannel, _velocity);
            _valueChangeEvent.notify(velocity);
        }
        ofParameter<int> _frequencyChannel;
        ofParameter<int> _velocityChannel;
        ofParameter<int> _frequency;
        ofParameter<int> _velocity;
        
        u_int64_t _timestamp;
        int _group;
    };
};
