#pragma once
#include "ofMain.h"
#include "./graphic.h"

namespace ofxLiveSet{
    namespace clip{
        class videoGrabber : public graphic{
        public:
            videoGrabber(int deviceId = 0, std::string name = "") : graphic(name){
                _name = name;
                _active.setName(_name);
                _grabber.setDeviceID(deviceId);
                _grabber.initGrabber(640, 480);
            }
            void update(){
                _grabber.update();
                if (_grabber.isFrameNew()){
                    begin();
                    _grabber.draw(-640/2,-480/2);
                    end();
                    setNewFrame();
                }
            }
            ofVideoGrabber _grabber;
            // TODO: parameter device id
        };
    }; // namespace clip
}; // namespace ofxLiveSet

