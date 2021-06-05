#pragma once
#include "ofMain.h"
#include "set.h"
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
    void addSet(set::pointer set);
    std::vector<ofxLiveSet::set::pointer> getSets();
	void save(std::string path);
	void load(std::string path);

	std::vector<set::pointer> _sets;
	set::pointer _set;

	ofParameterGroup _parameters;
	ofParameter<std::string> _name;
};
}; // namespace ofxLiveSet
