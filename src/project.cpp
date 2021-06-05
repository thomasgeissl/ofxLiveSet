#include "./project.h"

ofxLiveSet::project::project() : _set(nullptr)
{
    addSet(set::create());
}

void ofxLiveSet::project::addSet(set::pointer set)
{
    _sets.push_back(set);
    if (_set)
    {
        // _set->stop();
    }
    _set = set;
}

std::vector<ofxLiveSet::set::pointer> ofxLiveSet::project::getSets()
{
    return _sets;
}

void ofxLiveSet::project::save(std::string path)
{
    ofLogWarning("project") << "save is not yet implemented";
}
void ofxLiveSet::project::load(std::string path)
{
    ofLogWarning("project") << "load is not yet implemented";
}
