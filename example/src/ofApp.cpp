#include "ofApp.h"

ofApp::ofApp()
{
	_session = _project._session;
	_session->addTrack(new ofxLiveSet::track::graphic());
    _session->addTrack(new ofxLiveSet::track::graphic());
//    _session->addTrack(new ofxLiveSet::track::audio());
//    _session->addTrack(new ofxLiveSet::track::audio());
    _session->_tracks[0]->addClip(new visuals::lines());
    _session->_tracks[0]->addClip(new visuals::rects());
    _session->_tracks[1]->addClip(new ofxLiveSet::clip::graphic("graphic clip 0"));
    _session->_tracks[1]->addClip(new visuals::lines());
//    _session->_tracks[2]->addClip(new ofxLiveSet::clip::audio("/Users/thomasgeissl/Desktop/untitled_134.mp3"));
//    _session->_tracks[3]->addClip(new ofxLiveSet::clip::audio("/Users/thomasgeissl/Desktop/untitled_134.mp3"));

    _session->_tracks[0]->setClip(_session->_tracks[0]->_clips[1]);
    _session->_tracks[1]->setClip(_session->_tracks[1]->_clips[1]);
}

void ofApp::setup()
{
//    _gui.setup(_session);
}

//--------------------------------------------------------------
void ofApp::exit()
{
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch(key){
        case '0':
            _session->_tracks[0]->setClip(_session->_tracks[0]->_clips[0]);
            break;
        case '1':
            _session->_tracks[0]->setClip(_session->_tracks[0]->_clips[1]);
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
