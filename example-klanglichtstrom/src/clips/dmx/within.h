#pragma once
#include "./soundReactiveDmx.h"

namespace clips {
    class within : public soundReactiveDmx {
    public:
        within() : soundReactiveDmx() {
            _name = "within";
            _channel.set("channel", 1, 1, 512);
            _start.set("start", 12, 1, 16);
            _amount.set("amount", 1, 1, 16);
            _minValue.set("minValue", 0, 0, 255);
            _maxValue.set("maxValue", 70, 0, 255);
            _addPeakEnergy.set("addPeakEnergy", false);
            _speed.set("speed", .1, 0, 1);
            _add.set("add");
            _remove.set("remove");
            _active.setName(_name);

            _parameters.add(_start);
            _parameters.add(_amount);
            _parameters.add(_minValue);
            _parameters.add(_maxValue);
            _parameters.add(_addPeakEnergy);
            _parameters.add(_speed);
            _parameters.add(_add);
            _parameters.add(_remove);
            
//            _add.newListener([this](){_amount = _amount+1;});
//            _remove.newListener([this](){_amount = _amount-1;});
            
            _add.addListener(this, &within::onAdd);
            _remove.addListener(this, &within::onRemove);

        }
        
        void update(){
            for(auto i = 0; i < _amount; i++){
                auto dmxValue = 0;
                if(_addPeakEnergy){
                    dmxValue = ofMap(std::abs(std::sin(ofGetElapsedTimef()*10*_speed+0.4*i)) + _peakEnergy, 0, 2, _minValue, _maxValue);

                }else{
                    dmxValue = ofMap(std::abs(std::sin(ofGetElapsedTimef()*10*_speed+0.4*i)), 0, 1, _minValue, _maxValue);
                }
                std::pair<int, int> value((_start+i)%16+1, dmxValue);
                _valueChangeEvent.notify(value);

            }
            
        }
        void stop(){
            for(auto i = 1; i <= 16; i++){
                std::pair<int, int> value(i, 0);
                _valueChangeEvent.notify(value);
            }
            base::stop();
        }
        void setPeakEnergy(int analyserId, float value){
            if(analyserId != _soundAnalyserId){return;}
            _peakEnergy = value;
        }

        void onAdd(){
            _amount.set(ofClamp(_amount + 1, _amount.getMin(), _amount.getMax()));
        }
        void onRemove(){
            std::pair<int, int> value((_start+_amount-1) % 16 + 1, 0);
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
        
        float _peakEnergy;
    };
};
