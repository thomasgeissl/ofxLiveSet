#include "ofApp.h"

ofApp::ofApp() : _project(ofxLiveSet::project::create())
{
    _session = _project->_sessions.back();
}

void ofApp::setup()
{
    ofSetBackgroundColor(16, 16, 16);

    auto audioAnalyserTrack = ofxLiveSet::track::audio::create("audio analyser");
    auto videoATrack = ofxLiveSet::track::graphic::create("video A");
    auto videoBTrack = ofxLiveSet::track::graphic::create("video B");
    auto visualATrack = ofxLiveSet::track::graphic::create("visual A");
    auto visualBTrack = ofxLiveSet::track::graphic::create("visual B");
    _session->addTrack(audioAnalyserTrack);
    _session->addTrack(videoATrack);
    _session->addTrack(videoBTrack);
    _session->addTrack(visualATrack);
    _session->addTrack(visualBTrack);

    audioAnalyserTrack->mute();

    videoATrack->addClip(ofxLiveSet::clip::videoGrabber::create(0, "camera"))->setup();
    videoATrack->addClip(ofxLiveSet::clip::videoPlayer::create("videos/lake_carrier.mov", "lake carrier"), 2)->setup();

    videoBTrack->addClip(ofxLiveSet::clip::videoGrabber::create(0, "camera"))->setup();
    videoBTrack->addClip(ofxLiveSet::clip::videoPlayer::create("videos/lake_carrier.mov", "lake carrier"), 2)->setup();

    visualATrack->addClip(clips::cubeWithTrails::create())->setup();
    visualATrack->addClip(clips::midiVisualiser::create())->setup();
    visualATrack->addClip(clips::beatVisualiser::create())->setup();

    visualATrack->addClip(clips::snake::create())->setup();
    visualATrack->addClip(clips::gameOfLife::create())->setup();

    visualATrack->addClip(clips::unkownPleasures::create())->setup();
    visualATrack->addClip(clips::onTheDarkSideOfTheMoon::create())->setup();

    visualBTrack->addClip(clips::randomRectangles::create())->setup();
    visualBTrack->addClip(clips::bezierVertex::create(), 3)->setup();
    visualBTrack->addClip(clips::parametric2dEquation::create())->setup();

    visualBTrack->addClip(clips::arcs::create())->setup();
    visualBTrack->addClip(clips::circles::create())->setup();

    // visualBTrack->addClip(ofxLiveSet::clip::slidePlayer::create("TODO"))->setup();
    visualBTrack->addClip(clips::untitled_00::create())->setup();
    visualBTrack->addClip(clips::untitled_01::create())->setup();

    visualBTrack->addClip(clips::randomPolygon::create())->setup();

    _session->setup();
    _session->setupGui(150);
    _session->setupAudioEngine(2);

    _session->openMidiMapperInPort(0);
    _session->openOscControlInPort(9000);
    _session->stop();
    _session->_oscControlEnabled = false;

    _drawGui.set("drawGui", true);
    ofSoundStreamSetup(0, 1, this, 44100, _beat.getBufferSize(), 4);

    _renderApp->setSession(_session);
}

void ofApp::exit()
{
    ofSoundStreamClose();
}

void ofApp::update()
{
    ofSetWindowTitle("example-graphic control window: " + ofToString((int)(ofGetFrameRate())));
    _beat.update(ofGetElapsedTimeMillis());
    auto kick = _beat.kick();
    auto snare = _beat.snare();
    auto hihat = _beat.hihat();
    for (auto track : _session->_tracks)
    {
        // auto beatReactiveClip = dynamic_pointer_cast<clips::beatReactive>(track->_clip);
        // if(beatReactiveClip != nullptr){
        //     beatReactiveClip->setKick(kick);
        //     beatReactiveClip->setSnare(snare);
        //     beatReactiveClip->setHihat(hihat);
        // }
    }
}

void ofApp::draw()
{
    _session->draw();
    if (_drawGui)
    {
        _session->drawGui();
    }
}

void ofApp::keyPressed(int key)
{
    switch (key)
    {
    case 'g':
        _drawGui = !_drawGui;
        break;
    }
    _session->onKeyPressed(key);
}
void ofApp::keyReleased(int key) {}
void ofApp::mouseMoved(int x, int y) {}
void ofApp::mouseDragged(int x, int y, int button) {}
void ofApp::mousePressed(int x, int y, int button) {}
void ofApp::mouseReleased(int x, int y, int button) {}
void ofApp::mouseEntered(int x, int y) {}
void ofApp::mouseExited(int x, int y) {}
void ofApp::windowResized(int w, int h) {}
void ofApp::dragEvent(ofDragInfo dragInfo) {}
void ofApp::gotMessage(ofMessage msg) {}

//TODO: check how to integrate in session, and ofxPDSP
void ofApp::audioReceived(float *input, int bufferSize, int nChannels)
{
    _beat.audioReceived(input, bufferSize, nChannels);
}