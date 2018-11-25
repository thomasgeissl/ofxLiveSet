#pragma once

#include "ofMain.h"
#include "ofxLiveSet.h"
#include "ofxBeat.h"
#include "./clips/graphic/randomRectangles.h"
#include "./clips/graphic/progressBar.h"
#include "./clips/graphic/cubeWithTrails.h"
#include "./clips/graphic/bezierVertex.h"
#include "./clips/graphic/midiVisualiser.h"
#include "./clips/graphic/beatVisualiser.h"

class ofApp : public ofBaseApp{
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
    void audioReceived(float* input, int bufferSize, int nChannels);
    
    ofxLiveSet::project _project;
    ofxLiveSet::session *_session;
    ofxBeat _beat;

    ofParameterGroup _parameters;
    ofParameter<bool> _drawGui;
};

