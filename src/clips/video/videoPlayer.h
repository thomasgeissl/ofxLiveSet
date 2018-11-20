#pragma once
#include "ofMain.h"
#include "./graphic.h"

namespace ofxLiveSet
{
    namespace clip
    {
        class videoPlayer : public graphic
        {
        public:
            videoPlayer(int deviceId = 0, std::string name = "") : graphic(name){
                _openFileChooser.set("openFileChooser");
                _openFileChooser.addListener(this, &videoPlayer::onOpenFileChooser);
                _parameters.add(_openFileChooser);
            }
            void update(){
                _player.update();
                _fbo.begin();
                // if (_grabber.isFrameNew()){
                //     ofClear(255,0);
                //     _grabber.draw(0,0);
                // }
                _fbo.end();
            }
            void onOpenFileChooser(){

            }
            ofVideoPlayer _player;

            ofParameter<void> _openFileChooser;
            ofParameter<bool> _loop;
        };
    }; // namespace clip
}; // namespace ofxLiveSet