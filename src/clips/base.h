#pragma once
#include "ofMain.h"
#include "session.h"

namespace ofxLiveSet {
    namespace clip {
        class base {
            public:
                base() {

                }
                virtual void update() {
                    ofLogNotice("clip") << "update";
                }
                void start() {
                    ofLogNotice("clip") << "start";
                }
                void pause() {
                    ofLogNotice("clip") << "pause";
                }
                void stop() {
                    ofLogNotice("clip") << "stop";
                }
                ofParameterGroup _parameters;
                ofParameter<std::string> _name;
                ofParameter<bool> _active;

                // ofEvent _started;
                // ofEvent _finished;
                // ofEvent _stopped;
        };
    };
};
