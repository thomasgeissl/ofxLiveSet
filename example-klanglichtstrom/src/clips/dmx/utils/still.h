#pragma once
#include "clips/dmx.h"
#include "../../dmx.config.h"

namespace clips {
    class still : public ofxLiveSet::clip::dmx {
    public:
        typedef std::shared_ptr<still> pointer;
        static pointer create()
        {
            return std::make_shared<still>();
        }
        still() : ofxLiveSet::clip::dmx("still")
        {
            _name ="still";
            _active.setName(_name);
            _values.resize(KLS_LIGHTBULBSCOUNT);
            for(auto i = 0; i < _values.size(); i++) {
                _parameters.add(_values[i].set(ofToString(i), 0, 0, 255));
            }
        }
        
        void update() {
            for(auto i = 0; i < _values.size(); i++) {
                setValue(i + 1, _values[i]);
            }
        }
        
        std::vector<ofParameter<int>> _values;
    };
};
