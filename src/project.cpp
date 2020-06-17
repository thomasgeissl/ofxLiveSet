#include "./project.h"

ofxLiveSet::project::project() : _session(nullptr)
{
    addSession(session::create());
}
void ofxLiveSet::project::addSession(session::pointer session)
{
    _sessions.push_back(session);
    if (_session)
    {
        _session->stop();
    }
    _session = session;
}
std::vector<ofxLiveSet::session::pointer> ofxLiveSet::project::getSessions()
{
    return _sessions;
}

void ofxLiveSet::project::save(std::string path)
{
    ofLogWarning("project") << "save is not yet implemented";
}
void ofxLiveSet::project::load(std::string path)
{
    ofLogWarning("project") << "load is not yet implemented";
}
