#pragma once
#include "ofMain.h"
#include "ofxGui.h"
//#include "mqttSynchroniser.h"
#include "./tracks/base.h"

namespace ofxLiveSet {
class session {
public:
	session()
	{
		ofAddListener(ofEvents().draw, this, &session::onDraw, OF_EVENT_ORDER_AFTER_APP);
		ofAddListener(ofEvents().update, this, &session::onUpdate, OF_EVENT_ORDER_AFTER_APP);
	}
    void setup() {
        auto x = 0;
        auto y = 0;
        for(auto track : _tracks){
            auto panel = new ofxPanel();
            panel->setDefaultWidth(200);
            panel->setDefaultFillColor(ofColor::red);
//            panel->setDefaultBorderColor(ofColor::red);
            panel->setup(track->_parameters);
            panel->setHeaderBackgroundColor(ofColor::red);

            panel->setPosition(x, y);
            panel->setUseTTF(true);
            x += panel->getWidth() + 1; //TODO: get border width
            _panels.push_back(panel);
        }
        
        _name.set("name", "session");
        _active.set("active", false);
        _timestampString.set("time", "0");
        _mute.set("mute", false);
        _parameters.add(_name);
        _parameters.add(_active);
        _parameters.add(_timestampString);
        _parameters.add(_mute);
        
        _active.addListener(this, &session::onActive);

        
        auto scenes = 0;
        for(auto track : _tracks){
            scenes = std::max(scenes, (int)(track->_clips.size()));
        }
        _sceneTriggers.resize(scenes);
        for(auto sceneTrigger : _sceneTriggers){
            _parameters.add(sceneTrigger);
            sceneTrigger.addListener(this, &session::onSceneTrigger);
        }
        _scenesPanel.setDefaultFillColor(ofColor::green);
//        _scenesPanel.setDefaultBorderColor(ofColor::green);
        _scenesPanel.setup(_parameters);
        _scenesPanel.setHeaderBackgroundColor(ofColor::green);

        _scenesPanel.setPosition(ofGetWidth() - _scenesPanel.getWidth(), 0);
        _scenesPanel.setUseTTF(true);

//        _mqttSynchroniser.setup();
        _clipPanel.setFillColor(ofColor::red);
        
        for(auto track : _tracks){
            for(auto clip : track->_clips){
                ofAddListener(clip->_started, this, &session::onClipStarted);
            }
        }
    }
    
	void update(){
        if(_active){
            _timestamp = ofGetElapsedTimeMillis();
            _timestampString = ofToString((int)((_timestamp - _startedTimestamp)/1000));
            for (auto track : _tracks)
            {
                track->update();
            }
        }



//        _mqttSynchroniser.update();
	}
	void draw()
	{
		for (auto track : _tracks)
		{
            track->draw();
		}
        
        for(auto panel : _panels){
            panel->draw();
        }
        
        _clipPanel.draw();
        _scenesPanel.draw();
	}
	void onUpdate(ofEventArgs &e)
	{
		update();
	}

	void onDraw(ofEventArgs &e)
	{
		draw();
	}

	void start()
	{
        _startedTimestamp = ofGetElapsedTimeMillis();
        for (auto track : _tracks)
        {
            track->start();
        }
	}
	void pause()
	{
	}
	void stop()
	{
        _timestampString = "";
		for (auto track : _tracks)
		{
			track->stop();
		}
	}
    track::base * addTrack(track::base *track)
	{
		_tracks.push_back(track);
        return track;
	}
    
    void triggerScence(int index){
        for (auto track : _tracks)
        {
            track->stop();
            track->trigger(index);
        }
    }
    
    void showClipGui(int track, int index){
        if(track >= _tracks.size()){ return; }
        if(index >= _tracks[track]->_clips.size()){ return; }

        auto clip = _tracks[track]->_clips[index];
        showClipGui(clip);
    }
    void showClipGui(clip::base * clip){
        if(clip == nullptr){ return; }
        _clipPanel.setDefaultWidth(200);
        _clipPanel.setDefaultFillColor(ofColor::blue);
        //        _clipPanel.setDefaultBorderColor(ofColor::blue);
        _clipPanel.setup(clip->_parameters);
        _clipPanel.setPosition(0, ofGetHeight()/2);
        _clipPanel.setHeaderBackgroundColor(ofColor::blue);
    }
    
    clip::base* getClip(int track, int index){
        if(track >= _tracks.size()){ return nullptr; }
        if(index >= _tracks[track]->_clips.size()){ return nullptr; }
        return _tracks[track]->_clips[index];
    }
    void onSceneTrigger(const void* sender, bool & value) {
        if(!value){ return; }
        auto i = 0;
        for(auto sceneTrigger : _sceneTriggers) {
            if(sceneTrigger){
                sceneTrigger = false;
                triggerScence(i);
            }
            i++;
        }
    }
    
    void onActive(bool & value){
        if(value){
            start();
        }else{
            stop();
        }
    }
    void onClipStarted(const void* sender, bool & value) {
        auto clip = (clip::base *) (sender);
        showClipGui(clip);
        if(!_active){
            _active = true;
        }
    }
	std::vector<track::base *> _tracks;
	ofParameterGroup _parameters;
	ofParameter<std::string> _name;
    ofParameter<bool> _active;
    ofParameter<std::string> _timestampString;
    ofParameter<bool> _mute;
    std::vector<ofParameter<bool>> _sceneTriggers;

    std::vector<ofxPanel*> _panels;
    ofxPanel _clipPanel;
    ofxPanel _scenesPanel;
    
    u_int64_t _timestamp;
    u_int64_t _startedTimestamp;

//    mqttSynchroniser _mqttSynchroniser;

};
}; // namespace ofxLiveSet
