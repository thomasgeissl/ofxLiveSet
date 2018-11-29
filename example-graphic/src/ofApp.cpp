#include "ofApp.h"

ofApp::ofApp() : _session(_project._session){
}

void ofApp::setup(){
    ofSetBackgroundColor(16, 16, 16);
    
    auto videoTrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("video")));
    auto allTrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("all")));
    auto leftTrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("left")));
    auto rightTrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("right")));

    allTrack->addClip(new clips::randomRectangles())->setup();
    leftTrack->addClip(new clips::randomRectangles())->setup();
    rightTrack->addClip(new clips::randomRectangles())->setup();

    rightTrack->addClip(new clips::cubeWithTrails())->setup();
    allTrack->addClip(new clips::cubeWithTrails())->setup();

    leftTrack->addClip(new clips::bezierVertex(), 2)->setup();
    rightTrack->addClip(new clips::bezierVertex())->setup();
    rightTrack->addClip(new clips::parametric2dEquation())->setup();

    allTrack->addClip(new clips::progressBar())->setup();
    allTrack->addClip(new clips::midiVisualiser())->setup();
    allTrack->addClip(new clips::beatVisualiser())->setup();

    videoTrack->addClip(new ofxLiveSet::clip::videoGrabber(0, "camera"), 5)->setup();
    videoTrack->addClip(new ofxLiveSet::clip::videoPlayer("videos/lake_carrier.mov", "lake carrier"))->setup();

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
    _session->openOscMapperInPort(9000);
    _session->stop();
    _session->_oscControlEnabled = false;

    _drawGui.set("drawGui", true);
    ofSoundStreamSetup(0, 1, this, 44100, _beat.getBufferSize(), 4);

}

void ofApp::exit(){
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