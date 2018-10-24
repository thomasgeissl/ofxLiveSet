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
            videoGrabber(int deviceId = 0, std::string name = "") : graphic(name)
            {
                _grabber.setDeviceID(deviceId);
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

