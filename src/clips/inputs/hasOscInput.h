#pragma once
#include "../base.h"
namespace ofxLiveSet
{
    namespace clip
    {
        class hasOscInput
        {
        public:
            hasOscInput()
            {
            }
            virtual void onOscMessage(ofxOscMessage message){ofLogNotice() << "TODO: override onOscMessage";}
        };
    }; // namespace clip
};     // namespace ofxLiveSet
