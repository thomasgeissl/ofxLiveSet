#include "ofApp.h"

ofApp::ofApp(){
	_session = _project._session;
}

void ofApp::setup(){
//    auto firstGraphicTrack = _session->addTrack(new ofxLiveSet::track::graphic("Graphic 0"));
//    auto secondGraphicTrack = _session->addTrack(new ofxLiveSet::track::graphic("Graphic 1"));
//    auto thirdGraphicTrack = _session->addTrack(new ofxLiveSet::track::graphic("Graphic 2"));

    //    _session->addTrack(new ofxLiveSet::track::audio());
    //    _session->addTrack(new ofxLiveSet::track::audio());

//    firstGraphicTrack->addClip(new clips::lines());
//    firstGraphicTrack->addClip(new clips::rects());
//    firstGraphicTrack->addClip(new clips::rects());
//
//    secondGraphicTrack->addClip(new clips::lines());
//    secondGraphicTrack->addClip(new clips::lines());
//    secondGraphicTrack->addClip(new clips::lines());

//    thirdGraphicTrack->addClip(new ofxLiveSet::clip::videoGrabber(0));
    //    _session->_tracks[2]->addClip(new ofxLiveSet::clip::audio("/Users/thomasgeissl/Desktop/untitled_134.mp3"));
    //    _session->_tracks[3]->addClip(new ofxLiveSet::clip::audio("/Users/thomasgeissl/Desktop/untitled_134.mp3"));
    
    
    ofAddListener(_soundAnalyser._peakEnergyEvent, this, &ofApp::onPeakEnergy);
    _soundAnalyser.setup();
    
#ifdef USEDMX
    _dmx.connect("tty.usbserial-EN160415");
    _dmx.setChannels(16);
    _dmx.setLevel(1, 255);
    
    ofxLiveSet::track::dmx* firstDmxTrack = (ofxLiveSet::track::dmx*)(_session->addTrack(new ofxLiveSet::track::dmx("DMX 0")));
//    ofxLiveSet::track::dmx* secondDmxTrack = (ofxLiveSet::track::dmx*)(_session->addTrack(new ofxLiveSet::track::dmx("DMX 1")));
    
    firstDmxTrack->addClip(new clips::sin(1));
    firstDmxTrack->addClip(new clips::peak());
    firstDmxTrack->addClip(new clips::rand(1));
    firstDmxTrack->addClip(new clips::sines());
    firstDmxTrack->addClip(new clips::frozen());

//    secondDmxTrack->addClip(new clips::sin(2));
//    secondDmxTrack->addClip(new clips::rand(2));
    
    firstDmxTrack->setup(_dmx);
//    secondDmxTrack->setup(_dmx);
#endif
    _session->setup();
    _session->triggerScence(1);
//    secondDmxTrack->stop();
}

void ofApp::exit(){}

void ofApp::update(){
    _soundAnalyser.update();
}

void ofApp::draw(){

}

void ofApp::keyPressed(int key){
    switch(key){
        case '0':
            _session->showClipGui(0,0);
            break;
        case '1':
            _session->showClipGui(0,1);
            break;
        case '2':
            _session->showClipGui(0,2);
            break;
        case '3':
            _session->showClipGui(0,3);
            break;
        case '4':
            _session->showClipGui(0,4);
            break;
        case 'b':
            ((clips::peak *)(_session->_tracks[0]->_clips[1]))->bang(1);
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
void ofApp::onPeakEnergy(std::pair<int, float> & value){
    ofLogNotice()<<"peak "<<value.first<<" "<<value.second;
    if(value.first == 1 && value.second > .5){
        ((clips::peak *)(_session->_tracks[3]->_clips[1]))->bang(value.second);
    }
}

