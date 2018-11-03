#include "ofApp.h"

ofApp::ofApp(){
	_session = _project._session;
}

void ofApp::setup(){
    ofSetBackgroundColor(0, 0, 0);


//    thirdGraphicTrack->addClip(new ofxLiveSet::clip::videoGrabber(0));
    //    _session->_tracks[2]->addClip(new ofxLiveSet::clip::audio("/Users/thomasgeissl/Desktop/untitled_134.mp3"));
    //    _session->_tracks[3]->addClip(new ofxLiveSet::clip::audio("/Users/thomasgeissl/Desktop/untitled_134.mp3"));
    
    
    ofAddListener(_soundAnalyser._peakEnergyEvent, this, &ofApp::onPeakEnergy);
    ofAddListener(_soundAnalyser._pitchEvent, this, &ofApp::onPitch);
    ofAddListener(_soundAnalyser._rootMeanSquareEvent, this, &ofApp::onRootMeanSquare);
    ofAddListener(_soundAnalyser._fftMagnitudeSpectrumEvent, this, &ofApp::onFftMagnitudeSpectrum);
    ofAddListener(_soundAnalyser._melFrequencySpectrumEvent, this, &ofApp::onMelFrequencySpectrum);

    _soundAnalyser.setup();
    
#ifdef USEDMX
    _dmx.connect("tty.usbserial-EN160415");
    _dmx.setChannels(16);
    _dmx.setLevel(1, 255);
    
    ofxLiveSet::track::dmx* lightBulbsTrack = (ofxLiveSet::track::dmx*)(_session->addTrack(new ofxLiveSet::track::dmx("light bulbs")));
    ofxLiveSet::track::dmx* strobeTrack = (ofxLiveSet::track::dmx*)(_session->addTrack(new ofxLiveSet::track::dmx("strobe")));
    ofxLiveSet::track::dmx* utilsTrack = (ofxLiveSet::track::dmx*)(_session->addTrack(new ofxLiveSet::track::dmx("utils")));
    
    
    lightBulbsTrack->addClip(new clips::within());
    lightBulbsTrack->addClip(new clips::anchor());
    lightBulbsTrack->addClip(new clips::sin(1));
    lightBulbsTrack->addClip(new clips::peak());
    lightBulbsTrack->addClip(new clips::rand(1));
    lightBulbsTrack->addClip(new clips::sines());
    lightBulbsTrack->addClip(new clips::strobe());

    strobeTrack->addClip(new clips::externalStrobe(17, 18));

    utilsTrack->addClip(new clips::frozen());
    utilsTrack->addClip(new clips::midi2dmx());

//    secondDmxTrack->addClip(new clips::sin(2));
//    secondDmxTrack->addClip(new clips::rand(2));
    
    lightBulbsTrack->setup(&_dmx);
    strobeTrack->setup(&_dmx);
    utilsTrack->setup(&_dmx);
    utilsTrack->mute(true);

#endif
    
//    auto firstGraphicTrack = _session->addTrack(new ofxLiveSet::track::graphic("Graphic 0"));
//    auto secondGraphicTrack = _session->addTrack(new ofxLiveSet::track::graphic("Graphic 1"));
//    auto thirdGraphicTrack = _session->addTrack(new ofxLiveSet::track::graphic("Graphic 2"));
//    
//    _session->addTrack(new ofxLiveSet::track::audio());
//    _session->addTrack(new ofxLiveSet::track::audio());
//    
//    firstGraphicTrack->addClip(new clips::lines());
//    firstGraphicTrack->addClip(new clips::rects());
//    firstGraphicTrack->addClip(new clips::rects());
//    
//    secondGraphicTrack->addClip(new clips::lines());
//    secondGraphicTrack->addClip(new clips::lines());
//    secondGraphicTrack->addClip(new clips::lines());
    _session->setup();
    _session->triggerScence(1);
//    secondDmxTrack->stop();
    
    midiIn.openVirtualPort("ofxLiveSet");
    midiIn.ignoreTypes(false, false, false);
    midiIn.addListener(this);
    midiIn.setVerbose(true);
    
    _focusedTrack.set("focusedTrack", 0);
    _focusedClip.set("focusedClip", 0);

    _focusedTrack.addListener(this, &ofApp::onFocusChange);
    _focusedClip.addListener(this, &ofApp::onFocusChange);
}

void ofApp::exit(){
    for(auto i = 0; i < 512; i++){
        _dmx.setLevel(i, 0);
    }
    _dmx.update();
}

void ofApp::update(){
    _dmx.update();
    _soundAnalyser.update();
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
            ofSetColor(255,255,255, _dmx.getLevel(channel+1));
            ofDrawCircle(x+offset,y,radius);
            ofSetColor(255,255,255, _dmx.getLevel(channel+2));
            ofDrawCircle(x,y+offset,radius);
            ofSetColor(255,255,255, _dmx.getLevel(channel+3));
            ofDrawCircle(x+offset,y+offset,radius);
            x += ofGetWidth()/4;
        }
        y += ofGetHeight()/4;
        x = ofGetWidth()/2;
    }
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
        case '5':
            _session->showClipGui(0,5);
            break;
        case 'b': {
            auto clip = ((clips::peak *)(_session->_tracks[0]->_clips[1]));
            if(clip != nullptr){
                clip->setPeakEnergy(0, 1);
            }
            break;
        }
        case OF_KEY_LEFT: {
            _focusedTrack = std::max(0
                                     , _focusedTrack-1);
            break;
        }
        case OF_KEY_RIGHT: {
            _focusedTrack = std::min((int)(_session->_tracks.size()), _focusedTrack+1);
            break;
        }
        case OF_KEY_UP: {
            _focusedClip = std::max(0, _focusedClip-1);
            break;
        }
        case OF_KEY_DOWN: {
            _focusedClip = std::min((int)(_session->_tracks[_focusedTrack]->_clips.size()), _focusedClip+1);
            break;
        }
        case OF_KEY_RETURN: {
            auto clip = _session->getClip(_focusedTrack, _focusedClip);
            if(clip != nullptr){
                clip->toggle();
            }
            break;
        }
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

void ofApp::onFocusChange(int & value){
    ofLogNotice()<<_focusedTrack << " " << _focusedClip;
    _session->showClipGui(_focusedTrack, _focusedClip);
}
