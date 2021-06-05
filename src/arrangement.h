#pragma once
#include "ofMain.h"

namespace ofxLiveSet
{
class arrangement
{
public:
	typedef std::shared_ptr<arrangement> pointer;
	static pointer create()
	{
		return std::make_shared<arrangement>();
	}
	arrangement(){}

};
}; // namespace ofxLiveSet