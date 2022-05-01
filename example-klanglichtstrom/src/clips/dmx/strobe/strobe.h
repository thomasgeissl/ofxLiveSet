#pragma once
#include "clips/dmx.h"
#include "../../../dmx.config.h"

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
            _frequencyChannel = KLS_STROBECHANNEL;
            _velocityChannel = KLS_STROBECHANNEL + 1;
        }
        
        void update(){
            if(!_active) return;
            auto duration = ofGetElapsedTimeMillis() - _singleShotTimestamp;
            if(duration > 1000 && duration < 2000){
                _frequency = _frequency.getMin();
            }
        }
        void onActiveChange(bool & value){
            if(value){
                setDmxValue(_frequencyChannel, _frequency);
                setDmxValue(_velocityChannel, _velocity);
            }else{
                setDmxValue(_frequencyChannel, 0);
                setDmxValue(_velocityChannel, 0);
            }
        }
        void onValueChange(int & value){
            if(!_active) return;
            setDmxValue(_frequencyChannel, _frequency);
            setDmxValue(_velocityChannel, _velocity);
        }
        void onSingleShot(){
            _singleShotTimestamp = ofGetElapsedTimeMillis();
            _frequency = 16;
            if(_velocity == _velocity.getMin()){
                _velocity++;
            }
        }

        int _frequencyChannel;
        int _velocityChannel;
        ofParameter<int> _frequency;
        ofParameter<int> _velocity;
        ofParameter<void> _singleShot;
        u_int64_t _singleShotTimestamp;
    };
};
