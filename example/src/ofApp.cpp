#include "ofApp.h"

ofApp::ofApp(){
	_session = _project._session;
}

void ofApp::setup(){
//    _gui.setup(_session);
    auto firstVisualTrack = _session->addTrack(new ofxLiveSet::track::graphic());
    auto secondVisualTrack = _session->addTrack(new ofxLiveSet::track::graphic());
    //    _session->addTrack(new ofxLiveSet::track::audio());
    //    _session->addTrack(new ofxLiveSet::track::audio());

    _session->_tracks[0]->addClip(new visuals::lines());
    _session->_tracks[0]->addClip(new visuals::rects());
    _session->_tracks[1]->addClip(new ofxLiveSet::clip::graphic("graphic clip 0"));
    _session->_tracks[1]->addClip(new visuals::lines());
    //    _session->_tracks[2]->addClip(new ofxLiveSet::clip::audio("/Users/thomasgeissl/Desktop/untitled_134.mp3"));
    //    _session->_tracks[3]->addClip(new ofxLiveSet::clip::audio("/Users/thomasgeissl/Desktop/untitled_134.mp3"));
    

    firstVisualTrack->trigger(1);
    secondVisualTrack->trigger(1);
    
    
#ifdef USEDMX
    _dmx.connect("tty.usbserial-EN160415");
    _dmx.setChannels(16);
    _dmx.setLevel(1, 255);
    
    ofxLiveSet::track::dmx* firstDmxTrack = (ofxLiveSet::track::dmx*)(_session->addTrack(new ofxLiveSet::track::dmx()));
    ofxLiveSet::track::dmx* secondDmxTrack = (ofxLiveSet::track::dmx*)(_session->addTrack(new ofxLiveSet::track::dmx()));
    
    firstDmxTrack->addClip(new ofxLiveSet::clip::dmx());
    secondDmxTrack->addClip(new ofxLiveSet::clip::dmx());
    
    firstDmxTrack->trigger(0);
    secondDmxTrack->trigger(0);
    
    firstDmxTrack->setup(_dmx);
    secondDmxTrack->setup(_dmx);
#endif

}

void ofApp::exit(){}

void ofApp::update(){
//    _dmx.update();
}

void ofApp::draw(){}

void ofApp::keyPressed(int key){
    switch(key){
        case '0':
            _session->_tracks[0]->setClip(_session->_tracks[0]->_clips[0]);
            break;
        case '1':
            _session->_tracks[0]->setClip(_session->_tracks[0]->_clips[1]);
            break;
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
