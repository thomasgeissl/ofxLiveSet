#include "ofApp.h"

ofApp::ofApp() : _session(_project._session){
}

void ofApp::setup(){
    ofSetBackgroundColor(16, 16, 16);
    
    auto videoATrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("video A")));
    auto videoBTrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("video B")));
    auto reactiveTrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("reactive")));
    auto leftTrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("left")));
    auto rightTrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("right")));

    leftTrack->addClip(new clips::randomRectangles())->setup();
    rightTrack->addClip(new clips::randomRectangles())->setup();

    rightTrack->addClip(new clips::cubeWithTrails())->setup();

    leftTrack->addClip(new clips::bezierVertex(), 2)->setup();
    rightTrack->addClip(new clips::bezierVertex())->setup();
    rightTrack->addClip(new clips::parametric2dEquation())->setup();

    reactiveTrack->addClip(new clips::cubeWithTrails())->setup();
    reactiveTrack->addClip(new clips::midiVisualiser())->setup();
    reactiveTrack->addClip(new clips::beatVisualiser())->setup();

    videoATrack->addClip(new ofxLiveSet::clip::videoGrabber(0, "camera"), 5)->setup();
    videoATrack->addClip(new ofxLiveSet::clip::videoPlayer("videos/lake_carrier.mov", "lake carrier"))->setup();
    videoBTrack->addClip(new ofxLiveSet::clip::videoGrabber(0, "camera"), 5)->setup();
    videoBTrack->addClip(new ofxLiveSet::clip::videoPlayer("videos/lake_carrier.mov", "lake carrier"))->setup();

    leftTrack->_xPosition = 0;
    leftTrack->_yPosition = 0;
    leftTrack->_width = ofGetWidth()/2;
    rightTrack->_xPosition = ofGetWidth()/2;
    rightTrack->_yPosition = 0;
    rightTrack->_width = ofGetWidth()/2;

    _session->setup();
    _session->setupGui();
    _session->openMidiMapperInPort(0);    
    _session->openOscControlInPort(9000);
    _session->stop();
    _session->_oscControlEnabled = false;

    _drawGui.set("drawGui", true);
    ofSoundStreamSetup(0, 1, this, 44100, _beat.getBufferSize(), 4);

}

void ofApp::exit(){
    ofSoundStreamClose();
}

void ofApp::update(){
    ofSetWindowTitle("example-graphic: "+ofToString((int)(ofGetFrameRate())));
    _beat.update(ofGetElapsedTimeMillis());
    auto kick = _beat.kick();
    auto snare = _beat.snare();
    auto hihat = _beat.hihat();
    for(auto track : _session->_tracks){
        auto beatReactiveClip = dynamic_cast<clips::beatReactive *>(track->_clip);
        if(beatReactiveClip != nullptr){
            beatReactiveClip->setKick(kick);
            beatReactiveClip->setSnare(snare);
            beatReactiveClip->setHihat(hihat);
        }
    }
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
void ofApp::dragEvent(ofDragInfo dragInfo){}
void ofApp::gotMessage(ofMessage msg){}

//TODO: check how to integrate in session, and ofxPDSP
void ofApp::audioReceived(float* input, int bufferSize, int nChannels) {
  _beat.audioReceived(input, bufferSize, nChannels);
}