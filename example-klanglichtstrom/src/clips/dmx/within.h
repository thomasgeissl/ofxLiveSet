#pragma once
#include "ofxLiveSet.h"
#include "../../dmx.config.h"

namespace clips {
    class within : public ofxLiveSet::clip::dmx, public ofxLiveSet::clip::soundReactive{
    public:
        typedef std::shared_ptr<within> pointer;
        static pointer create()
        {
            return std::make_shared<within>();
        }
        within() : ofxLiveSet::clip::dmx(), ofxLiveSet::clip::soundReactive() {
            _name = "within";
            _channel.set("channel", 1, 1, 512);
            _active.setName(_name);

            addParameter(_soundAnalyserId);
            addParameter(_climaxSoundAnalyserId.set("climaxSoundAnalyser", 2, 0, 16));
            addParameter(_start.set("start", 12, 1, 16));
            addParameter(_minValue.set("minValue", 0, 0, 255));
            addParameter(_maxValue.set("maxValue", 0, 0, 255));
            addParameter(_addPeakEnergy.set("addPeakEnergy", false));
            addParameter(_speed.set("speed", .1, 0, 1));
            addParameter(_amount.set("amount", 1, 1, KSL_LIGHTBULBSCOUNT));
            addParameter(_add.set("add"));
            addParameter(_remove.set("remove"));

            _meters.setName("meters");
            _meters.add(_peakEnergy.set("peakEnergy", 0, 0, 5));
            // _parameters.add(_meters);
            
//            _add.newListener([this](){_amount = _amount+1;});
//            _remove.newListener([this](){_amount = _amount-1;});

            
            _add.addListener(this, &within::onAdd);
            _remove.addListener(this, &within::onRemove);

            _climax = false;
        }
        
        void update(){
            if(_climax){
                auto duration = 1000;
                auto timestamp = ofGetElapsedTimeMillis();
                if(timestamp - _climaxTimestamp < duration){
                    for(auto i = 0; i < _amount; i++){
                        setValue(i + 1, ofMap(timestamp - _climaxTimestamp, 0, duration, 255, 0));
                    }
                }else{
                    _climax = false;
                }
            }else{
                for(auto i = 0; i < _amount; i++){
                    auto dmxValue = 0;
                    if(_addPeakEnergy){
                        dmxValue = ofMap(std::abs(std::sin(ofGetElapsedTimef()*10*_speed+0.4*i)) - 0.*_peakEnergy, 0, 1, _minValue, _maxValue, true);
                    }else{
                        dmxValue = ofMap(std::abs(std::sin(ofGetElapsedTimef()*10*_speed+0.4*i)), 0, 1, _minValue, _maxValue);
                    }
                    std::pair<int, int> value((_start+i)%KSL_LIGHTBULBSCOUNT+1, dmxValue);
                    _valueChangeEvent.notify(value);
                }
            }
        }
        void stop(){
            for(auto i = 1; i <= KSL_LIGHTBULBSCOUNT; i++){
                std::pair<int, int> value(i, 0);
                _valueChangeEvent.notify(value);
            }
            base::stop();
        }

        void setPeakEnergy(int analyserId, float value){
            if(analyserId == _soundAnalyserId){
                _peakEnergy = value;
            }
            if(analyserId == _climaxSoundAnalyserId){
                // _climax = true;
                if(value > 0.5){
                    _climaxTimestamp = ofGetElapsedTimeMillis();
                    ofLogNotice() << "TODO: climax";
                    ofLogNotice() << value;
                }
            }
        }

        void onAdd(){
            _amount.set(ofClamp(_amount + 1, _amount.getMin(), _amount.getMax()));
        }
        void onRemove(){
            std::pair<int, int> value((_start+_amount-1) % KSL_LIGHTBULBSCOUNT + 1, 0);
            _valueChangeEvent.notify(value);
            _amount.set(ofClamp(_amount - 1, _amount.getMin(), _amount.getMax()));
        }
        ofParameter<int> _channel;
        ofParameter<int> _start;
        ofParameter<int> _amount;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;
        ofParameter<bool> _addPeakEnergy;
        ofParameter<float> _speed;
        ofParameter<void> _add;
        ofParameter<void> _remove;
        ofParameter<int> _climaxSoundAnalyserId;
        ofParameterGroup _meters;
        ofParameter<float> _peakEnergy;

        bool _climax;
        u_int64_t _climaxTimestamp;
    };
};
