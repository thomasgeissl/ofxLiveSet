#pragma once
#include "clips/dmx.h"

namespace clips {
    class sin : public ofxLiveSet::clip::dmx {
    public:
        sin(int channel) : _channel(channel) {
            _timestamp = ofGetElapsedTimeMillis();
        }
        
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(timestamp - _timestamp < 50){
                return;
            }
            _timestamp = timestamp;
            std::pair<int, int> value(1, ofMap(std::sin(ofGetElapsedTimef()), 0, 1, 0, 255));
            _valueChangeEvent.notify(value);
        }
        
        u_int64_t _timestamp;
        int _channel;
    };
};
