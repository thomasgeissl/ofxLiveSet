#include "ofApp.h"

ofApp::ofApp() : _session(_project._session){
}

void ofApp::setup(){
    ofSetBackgroundColor(16, 16, 16);

    _soundAnalyser.setup();
    _soundAnalyser.addListener(this);

    _dmx.connect("tty.usbserial-EN160415");
    _dmx.setChannels(16);
    
    ofxLiveSet::track::dmx* lightBulbsTrack = (ofxLiveSet::track::dmx*)(_session->addTrack(new ofxLiveSet::track::dmx("light bulbs")));
    ofxLiveSet::track::dmx* strobeTrack = (ofxLiveSet::track::dmx*)(_session->addTrack(new ofxLiveSet::track::dmx("strobe")));
    ofxLiveSet::track::dmx* utilsTrack = (ofxLiveSet::track::dmx*)(_session->addTrack(new ofxLiveSet::track::dmx("utils")));
    
    
    lightBulbsTrack->addClip(new clips::within())->setup();
    lightBulbsTrack->addClip(new clips::anchor())->setup();
    lightBulbsTrack->addClip(new clips::firn())->setup();
    lightBulbsTrack->addClip(new clips::schwanensee())->setup();

    strobeTrack->addClip(new clips::externalStrobe(17, 18), 2)->setup();

    utilsTrack->addClip(new clips::still(), 4)->setup();
    utilsTrack->addClip(new clips::midi2dmx())->setup();
    

    lightBulbsTrack->setup(&_dmx);
    strobeTrack->setup(&_dmx);
    utilsTrack->setup(&_dmx);
    
    _session->setup();
    _session->renameScene(0, "within");
    _session->renameScene(1, "anchor");
    _session->renameScene(2, "firn");
    _session->renameScene(3, "schwanensee");
    _session->stop();

    
    _midiIn.openVirtualPort("ofxLiveSet");
    _midiIn.ignoreTypes(false, false, false);
    _midiIn.addListener(this);
    _midiIn.setVerbose(true);
}

void ofApp::exit(){
    for(auto i = 1; i <= 512; i++){
        _dmx.setLevel(i, 0);
    }
    _dmx.update();
}

void ofApp::update(){
    _dmx.update();
    _soundAnalyser.update();
    ofSetWindowTitle("klanglichtstrom :: fps: "+ofToString((int)(ofGetFrameRate())));
}

void ofApp::draw(){
    auto offset = 100;
    auto radius = 20;
    auto x = ofGetWidth()/2;
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
            
            x += ofGetWidth()/4;
        }
        y += ofGetHeight()/4;
        x = ofGetWidth()/2;
    }
    
////    draw audio
//    x = ofGetWidth()/2 - 100;
//    y = ofGetHeight()/2 - 100;
//    ofSetColor(255, 0, 0, 64);
//    ofDrawRectangle(x, y, 50, 50);
//    ofSetColor(ofColor::white);
//    ofDrawBitmapString("audio", x,y+25);
//
////    draw left speaker
//    x = ofGetWidth() - 100;
//    y = ofGetHeight()/2 - 100;
//    ofSetColor(255, 0, 255, 64);
//    ofDrawRectangle(x, y, 50, 50);
//    ofSetColor(ofColor::white);
//    ofDrawBitmapString("left", x,y+25);
//    ofDrawBitmapString("speaker", x,y+45);
//
////    draw right speaker
//    x = ofGetWidth()/2 - 100;
//    y = ofGetHeight() - 50;
//    ofSetColor(255, 0, 255, 64);
//    ofDrawRectangle(x, y, 50, 50);
//    ofSetColor(ofColor::white);
//    ofDrawBitmapString("right", x,y+25);
//    ofDrawBitmapString("speaker", x,y+45);
//
////    draw light
//    x = ofGetWidth() - 100;
//    y = ofGetHeight() - 50;
//    ofSetColor(0, 0, 255, 64);
//    ofDrawRectangle(x, y, 50, 50);
//    ofSetColor(ofColor::white);
//    ofDrawBitmapString("light", x,y+25);
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

void ofApp::onPeakEnergy(std::pair<int, float> & value){
    for(auto track : _session->_tracks){
        auto clip = dynamic_cast<clips::soundReactiveDmx *>(track->_clip);
        if (clip != nullptr) {
            clip->setPeakEnergy(value.first, value.second);
        }
    }
}
void ofApp::onPitch(std::pair<int, float> & value){
    int note = std::round((value.second > 0 ? 17.3123405046 * log(.12231220585 * value.second) : -1500));
    
    for(auto track : _session->_tracks){
        auto clip = dynamic_cast<clips::soundReactiveDmx *>(track->_clip);
        if (clip != nullptr) {
            clip->setPitch(value.first, note);
        }
    }
}
void ofApp::onRootMeanSquare(std::pair<int, float> & value){
    for(auto track : _session->_tracks){
        auto clip = dynamic_cast<clips::soundReactiveDmx *>(track->_clip);
        if (clip != nullptr) {
            clip->setRootMeanSquare(value.first, value.second);
        }
    }
}
void ofApp::onFftMagnitudeSpectrum(std::pair<int, std::vector<float>> & value){
    for(auto track : _session->_tracks){
        auto clip = dynamic_cast<clips::soundReactiveDmx *>(track->_clip);
        if (clip != nullptr) {
            clip->setFftMagnitudeSpectrum(value.first, value.second);
        }
    }
}
void ofApp::onMelFrequencySpectrum(std::pair<int, std::vector<float>> & value){ 
    for(auto track : _session->_tracks){
        auto clip = dynamic_cast<clips::soundReactiveDmx *>(track->_clip);
        if (clip != nullptr) {
            clip->setMelFrequencySpectrum(value.first, value.second);
        }
    }
}
void ofApp::newMidiMessage(ofxMidiMessage& message){
    for(auto track : _session->_tracks){
        auto clip = dynamic_cast<clips::midiReactiveDmx *>(track->_clip);
        if (clip != nullptr) {
            auto status = message.status;
            if(status == MIDI_NOTE_ON){
                clip->setNoteOn(message.pitch, message.velocity);
            }else if(status == MIDI_NOTE_OFF){
                clip->setNoteOff(message.pitch, message.velocity);
            }
        }
    }
}
