#include "set.h"

ofxLiveSet::set::set(){
    addSession(ofxLiveSet::session::create());
}
void ofxLiveSet::set::addSession(session::pointer session)
{
    _sessions.push_back(session);
    if (_session)
    {
        _session->stop();
    }
    _session = session;
}

std::vector<ofxLiveSet::session::pointer> ofxLiveSet::set::getSessions()
{
    return _sessions;
}