#include "ofApp.h"

ofApp::ofApp() : _project(ofxLiveSet::project::create()){
    _session = _project->_sessions.back();
}

void ofApp::setup(){
    ofSetBackgroundColor(16, 16, 16);

    auto allTrack = ofxLiveSet::track::audio::create("all");
    auto leftTrack = ofxLiveSet::track::audio::create("left");
    auto rightTrack = ofxLiveSet::track::audio::create("right");

    _session->addTrack(allTrack);
    _session->addTrack(leftTrack);
    _session->addTrack(rightTrack);

    rightTrack->addClip(ofxLiveSet::clip::audioPlayer::create())->setup();

    _session->setup();
    _session->setupGui();
    // osx: you might need to create an aggregated device
    _session->setupAudioEngine(2);
    _session->openMidiMapperInPort(0);
    _session->openOscControlInPort(9000);
    _session->stop();
}

void ofApp::exit(){
}

void ofApp::update(){
    ofSetWindowTitle("example-audio: "+ofToString((int)(ofGetFrameRate())));
}

void ofApp::draw(){
    _session->drawGui();
}

void ofApp::keyPressed(int key){
    _session->onKeyPressed(key);
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