#pragma once
#include "ofMain.h"
#include "../clips/base.h"

namespace ofxLiveSet {
    namespace track {
        class base {
            public:
                base() {

                }
                void update() {
                    for(auto clip : _clips) {
                        clip->update();
                    }
                    ofLogNotice("track") << "update";
                }
                void start() {

                }
                void pause() {

                }
                void stop() {

                }
                void addClip(clip::base *clip) {
                    _clips.push_back(clip);
                }
                std::vector<clip::base*> _clips;
                clip::base *_clip;
                ofParameterGroup _parameters;
                ofParameter<std::string> _name;
        };
    };
};
