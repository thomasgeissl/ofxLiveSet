#pragma once

#include "ofMain.h"
#include "ofxDmx.h"
#include "ofxMidi.h"
#include "ofxMidiMapper.h"
#include "ofxSoundAnalyser.h"

#include "project.h"
#include "tracks/dmx.h"
#include "clips/dmx.h"

#include "./clips/dmx/sin.h"
#include "./clips/dmx/sines.h"
#include "./clips/dmx/rand.h"
#include "./clips/dmx/peak.h"
#include "./clips/dmx/strobe.h"
#include "./clips/dmx/within.h"
#include "./clips/dmx/anchor.h"
#include "./clips/dmx/firn.h"
#include "./clips/dmx/schwanensee.h"
#include "./clips/dmx/externalStrobe.h"
#include "./clips/dmx/utils/still.h"
#include "./clips/dmx/utils/midi2dmx.h"

#include "gui/session.h"

class ofApp : public ofBaseApp, public ofxMidiListener, public ofxSoundAnalyserListener {
public:
    ofApp();
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void onPeakEnergy(std::pair<int, float> & value);
    void onPitch(std::pair<int, float> & value);
    void onRootMeanSquare(std::pair<int, float> & value);
    void onFftMagnitudeSpectrum(std::pair<int, std::vector<float>> & value);
    void onMelFrequencySpectrum(std::pair<int, std::vector<float>> & value);
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
    void onFocusChange(int & value);
    ofxLiveSet::project _project;
    ofxLiveSet::session *_session;
    
    ofxDmx _dmx;
    ofxMidiIn _midiIn;
    ofxMidiMapper _midiMapper;
    ofxSoundAnalyser _soundAnalyser;

    ofParameterGroup _parameters;
    ofParameter<int> _focusedTrack;
    ofParameter<int> _focusedClip;
};

