#pragma once
#include "ofMain.h"
#include "ofxMQTT.h"
#include "./mqttSynchronisable.h"

namespace ofxLiveSet {
//namespace utils {
class mqttSynchroniser {
public:
    void setup(){
        _client.begin("localhost", 1883);
        _client.connect("openframeworks");
    
        ofAddListener(_client.onOnline, this, &mqttSynchroniser::onOnline);
        ofAddListener(_client.onOffline, this, &mqttSynchroniser::onOffline);
        ofAddListener(_client.onMessage, this, &mqttSynchroniser::onMessage);
    }
    void update() {
        _client.update();
    }
    
    void exit(){
        _client.disconnect();
    }
    
    void onOnline(){
        ofLogNotice("mqttSynchroniser") << "successfully connected";
        _client.subscribe("remote/ofxLiveSet/mqttSynchroniser/#");

    }
    
    void onOffline(){
        ofLogNotice("mqttSynchroniser") << "disconnected";
    }
    
    void onMessage(ofxMQTTMessage &msg){
        if(msg.topic == "remote/ofxLiveSet/mqttSynchroniser/request"){
            // TODO: send all parameters
            for(auto item : _synchronisables){
                auto id = item.first;
                auto synchronisable = item.second;
                auto value = id;

                auto voidParameter = dynamic_cast<mqttSynchronisableVoid *>(_synchronisables[value]);
                auto boolParameter = dynamic_cast<mqttSynchronisableBool *>(_synchronisables[value]);
                auto intParameter = dynamic_cast<mqttSynchronisableInt *>(_synchronisables[value]);
                auto floatParameter = dynamic_cast<mqttSynchronisableFloat *>(_synchronisables[value]);
                auto colorParameter = dynamic_cast<mqttSynchronisableColor *>(_synchronisables[value]);

                if(voidParameter != nullptr){
                    ofJson payload;
                    payload["name"] = voidParameter->_parameter.getName();
                    _client.publish(value, payload.dump());
                }else if(boolParameter != nullptr){
                    ofJson payload;
                    payload["value"] = boolParameter->_parameter.get();
                    // payload["min"] = boolParameter->_parameter.getMin();
                    // payload["max"] = boolParameter->_parameter.getMax();
                    payload["name"] = boolParameter->_parameter.getName();
                    _client.publish(value, payload.dump());
                }else if(intParameter != nullptr){
                    ofJson payload;
                    payload["value"] = intParameter->_parameter.get();
                    payload["min"] = intParameter->_parameter.getMin();
                    payload["max"] = intParameter->_parameter.getMax();
                    payload["name"] = intParameter->_parameter.getName();
                    _client.publish(value, payload.dump());
                }else if(floatParameter != nullptr){
                    ofJson payload;
                    payload["value"] = floatParameter->_parameter.get();
                    payload["min"] = floatParameter->_parameter.getMin();
                    payload["max"] = floatParameter->_parameter.getMax();
                    payload["name"] = floatParameter->_parameter.getName();
                    _client.publish(value, payload.dump());
                }else if(colorParameter != nullptr){
                    ofJson payload;
                    payload["red"] = colorParameter->_parameter.get().r;
                    payload["green"] = colorParameter->_parameter.get().g;
                    payload["blue"] = colorParameter->_parameter.get().b;
                    payload["alpha"] = colorParameter->_parameter.get().a;
                    payload["name"] = colorParameter->_parameter.getName();
                    _client.publish(value, payload.dump());
                }
            }
        }else{
            ofJson payload;
            try{
                payload = ofJson::parse(msg.payload);
                std::string remote = "remote/";
                std::string topic = msg.topic;
                auto id = topic.substr(remote.size(), topic.size()-remote.size());
                if(_synchronisables.find(id) != _synchronisables.end()){
                    _synchronisables[id]->set(payload);
                }
            }catch(...){

            }
        }
    }
    void onChangeEvent(std::string & value){
        auto voidParameter = dynamic_cast<mqttSynchronisableVoid *>(_synchronisables[value]);
        auto boolParameter = dynamic_cast<mqttSynchronisableBool *>(_synchronisables[value]);
        auto intParameter = dynamic_cast<mqttSynchronisableInt *>(_synchronisables[value]);
        auto floatParameter = dynamic_cast<mqttSynchronisableFloat *>(_synchronisables[value]);
        auto colorParameter = dynamic_cast<mqttSynchronisableColor *>(_synchronisables[value]);

        if(voidParameter != nullptr){
            ofJson payload;
            payload["name"] = voidParameter->_parameter.getName();
            _client.publish(value, payload.dump());
        }else if(boolParameter != nullptr){
            ofJson payload;
            payload["value"] = boolParameter->_parameter.get();
            // payload["min"] = boolParameter->_parameter.getMin();
            // payload["max"] = boolParameter->_parameter.getMax();
            payload["name"] = boolParameter->_parameter.getName();
            _client.publish(value, payload.dump());
        }else if(intParameter != nullptr){
            ofJson payload;
            payload["value"] = intParameter->_parameter.get();
            payload["min"] = intParameter->_parameter.getMin();
            payload["max"] = intParameter->_parameter.getMax();
            payload["name"] = intParameter->_parameter.getName();
            _client.publish(value, payload.dump());
        }else if(floatParameter != nullptr){
            ofJson payload;
            payload["value"] = floatParameter->_parameter.get();
            payload["min"] = floatParameter->_parameter.getMin();
            payload["max"] = floatParameter->_parameter.getMax();
            payload["name"] = floatParameter->_parameter.getName();
            _client.publish(value, payload.dump());
        }else if(colorParameter != nullptr){
            ofJson payload;
            payload["red"] = colorParameter->_parameter.get().r;
            payload["green"] = colorParameter->_parameter.get().g;
            payload["blue"] = colorParameter->_parameter.get().b;
            payload["alpha"] = colorParameter->_parameter.get().a;
            payload["name"] = colorParameter->_parameter.getName();
            _client.publish(value, payload.dump());
        }
    }

	void addParameter(ofParameter <void> & parameter, std::string id){
        auto synchronisable = new mqttSynchronisableVoid(parameter, id);
	    ofAddListener(synchronisable->getChangedEvent(), this, &mqttSynchroniser::onChangeEvent);
	    _synchronisables[id] = synchronisable;
    }
	void addParameter(ofParameter <bool> & parameter, std::string id){
        auto synchronisable = new mqttSynchronisableBool(parameter, id);
	    ofAddListener(synchronisable->getChangedEvent(), this, &mqttSynchroniser::onChangeEvent);
	    _synchronisables[id] = synchronisable;
    }
	void addParameter(ofParameter <float> & parameter, std::string id){
        auto synchronisable = new mqttSynchronisableFloat(parameter, id);
	    ofAddListener(synchronisable->getChangedEvent(), this, &mqttSynchroniser::onChangeEvent);
	    _synchronisables[id] = synchronisable;
    }
	void addParameter(ofParameter <int> & parameter, std::string id){
        auto synchronisable = new mqttSynchronisableInt(parameter, id);
	    ofAddListener(synchronisable->getChangedEvent(), this, &mqttSynchroniser::onChangeEvent);
	    _synchronisables[id] = synchronisable;
    }
    void addParameter(ofParameter <ofColor> & parameter, std::string id){
        auto synchronisable = new mqttSynchronisableColor(parameter, id);
	    ofAddListener(synchronisable->getChangedEvent(), this, &mqttSynchroniser::onChangeEvent);
	    _synchronisables[id] = synchronisable;
    }


    void addParameters(ofParameterGroup & parameters, std::string prefix){
            for(auto parameter : parameters){
                auto type = parameter->type();
                if(type == "11ofParameterIvE"){
                    auto typedParameter = parameter->cast<void>();
                    addParameter(typedParameter, prefix+typedParameter.getName());
                }else if(type == "11ofParameterIbE"){
                    auto typedParameter = parameter->cast<bool>();
                    addParameter(typedParameter, prefix+typedParameter.getName());
                }else if(type == "11ofParameterIiE"){
                    auto typedParameter = parameter->cast<int>();
                    addParameter(typedParameter, prefix+typedParameter.getName());
                }else if(type == "11ofParameterIfE"){
                    auto typedParameter = parameter->cast<float>();
                    addParameter(typedParameter, prefix+typedParameter.getName());
                }else if(type == "11ofParameterI8ofColor_IhE"){
                    auto typedParameter = parameter->cast<ofColor>();
                    addParameter(typedParameter, prefix+typedParameter.getName());
                }
            }
    }
    
    ofxMQTT _client;
    int _idCounter;
    std::string _sessionId;
    std::map<std::string, mqttSynchronisable*> _synchronisables;
};
//}; // namespace utils
}; // namespace ofxLiveSet
