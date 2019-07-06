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
    typedef std::shared_ptr<audio> pointer;
    static pointer create(std::string name)
    {
        return std::make_shared<audio>(name);
    }
    audio(std::string name = "") : base(name){
	}
};
}; // namespace clip
}; // namespace ofxLiveSet
