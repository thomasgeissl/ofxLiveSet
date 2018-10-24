#include "ofApp.h"

ofApp::ofApp(){
	_session = _project._session;
}

void ofApp::setup(){
//    _gui.setup(_session);
    auto firstGraphicTrack = _session->addTrack(new ofxLiveSet::track::graphic("Graphic 0"));
    auto secondGraphicTrack = _session->addTrack(new ofxLiveSet::track::graphic("Graphic 1"));
    auto thirdGraphicTrack = _session->addTrack(new ofxLiveSet::track::graphic("Graphic 2"));

    //    _session->addTrack(new ofxLiveSet::track::audio());
    //    _session->addTrack(new ofxLiveSet::track::audio());

    firstGraphicTrack->addClip(new clips::lines());
    firstGraphicTrack->addClip(new clips::rects());
    firstGraphicTrack->addClip(new clips::rects());

    secondGraphicTrack->addClip(new clips::lines());
    secondGraphicTrack->addClip(new clips::lines());
    secondGraphicTrack->addClip(new clips::lines());

    thirdGraphicTrack->addClip(new ofxLiveSet::clip::videoGrabber(0));
    //    _session->_tracks[2]->addClip(new ofxLiveSet::clip::audio("/Users/thomasgeissl/Desktop/untitled_134.mp3"));
    //    _session->_tracks[3]->addClip(new ofxLiveSet::clip::audio("/Users/thomasgeissl/Desktop/untitled_134.mp3"));
    

    firstGraphicTrack->trigger(1);
    secondGraphicTrack->trigger(1);
    
    
#ifdef USEDMX
    _dmx.connect("tty.usbserial-EN160415");
    _dmx.setChannels(16);
    _dmx.setLevel(1, 255);
    
    ofxLiveSet::track::dmx* firstDmxTrack = (ofxLiveSet::track::dmx*)(_session->addTrack(new ofxLiveSet::track::dmx("DMX 0")));
    ofxLiveSet::track::dmx* secondDmxTrack = (ofxLiveSet::track::dmx*)(_session->addTrack(new ofxLiveSet::track::dmx("DMW 1")));
    
    firstDmxTrack->addClip(new clips::sin(1));
    secondDmxTrack->addClip(new clips::sin(2));
    firstDmxTrack->addClip(new clips::rand(1));
    secondDmxTrack->addClip(new clips::rand(2));
    firstDmxTrack->trigger(1);
    secondDmxTrack->trigger(0);
    
    firstDmxTrack->setup(_dmx);
    secondDmxTrack->setup(_dmx);
#endif

    auto x = 20;
    auto y = 20;
    for(auto track : _session->_tracks){
        auto panel = new ofxPanel();
        panel->setup(track->_parameters);
        panel->setPosition(x, y);
        x += panel->getWidth();
        _panels.push_back(panel);
    }
}

void ofApp::exit(){}

void ofApp::update(){
//    _dmx.update();
}

void ofApp::draw(){
    for(auto panel : _panels){
        panel->draw();
    }
}

void ofApp::keyPressed(int key){
    switch(key){
        case '0':
            _session->triggerScence(0);
            break;
        case '1':
            _session->triggerScence(1);
            break;
        case '2':
            _session->triggerScence(2);
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
