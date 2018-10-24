#pragma once
#include "ofMain.h"
#include "./graphic.h"

namespace ofxLiveSet
{
    namespace clip
    {
        class videoGrabber : public graphic
        {
        public:
            videoGrabber(std::string name = "") : base(name)
            {
                _fbo.allocate(1920, 1080);
            }
            void update()
            {
                _grabber.update();
                _fbo.begin();
                if (_grabber.isFrameNew()){
                    _grabber.draw(0,0);
                }
                _fbo.end();
            }
            ofVideoGrabber _grabber;
        };
    }; // namespace clip
}; // namespace ofxLiveSet

