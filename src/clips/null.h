#pragma once
#include "ofMain.h"
#include "base.h"

namespace ofxLiveSet{
    namespace clip{
        class nullClip : public base{
        public:
            nullClip() : base("null"){
                _active.setName("");
                _active.addListener(this, &nullClip::onActiveChange);
            }
            void update(){}
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

