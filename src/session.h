#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "tracks/base.h"

namespace ofxLiveSet
{
class session
{
public:
	session()
	{
		ofAddListener(ofEvents().draw, this, &session::onDraw, OF_EVENT_ORDER_AFTER_APP);
		ofAddListener(ofEvents().update, this, &session::onUpdate, OF_EVENT_ORDER_AFTER_APP);
	}
    void setup() {
        auto x = 20;
        auto y = 20;
        for(auto track : _tracks){
            auto panel = new ofxPanel();
            panel->setup(track->_parameters);
            panel->setPosition(x, y);
            x += panel->getWidth();
            _panels.push_back(panel);
        }
    }
    
	void update()
	{
		for (auto track : _tracks)
		{
			track->update();
		}
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
        
        _panel.draw();
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
        auto clip = _tracks[track]->_clips[index];
        if(clip != nullptr) {
            clip->_parameters;
            ofLogNotice() << "show clip gui";
        }
        _panel.setup(clip->_parameters);
    }
	std::vector<track::base *> _tracks;
	ofParameterGroup _parameters;
	ofParameter<std::string> _name;
    
    std::vector<ofxPanel*> _panels;
    ofxPanel _panel;

};
}; // namespace ofxLiveSet
