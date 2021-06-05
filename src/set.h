#pragma once
#include "ofMain.h"
#include "session.h"
#include "arrangement.h"

namespace ofxLiveSet
{
class set
{
public:
	typedef std::shared_ptr<set> pointer;
	static pointer create()
	{
		return std::make_shared<set>();
	}
	set();
    void addSession(session::pointer session);
    std::vector<ofxLiveSet::session::pointer> getSessions();

	std::vector<session::pointer> _sessions;
	session::pointer _session;
	arrangement::pointer _arrangement;
	ofParameter<std::string> _name;
};
}; // namespace ofxLiveSet
