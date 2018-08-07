#pragma once
#include "ofMain.h"
#include "tracks/base.h"

namespace ofxLiveSet {
    class session {
        public:
            session() {
            }
            void update() {
                for(auto track : _tracks) {
                    track->update();
                }
            }
            void start() {

            }
            void pause() {

            }
            void stop() {
                for(auto track : _tracks) {
                    track->stop();
                }
            }
            void addTrack(track::base *track) {
                _tracks.push_back(track);
            }
            std::vector<track::base*> _tracks;
            ofParameterGroup _parameters;
            ofParameter<std::string> _name;
    };
};