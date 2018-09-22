#pragma once
#include "ofMain.h"
#include "base.h"

namespace ofxLiveSet
{
namespace track
{
class audio : public base
{
public:
	audio(std::string name = "") : base(name)
	{
	}
};
}; // namespace track
}; // namespace ofxLiveSet
