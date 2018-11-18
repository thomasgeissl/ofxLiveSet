#pragma once
#include "ofMain.h"
#include "base.h"
#include "../clips/graphic.h"

namespace ofxLiveSet
{
namespace track
{
class graphic : public base
{
public:
    graphic(std::string name = "") : base(name)
	{
	}
    virtual void draw(){
        base::draw();
        if(!_mute && _clip) {
            ((ofxLiveSet::clip::graphic *)(_clip))->_fbo.draw(0,0); // TODO: apply gain/alpha
        }
    }
};
}; // namespace track
}; // namespace ofxLiveSet
