#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "session.h"

namespace ofxLiveSet{
namespace clip{
class base{
public:
    typedef std::shared_ptr<base> pointer;
    static pointer create(std::string name)
    {
        return std::make_shared<base>(name);
    }
    base(std::string name = ""){
        _active.setName(name);
        _active.addListener(this, &base::onActiveChange);
        _parameters.setName("clip");
        _name.set("name", name);
        _active.set("active", false);
        
//        _parameters.add(_name);
        _parameters.add(_active);
	}
    virtual void setup(){}
    void setupGui(){
        //        ofxPanel::setDefaultWidth(200);
        ofxPanel::setDefaultFillColor(ofColor::blue);
        _gui.setup(_parameters);
        _gui.setHeaderBackgroundColor(ofColor::blue);
        _gui.setUseTTF(true);
        _gui.setFillColor(ofColor::red);
    }
	virtual void update(){}
	virtual void draw(){}
	virtual void start(){
        _active = true;
	}
	virtual void pause(){}
	virtual void stop(){
        _active = false;
	}
    void toggle(){
        _active = !_active;
    }
    
    void onActiveChange(bool & value) {
        bool valueToBeNotified = true;
        if(value){
            ofNotifyEvent(_started, valueToBeNotified, this);
            onStart();
        } else {
            ofNotifyEvent(_stopped, valueToBeNotified, this);
            onStop();
        }
    }
    virtual void onStart(){}
    virtual void onStop(){}

    void addParameter(ofAbstractParameter &parameter){
        _parameters.add(parameter);
    }

    std::string getName(){
        return _name;
    }

    bool isActive(){
        return _active;
    }
    void setActive(bool value){
        _active = value;
    }

	ofParameterGroup _parameters;
	ofParameter<std::string> _name;
	ofParameter<bool> _active;

    ofEvent<bool> _started;
    ofEvent<bool> _finished;
    ofEvent<bool> _stopped;
    
    ofxPanel _gui;
};
}; // namespace clip
}; // namespace ofxLiveSet
