#pragma once

#include "ofMain.h"
#include "ofApp.h"

class renderApp : public ofBaseApp{
public:
    renderApp(){}
    void setup(){
        ofSetBackgroundColor(0);
    }
    void update(){
        ofSetWindowTitle("example-graphic render window: "+ofToString((int)(ofGetFrameRate())));
    }
    void draw(){
        if(_session != nullptr){
            _session->getFbo().draw(0,0);
        }
    }
    void exit(){}
    
    void windowResized(int w, int h){
        if(_session != nullptr){
            _session->onWindowResized(w, h);
        }
    }
    void keyPressed(int key){
        switch(key){
            case 'f': ofToggleFullscreen(); break;
        }
    }
    void setSession(ofxLiveSet::session::pointer session){
        _session = session;
    }

    ofxLiveSet::session::pointer _session;
};

