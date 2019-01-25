#include "ofApp.h"

ofApp::ofApp() : _session(_project._session){
}

void ofApp::setup(){
    ofSetBackgroundColor(16, 16, 16);
    
    auto audioAnalyserTrack = (ofxLiveSet::track::audio*)(_session->addTrack(new ofxLiveSet::track::audio("audio analyser")));
    auto videoATrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("video A")));
    auto videoBTrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("video B")));
    auto visualATrack = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("visual A")));
    auto visualB = (ofxLiveSet::track::graphic*)(_session->addTrack(new ofxLiveSet::track::graphic("visual B")));

    audioAnalyserTrack->mute();

    videoATrack->addClip(new ofxLiveSet::clip::videoGrabber(0, "camera"))->setup();
    videoATrack->addClip(new ofxLiveSet::clip::videoPlayer("videos/lake_carrier.mov", "lake carrier"), 2)->setup();

    videoBTrack->addClip(new ofxLiveSet::clip::videoGrabber(0, "camera"))->setup();
    videoBTrack->addClip(new ofxLiveSet::clip::videoPlayer("videos/lake_carrier.mov", "lake carrier"), 2)->setup();

    visualATrack->addClip(new clips::cubeWithTrails())->setup();
    visualATrack->addClip(new clips::midiVisualiser())->setup();
    visualATrack->addClip(new clips::beatVisualiser())->setup();

    visualATrack->addClip(new clips::snake())->setup();
    visualATrack->addClip(new clips::gameOfLife())->setup();

    visualATrack->addClip(new clips::unkownPleasures())->setup();
    visualATrack->addClip(new clips::onTheDarkSideOfTheMoon())->setup();

    visualB->addClip(new clips::randomRectangles())->setup();
    visualB->addClip(new clips::bezierVertex(), 3)->setup();
    visualB->addClip(new clips::parametric2dEquation())->setup();

    visualB->addClip(new clips::arcs())->setup();
    visualB->addClip(new clips::circles())->setup();


    visualB->addClip(new ofxLiveSet::clip::slidePlayer("TODO"))->setup();
    visualB->addClip(new clips::untitled_00())->setup();
    visualB->addClip(new clips::untitled_01())->setup();

    visualB->addClip(new clips::randomPolygon())->setup();

    _session->setup();
    _session->setupGui(150);
    _session->setupAudioEngine(2);

    _session->openMidiMapperInPort(0);    
    _session->openOscControlInPort(9000);
    _session->stop();
    _session->_oscControlEnabled = false;

    _drawGui.set("drawGui", true);
    ofSoundStreamSetup(0, 1, this, 44100, _beat.getBufferSize(), 4);

    _renderApp->_session = _session;
}

void ofApp::exit(){
    ofSoundStreamClose();
}

void ofApp::update(){
    ofSetWindowTitle("example-graphic control window: "+ofToString((int)(ofGetFrameRate())));
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
    _session->draw();
    if(_drawGui){
        _session->drawGui();
    }
}

void ofApp::keyPressed(int key){
    switch(key){
        case 'g': _drawGui = !_drawGui; break;
    }
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
void ofApp::dragEvent(ofDragInfo dragInfo){}
void ofApp::gotMessage(ofMessage msg){}

//TODO: check how to integrate in session, and ofxPDSP
void ofApp::audioReceived(float* input, int bufferSize, int nChannels) {
  _beat.audioReceived(input, bufferSize, nChannels);
}