#pragma once
#include "ofMain.h"
#include "./graphic.h"

namespace ofxLiveSet{
    namespace clip{
        class videoGrabber : public graphic{
        public:
            videoGrabber(int deviceId = 0, std::string name = "") : graphic(name){
                _grabber.setDeviceID(deviceId);
                _grabber.initGrabber(640, 480);
            }
            void update(){
                _grabber.update();
                beginFboWithShaderIfActive();
                if (_grabber.isFrameNew()){
                    ofClear(255,0);
                    ofTranslate(ofMap(_x, 0, 1, 0, _width), ofMap(_y, 0, 1, 0, _height));
                    ofScale(ofMap(_scale, 0, 1, 0, 2), ofMap(_scale, 0, 1, 0, 2), 0);
                    _grabber.draw(-640/2,-480/2);

                    setNewFrame();
                }
                endFboWithShaderIfActive();
            }
            ofVideoGrabber _grabber;
        };
    }; // namespace clip
}; // namespace ofxLiveSet

