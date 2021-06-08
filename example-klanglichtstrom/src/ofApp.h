#pragma once

#include "ofMain.h"
#include "ofxApp.h"
#include "ofxDmx.h"
#include "ofxLiveSet.h"
#include "./tracks/mqtt.h"
#include "./visualisation.h"

// out comment if you dont have an enttec usb interface connected
#define SENDDMX

class ofApp : public ofxApp
{
public:
    ofApp();
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofxLiveSet::project::pointer _project;
    ofxLiveSet::session::pointer _session;

    ofxDmx _dmx;
    visualisation _visualisation;
};