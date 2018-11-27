#pragma once
#include "ofMain.h"
#include "base.h"
#include "ofxPDSP.h"

namespace ofxLiveSet
{
namespace clip
{
class audio : public base, public pdsp::Patchable
{
public:
    audio(std::string name = "") : base(name){
	}
};
}; // namespace clip
}; // namespace ofxLiveSet
