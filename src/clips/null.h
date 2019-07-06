#pragma once
#include "ofMain.h"
#include "base.h"

namespace ofxLiveSet{
    namespace clip{
        class nullClip : public base{
        public:
            typedef std::shared_ptr<nullClip> pointer;
            static pointer create()
            {
                return std::make_shared<nullClip>();
            }
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

