#include "ofApp.h"

ofApp::ofApp() : _session(_project._session){
}

void ofApp::setup(){
    ofSetBackgroundColor(16, 16, 16);

    _dmx.connect("/dev/cu.usbserial-EN160415");
    _dmx.setChannels(18);
    
    auto lightBulbsTrack = ofxLiveSet::track::dmx::create("light bulbs");
    auto strobeTrack = ofxLiveSet::track::dmx::create("strobe");
    auto hazerTrack = ofxLiveSet::track::dmx::create("hazer");
    auto utilsTrack = ofxLiveSet::track::dmx::create("utils");
    auto afterShowTrack = ofxLiveSet::track::dmx::create("after show");

    lightBulbsTrack->addClip(clips::within::create())->setup();
    lightBulbsTrack->addClip(clips::anchor::create())->setup();
    lightBulbsTrack->addClip(clips::schwanensee::create())->setup();
    lightBulbsTrack->addClip(clips::firn::create())->setup();
    lightBulbsTrack->addClip(clips::wind::create())->setup();

    strobeTrack->addClip(clips::strobe::create(17, 18), 3)->setup();

    utilsTrack->addClip(clips::still::create(), 7)->setup();
    utilsTrack->addClip(clips::midi2dmx::create())->setup();
    
    utilsTrack->mute();
    afterShowTrack->mute();

    _session->addTrack(lightBulbsTrack);
    _session->addTrack(strobeTrack);
    _session->addTrack(hazerTrack);
    _session->addTrack(utilsTrack);
    _session->addTrack(afterShowTrack);
    
    for(auto track : _session->_tracks){
        if(auto dmxTrack = std::dynamic_pointer_cast<ofxLiveSet::track::dmx>(track)){
            dmxTrack->setup(&_dmx);
        }
    }

    _session->setup();
    _session->setupGui();
    _session->openOscControlInPort(9000);
    _session->openMidiMapperInPort(0);
    _session->stop();
    _session->_mqttSynchroniserEnabled = false;

    _session->renameScene(0, "within");
    _session->renameScene(1, "anchor");
    _session->renameScene(2, "schwanensee");
    _session->renameScene(3, "firn");
    _session->renameScene(4, "wind");
    _session->_sceneInformation[0]._text = "";
    _session->_sceneInformation[1]._text = "activate analyser\non beats track\nin ableton live";
    _session->_sceneInformation[2]._text = "";
    _session->_sceneInformation[3]._text = "";
}

void ofApp::exit(){
    for(auto i = 1; i <= 18; i++){
        _dmx.setLevel(i, 0);
    }
    _dmx.update();
}

void ofApp::update(){
#ifdef SENDDMX
    _dmx.update();
#endif
}

void ofApp::draw(){
    auto offset = 80;
    auto radius = 20;
    auto x = ofGetWidth()/3*2;
    auto y = ofGetHeight()/2;
    for(auto row = 0; row < 2; row++){
        for(auto column = 0; column < 2; column++){
            int channel = (row * 2 + column) * 4 +1;
            ofSetColor(255,255,255, _dmx.getLevel(channel));
            ofDrawCircle(x,y,radius);
            ofNoFill();
            ofSetColor(255,255,255, 32);
            ofDrawCircle(x,y,radius);
            ofFill();

            ofSetColor(255,255,255, _dmx.getLevel(channel+1));
            ofDrawCircle(x+offset,y,radius);
            ofNoFill();
            ofSetColor(255,255,255, 32);
            ofDrawCircle(x+offset,y,radius);
            ofFill();
            
            ofSetColor(255,255,255, _dmx.getLevel(channel+2));
            ofDrawCircle(x,y+offset,radius);
            ofNoFill();
            ofSetColor(255,255,255, 32);
            ofDrawCircle(x,y+offset,radius);
            ofFill();
            
            ofSetColor(255,255,255, _dmx.getLevel(channel+3));
            ofDrawCircle(x+offset,y+offset,radius);
            ofNoFill();
            ofSetColor(255,255,255, 32);
            ofDrawCircle(x+offset,y+offset,radius);
            ofFill();
            
            x += offset*2;
        }
        y += ofGetHeight()/4;
        x = ofGetWidth()/3*2;
    }
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