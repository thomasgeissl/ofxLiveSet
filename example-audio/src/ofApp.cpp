#include "ofApp.h"

ofApp::ofApp() : _session(_project._session){
}

void ofApp::setup(){
    ofSetBackgroundColor(16, 16, 16);

    // inputs
    _soundAnalyser.setup();
    _soundAnalyser.addListener(this);
    
    _midiIn.openVirtualPort("ofxLiveSet");
    _midiIn.ignoreTypes(false, false, false);
    _midiIn.addListener(this);
    _midiIn.setVerbose(true);
    
    // session
    auto allTrack = (ofxLiveSet::track::audio*)(_session->addTrack(new ofxLiveSet::track::audio("all")));
    auto leftTrack = (ofxLiveSet::track::audio*)(_session->addTrack(new ofxLiveSet::track::audio("left")));
    auto rightTrack = (ofxLiveSet::track::audio*)(_session->addTrack(new ofxLiveSet::track::audio("right")));

    rightTrack->addClip(new ofxLiveSet::clip::audioPlayer())->setup();

    _session->setup();
    _session->setupGui();
    // osx: you might need to create an aggregated device
    _session->setupAudioEngine(2);
    _session->openMidiMapperInPort(0);
    _session->openOscInPort(9000);
    _session->stop();

    _drawGui.set("drawGui", true);

}

void ofApp::exit(){
}

void ofApp::update(){
    _soundAnalyser.update();
    ofSetWindowTitle("example-audio: "+ofToString((int)(ofGetFrameRate())));
}

void ofApp::draw(){
    if(_drawGui){
        _session->drawGui();
    }
}

void ofApp::keyPressed(int key){
    _session->onKeyPressed(key);
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

void ofApp::onPeakEnergy(std::pair<int, float> & value){
    // for(auto track : _session->_tracks){
    //     auto clip = dynamic_cast<clips::soundReactiveDmx *>(track->_clip);
    //     if (clip != nullptr) {
    //         clip->setPeakEnergy(value.first, value.second);
    //     }
    // }
}
void ofApp::onPitch(std::pair<int, float> & value){
    int note = std::round((value.second > 0 ? 17.3123405046 * log(.12231220585 * value.second) : -1500));
    
    // for(auto track : _session->_tracks){
    //     auto clip = dynamic_cast<clips::soundReactiveDmx *>(track->_clip);
    //     if (clip != nullptr) {
    //         clip->setPitch(value.first, note);
    //     }
    // }
}
void ofApp::onRootMeanSquare(std::pair<int, float> & value){
    // for(auto track : _session->_tracks){
    //     auto clip = dynamic_cast<clips::soundReactiveDmx *>(track->_clip);
    //     if (clip != nullptr) {
    //         clip->setRootMeanSquare(value.first, value.second);
    //     }
    // }
}
void ofApp::onFftMagnitudeSpectrum(std::pair<int, std::vector<float>> & value){
    // for(auto track : _session->_tracks){
    //     auto clip = dynamic_cast<clips::soundReactiveDmx *>(track->_clip);
    //     if (clip != nullptr) {
    //         clip->setFftMagnitudeSpectrum(value.first, value.second);
    //     }
    // }
}
void ofApp::onMelFrequencySpectrum(std::pair<int, std::vector<float>> & value){ 
    // for(auto track : _session->_tracks){
    //     auto clip = dynamic_cast<clips::soundReactiveDmx *>(track->_clip);
    //     if (clip != nullptr) {
    //         clip->setMelFrequencySpectrum(value.first, value.second);
    //     }
    // }
}
void ofApp::newMidiMessage(ofxMidiMessage& message){
    // for(auto track : _session->_tracks){
    //     auto clip = dynamic_cast<clips::midiReactiveDmx *>(track->_clip);
    //     if (clip != nullptr) {
    //         auto status = message.status;
    //         if(status == MIDI_NOTE_ON){
    //             clip->setNoteOn(message.pitch, message.velocity);
    //         }else if(status == MIDI_NOTE_OFF){
    //             clip->setNoteOff(message.pitch, message.velocity);
    //         }
    //     }
    // }
}
