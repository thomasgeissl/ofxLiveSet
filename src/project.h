#pragma once
#include "ofMain.h"
#include "session.h"

namespace ofxLiveSet {
    class project {
        public:
            project() : _session(nullptr)
            {
                addSession(new session());
            }
            void addSession(session *session) {
                _sessions.push_back(session);
                _session->stop();
                _session = session;
            }
            void save(std::string path) {

            }
            void load(std::string path) {

            }
            std::vector<session*> _sessions;
            session *_session;
            ofParameterGroup _parameters;
            ofParameter<std::string> _name;
    };
};