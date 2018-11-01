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
            panel->setup(track->_parameters);
            panel->setPosition(x, y);
            x += panel->getWidth();
            _panels.push_back(panel);
        }
        
        _name.set("name", "session");
        _stop.set("stop", false);
        _solo.set("solo", false);
        _mute.set("mute", false);
        _parameters.add(_name);
        _parameters.add(_stop);
        _parameters.add(_solo);
        _parameters.add(_mute);
        
        _stop.addListener(this, &session::onStop);

        
        auto scenes = 0;
        for(auto track : _tracks){
            scenes = std::max(scenes, (int)(track->_clips.size()));
        }
        _sceneTriggers.resize(scenes);
        for(auto sceneTrigger : _sceneTriggers){
            _parameters.add(sceneTrigger);
            sceneTrigger.addListener(this, &session::onSceneTrigger);
        }
        _scenesPanel.setup(_parameters);
        _scenesPanel.setPosition(ofGetWidth() - _scenesPanel.getWidth(), 0);

//        _mqttSynchroniser.setup();
    }
    
	void update()
	{
		for (auto track : _tracks)
		{
			track->update();
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
        if(clip == nullptr){ return; }
        
        _clipPanel.setup(clip->_parameters);
        _clipPanel.setPosition(0, ofGetHeight()/2);
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
    
    void onStop(bool & value){
        if(!value){ return; }
        _stop = false;
        stop();
    }
	std::vector<track::base *> _tracks;
	ofParameterGroup _parameters;
	ofParameter<std::string> _name;
    ofParameter<bool> _stop;
    ofParameter<bool> _solo;
    ofParameter<bool> _mute;
    std::vector<ofParameter<bool>> _sceneTriggers;

    std::vector<ofxPanel*> _panels;
    ofxPanel _clipPanel;
    ofxPanel _scenesPanel;

//    mqttSynchroniser _mqttSynchroniser;

};
}; // namespace ofxLiveSet
