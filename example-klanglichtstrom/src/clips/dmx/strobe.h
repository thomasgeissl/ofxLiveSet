#pragma once
#include "clips/dmx.h"

namespace clips {
    class strobe : public ofxLiveSet::clip::dmx {
    public:
        typedef std::shared_ptr<strobe> pointer;
        static pointer create(int frequencyChannel, int velocityChannel)
        {
            return std::make_shared<strobe>(frequencyChannel, velocityChannel);
        }
        strobe(int frequencyChannel, int velocityChannel) : dmx() {
            _name = "strobe";
            _active.setName("strobe");
            _frequencyChannel.set("frequencyChannel", frequencyChannel, 1, 512);
            _velocityChannel.set("velocityChannel", velocityChannel, 1, 512);
            _frequency.set("frequency", 0, 0, 255);
            _velocity.set("velocity", 0, 0, 255);
            
            _parameters.add(_frequency);
            _parameters.add(_velocity);
            
            _active.addListener(this, &strobe::onActiveChange);
            _frequency.addListener(this, &strobe::onValueChange);
            _velocity.addListener(this, &strobe::onValueChange);
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
    };
};
