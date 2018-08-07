#pragma once
#include "ofMain.h"
#include "base.h"

namespace ofxLiveSet {
    namespace clip {
        class graphic : public base {
            public:
                graphic() {
                    _fbo.allocate(1920, 1080);
                }
                void update() {
                    ofLogNotice("clip::graphic") << "update";
                    _fbo.begin();
                    _fbo.end();
                }
                void start() {
                    ofLogNotice("clip::graphic") << "start";
                }
                void pause() {
                    ofLogNotice("clip::grapphic") << "pause";
                }
                void stop() {
                    ofLogNotice("clip::graphic") << "stop";
                }
                ofFbo _fbo;
                ofParameterGroup _parameters;
                ofParameter<std::string> _name;
        };
    };
};
