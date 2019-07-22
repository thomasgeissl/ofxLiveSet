#pragma once
#include "../config.h"
#ifdef OFXLIVESET_USE_LUA

#include "./base.h"
#include "ofxLua.h"

namespace ofxLiveSet {
namespace clip {
    class lua: public ofxLuaListener{
    public:
        lua(){
        }
        void errorReceived(std::string& msg){
            
        }
        ofxLua _lua;
    };
};
};

#endif