#pragma once
#include "clips/dmx.h"

namespace clips {
    class sines : public ofxLiveSet::clip::dmx {
    public:
        sines() {
            _channel.set("channel", 1);
            _amount.set("amount", 16, 1, 16);

            _parameters.add(_channel);
            _parameters.add(_amount);

            _timestamp = ofGetElapsedTimeMillis();
        }
        
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(timestamp - _timestamp < 50){
                return;
            }
            _timestamp = timestamp;
            for(auto i = 0; i < _amount; i++){
                std::pair<int, int> value(_channel + i, ofMap(std::sin(ofGetElapsedTimef() + 1), 0, 1, 0, 255));
                _valueChangeEvent.notify(value);
            }
      
        }
        
        ofParameter<int> _channel;
        ofParameter<int> _amount;

        
        u_int64_t _timestamp;
    };
};
