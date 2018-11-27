#pragma once
#include "ofParameter.h"

class mqttSynchroniser;
class mqttSynchronisable {
public:
    mqttSynchronisable(std::string id) : _id(id){}
    void addListener(mqttSynchroniser & listener){

    }
    virtual void set(ofJson payload){}
    ofEvent <std::string> & getChangedEvent(){
        return _changedEvent;
    }    
protected:
    ofEvent<std::string> _changedEvent;
    std::string _id;
    
private:
};


class mqttSynchronisableVoid : public mqttSynchronisable {
public:
    mqttSynchronisableVoid(ofParameter <void> & parameter, std::string id) : mqttSynchronisable(id), _parameter(parameter){
        parameter.addListener(this, &mqttSynchronisableVoid::notify);
    }
    virtual void set(ofJson payload){
        _parameter.trigger();
    }
    void notify(){
        _changedEvent.notify(_id);
    }
    ofParameter <void> _parameter;
};
class mqttSynchronisableBool : public mqttSynchronisable {
public:
    mqttSynchronisableBool(ofParameter <bool> & parameter, std::string id) : mqttSynchronisable(id), _parameter(parameter){
        parameter.addListener(this, &mqttSynchronisableBool::notify);
    }
    virtual void set(ofJson payload){
        try{
            bool value = payload["value"];
            _parameter = value;
        }catch(...){

        }
    }
    void notify(bool & value){
        _changedEvent.notify(_id);
    }
    ofParameter <bool> _parameter;
};
class mqttSynchronisableInt : public mqttSynchronisable {
public:
    mqttSynchronisableInt(ofParameter <int> & parameter, std::string id) : mqttSynchronisable(id), _parameter(parameter){
        parameter.addListener(this, &mqttSynchronisableInt::notify);
    }
    virtual void set(ofJson payload){
        try{
            int value = payload["value"];
            _parameter = value;
        }catch(...){

        }
    }
    void notify(int & value){
        _changedEvent.notify(_id);
    }
    ofParameter <int> _parameter;
};
class mqttSynchronisableFloat : public mqttSynchronisable {
public:
    mqttSynchronisableFloat(ofParameter <float> & parameter, std::string id) : mqttSynchronisable(id), _parameter(parameter){
        parameter.addListener(this, &mqttSynchronisableFloat::notify);
    }
    virtual void set(ofJson payload){
        try{
            float value = payload["value"];
            _parameter = value;
        }catch(...){

        }
    }
    void notify(float & value){
        _changedEvent.notify(_id);
    }
    ofParameter <float> _parameter;
};
class mqttSynchronisableColor : public mqttSynchronisable {
public:
    mqttSynchronisableColor(ofParameter<ofColor> & parameter, std::string id) : mqttSynchronisable(id), _parameter(parameter){
        parameter.addListener(this, &mqttSynchronisableColor::notify);
    }
    virtual void set(ofJson payload){
        try{
            ofColor value = ofColor(payload["red"], payload["green"], payload["blue"], payload["alpha"]);
            _parameter = value;
        }catch(...){

        }
    }
    void notify(ofColor & value){
        _changedEvent.notify(_id);
    }
    ofParameter <ofColor> _parameter;
};