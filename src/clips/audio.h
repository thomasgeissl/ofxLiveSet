#pragma once
#include "ofMain.h"
#include "base.h"

namespace ofxLiveSet {
    namespace clip {
        class audio : public base {
            public:
                audio() {

                }
                audio(std::string path) {
                    _player.load(path);
                }
                void update() {
                    ofLogNotice("clip::audio") << "update";
                }
                void start() {
                    ofLogNotice("clip::audio") << "start";
                    _player.play();
                }
                void pause() {
                    ofLogNotice("clip::audio") << "pause";
                }
                void stop() {
                    ofLogNotice("clip::audio") << "stop";
                }
                ofSoundPlayer _player;
                ofParameterGroup _parameters;
                ofParameter<std::string> _name;
        };
    };
};
