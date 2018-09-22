#pragma once
#include "ofMain.h"

namespace ofxLiveSet
{
namespace gui
{
class session{
    public:

    session(){
    };
    void setup(ofxLiveSet::session* session){
        _session = session;
        
        ofAddListener(ofEvents().draw, this, &session::onDraw, OF_EVENT_ORDER_AFTER_APP);
        ofAddListener(ofEvents().update, this, &session::onUpdate, OF_EVENT_ORDER_AFTER_APP);
    }
    void update(){
        
    }
    void draw(){
        ofSetColor(ofColor(255,0,0));
        ofDrawRectangle(0,0, ofGetWidth(), ofGetHeight()/10);
        auto trackIndex = 0;
        auto trackWidth = ofGetWidth()/_session->_tracks.size();
        for(auto track : _session->_tracks){
            ofSetColor(ofRandom(255), 0, ofRandom(255));
            ofDrawRectangle(trackIndex*trackWidth, 0, trackWidth, ofGetHeight());
            trackIndex++;
        }
    }
    void onUpdate(ofEventArgs &e)
    {
        update();
    }
    
    void onDraw(ofEventArgs &e)
    {
        draw();
    }
    
    ofxLiveSet::session *_session;
//    std::vector<ofxLiveSet::gui::track> _tracks;
};
};
};
