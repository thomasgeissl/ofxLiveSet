#pragma once
#include "ofMain.h"
#include "base.h"

namespace ofxLiveSet{
    namespace clip{
        class nullClip : public base{
        public:
            nullClip() : base("name"){
                _active.setName("");
                _active.addListener(this, &nullClip::onActiveChange);

            }
            void draw(){}
            void onActiveChange(bool & value) {
                if(value){
                    _active = false;
                } else {
                }
            }
        };
    }; // namespace clip
}; // namespace ofxLiveSet

