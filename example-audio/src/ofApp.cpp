#include "ofApp.h"

ofApp::ofApp() : _session(_project._session){
}

void ofApp::setup(){
    ofSetBackgroundColor(16, 16, 16);

    auto allTrack = (ofxLiveSet::track::audio*)(_session->addTrack(new ofxLiveSet::track::audio("all")));
    auto leftTrack = (ofxLiveSet::track::audio*)(_session->addTrack(new ofxLiveSet::track::audio("left")));
    auto rightTrack = (ofxLiveSet::track::audio*)(_session->addTrack(new ofxLiveSet::track::audio("right")));

    rightTrack->addClip(new ofxLiveSet::clip::audioPlayer())->setup();

    _session->setup();
    _session->setupGui();
    // osx: you might need to create an aggregated device
    _session->setupAudioEngine(2);
    _session->openMidiMapperInPort(0);
    _session->openOscInPort(9000);
    _session->stop();

    _drawGui.set("drawGui", true);
}

void ofApp::exit(){
}

void ofApp::update(){
    ofSetWindowTitle("example-audio: "+ofToString((int)(ofGetFrameRate())));
}

void ofApp::draw(){
    if(_drawGui){
        _session->drawGui();
    }
}

void ofApp::keyPressed(int key){
    _session->onKeyPressed(key);
    switch(key){
        case 'g': _drawGui = !_drawGui; break;
    }
}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}