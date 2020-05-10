#pragma once
#include "../config.h"
#ifdef OFXLIVESET_USE_LUA

#include "./base.h"
// #include "ofxLua.h"
#include <sys/stat.h>
#include <ctime>

namespace ofxLiveSet
{
namespace clip
{

class lua
// : public ofxLuaListener
{
public:
    lua()
    {
        // _lua.addListener(this);
    }
    void setup()
    {
        // _lua.scriptSetup();
    }
    void update()
    {
        _checkScriptCounter = (_checkScriptCounter + 1) % 60;
        if (_checkScriptCounter == 0)
        {
            struct stat fileInfo;
            if (stat(_path.c_str(), &fileInfo) != 0)
            {
                if (fileInfo.st_ctime != _lastModTime)
                {
                    _lastModTime = fileInfo.st_ctime;
                    reload();
                }
            }
        }
        // _lua.scriptUpdate();
    }
    void errorReceived(std::string &msg)
    {
    }
    void open(std::string path)
    {
        struct stat fileInfo;
        if (stat(path.c_str(), &fileInfo) != 0)
        {
            _path = path;
            _lastModTime = fileInfo.st_ctime;
        }
        // _lua.scriptExit();
        // _lua.init();
        // _lua.doScript(path, true);
    }
    void reload()
    {
        // _lua.scriptExit();
        // _lua.init();
        // _lua.doScript(_path, true);
        // _lua.scriptSetup();
    }
    // ofxLua _lua;
    std::string _path;
    std::time_t _lastModTime;
    int _checkScriptCounter;
};
}; // namespace clip
}; // namespace ofxLiveSet

#endif