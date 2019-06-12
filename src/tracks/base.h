#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "../clips/base.h"
#include "../clips/null.h"

namespace ofxLiveSet{
namespace track{
class base{
public:
    base(std::string name = "") : _clip(nullptr), _focused(false){
        _parameters.setName(name);
        _name.set("name", name);
        _stop.set("stop");
        _mute.set("mute", false);
        _solo.set("solo", false);
        _gain.set("gain", 1, 0, 1);

        _controls.setName("control");
//        _parameters.add(_name);
        _controls.add(_stop);
        _controls.add(_solo);
        _controls.add(_mute);
        _controls.add(_gain);
        _parameters.add(_controls);

        _clipTriggers.setName("clips");
        _parameters.add(_clipTriggers);

        _stop.addListener(this, &base::onStop);

        auto ioName = name == "" ? "IO" : name+" IO";
        _ioParameters.setName(ioName);
	}

    virtual void setup(){}

    void setupGui(){
        ofxPanel::setDefaultFillColor(ofColor::red);
        _gui.setup(_parameters);
        _gui.setHeaderBackgroundColor(ofColor::red);
        _gui.setUseTTF(true);
        _gui.setFillColor(ofColor::red);

        ofxPanel::setDefaultFillColor(ofColor::red);
        _ioGui.setup(_ioParameters);
        _ioGui.setHeaderBackgroundColor(ofColor::red);
        _ioGui.setUseTTF(true);
        _ioGui.setFillColor(ofColor::red);

        for(auto clip : _clips){
            clip->setupGui();
        }
    }

	virtual void update(){
		if (_clip != nullptr)
		{
            _clip->update();
		}
	}

	virtual void draw(){
		if (_clip)
		{
            _clip->draw();
		}
	}

    void drawGui(){
        if(_focused){
            _gui.setHeaderBackgroundColor(ofColor::purple);
            _ioGui.draw();
        }else{
            _gui.setHeaderBackgroundColor(ofColor::red);
        }
        _gui.draw();
    }

	void start(){
        if (_clip != nullptr){
            _clip->start();
        }
	}

	void stop(){
        if (_clip != nullptr){
            _clip->stop();
        }
	}

    clip::base* addClip(clip::base *clip){
		_clips.push_back(clip);
        _clipTriggers.add(clip->_active);
        ofAddListener(clip->_started, this, &base::onClipStarted);
        ofAddListener(clip->_stopped, this, &base::onClipStopped);

        return clip;
	}

    clip::base* addClip(clip::base *clip, int index){
        while(_clips.size() < index){
            auto nullClip = new clip::nullClip();
            _clipTriggers.add(nullClip->_active);
            _clips.push_back(nullClip);
        }
        _clips.push_back(clip);
        _clipTriggers.add(clip->_active);
        ofAddListener(clip->_started, this, &base::onClipStarted);
        ofAddListener(clip->_stopped, this, &base::onClipStopped);
        
        return clip;
    }

    void setClip(clip::base *clip){
        _clip = clip;
    }

    void trigger(int index){
        stop();
        if(index < _clips.size()){
            _clip = _clips[index];
            if(_clip != nullptr){
                _clip->start();
            }
        }
    }

    void mute(bool value = true){
        _mute = value;
    }

    void onClipStarted(const void* sender, bool & value) {
        for(auto clip : _clips) {
            if(clip != sender) {
                clip->stop();
            }
        }
        _clip = (clip::base *) (sender);
    }

    void onClipStopped(const void* sender, bool & value) {
        if(_clip == sender) {
            _clip->stop();
            _clip = nullptr;
        }
    }

    void onStop() {
        stop();
    }


    void setGain(float value){
        _gain = value;
    }
   
	std::vector<clip::base *> _clips;
	clip::base *_clip;
	ofParameterGroup _parameters;
    ofParameterGroup _controls;
	ofParameter<std::string> _name;
    ofParameter<void> _stop;
    ofParameter<bool> _mute;
    ofParameter<bool> _solo;
    ofParameter<float> _gain;
    ofParameterGroup _clipTriggers;
    
    ofxPanel _gui;
    ofParameterGroup _ioParameters;
    ofxPanel _ioGui;

    bool _focused;
};
}; // namespace track
}; // namespace ofxLiveSet
