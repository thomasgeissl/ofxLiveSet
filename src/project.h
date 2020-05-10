#pragma once
#include "ofMain.h"
#include "session.h"

namespace ofxLiveSet
{
class project
{
public:
	typedef std::shared_ptr<project> pointer;
	static pointer create()
	{
		return std::make_shared<project>();
	}
	project();
	void addSession(session::pointer session);
	std::vector<session::pointer> getSessions();
	void save(std::string path);
	void load(std::string path);

	std::vector<session::pointer> _sessions;
	session::pointer _session;
	ofParameterGroup _parameters;
	ofParameter<std::string> _name;
};
}; // namespace ofxLiveSet
