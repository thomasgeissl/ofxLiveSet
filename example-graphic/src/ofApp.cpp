#include "ofApp.h"

ofApp::ofApp() : _session(_project._session){
}

void ofApp::setup(){
    ofSetBackgroundColor(16, 16, 16);
    
    auto allTrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("all")));
    auto leftTrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("left")));
    auto rightTrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("right")));
    auto videoTrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("video")));


    allTrack->addClip(new clips::background())->setup(); 
    leftTrack->addClip(new clips::background())->setup();
    rightTrack->addClip(new clips::background())->setup();

    allTrack->addClip(new clips::randomRectangles())->setup();
    leftTrack->addClip(new clips::randomRectangles())->setup();
    rightTrack->addClip(new clips::randomRectangles())->setup();

    allTrack->addClip(new clips::progressBar())->setup();

    videoTrack->addClip(new ofxLiveSet::clip::videoGrabber())->setup();
    videoTrack->addClip(new ofxLiveSet::clip::videoPlayer())->setup();

    leftTrack->_xPosition = 0;
    leftTrack->_yPosition = 0;
    leftTrack->_width = ofGetWidth()/2;
    rightTrack->_xPosition = ofGetWidth()/2;
    rightTrack->_yPosition = 0;
    rightTrack->_width = ofGetWidth()/2;
    allTrack->_xPosition = 0;
    allTrack->_yPosition = 0;
    allTrack->_width = ofGetWidth();
    videoTrack->_xPosition = 0;
    videoTrack->_yPosition = 0;
    videoTrack->_width = ofGetWidth();
    videoTrack->_height = ofGetHeight();

    _session->setup();
    _session->setupGui();
    _session->openMidiMapperInPort(0);    
    _session->openOscInPort(9000);
    _session->stop();

    _drawGui.set("drawGui", true);
}

void ofApp::exit(){
}

void ofApp::update(){
    ofSetWindowTitle("example-graphic: "+ofToString((int)(ofGetFrameRate())));
}

void ofApp::draw(){
    if(_drawGui){
        _session->drawGui();
    }
}

void ofApp::keyPressed(int key){
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