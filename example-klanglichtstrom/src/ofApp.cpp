#include "ofApp.h"
#include "./dmx.config.h"

#include "./clips/dmx/within.h"
#include "./clips/dmx/anchor.h"
#include "./clips/dmx/schwanensee.h"
#include "./clips/dmx/firn.h"
#include "./clips/dmx/wind.h"
#include "./clips/dmx/centerOfMass.h"
#include "./clips/dmx/strobe.h"
#include "./clips/dmx/spot.h"
#include "./clips/dmx/chimes.h"
#include "./clips/dmx/outro.h"
#include "./clips/dmx/utils/still.h"
#include "./clips/dmx/utils/midi2dmx.h"
#include "./clips/dmx/utils/spares.h"

ofApp::ofApp() : _project(ofxLiveSet::project::create())
{
    setApplicationName("klangLichtStrom");
    setApplicationVersion("1.2.0");

    _session = _project->_sets.front()->_sessions.front();
}

void ofApp::setup()
{
    ofSetBackgroundColor(16, 16, 16);

    _dmx.connect("/dev/cu.usbserial-EN160415");
    _dmx.setChannels(64);
    _visualisation.setup(&_dmx);

    auto lightBulbsTrack = ofxLiveSet::track::dmx::create("light bulbs");
    auto rainMakerTrack = ofxLiveSet::track::dmx::create("rainmaker");
    auto strobeTrack = ofxLiveSet::track::dmx::create("strobe");
    auto chimesTrack = ofxLiveSet::track::dmx::create("chimes");
    auto mqttTrack = ofxLiveSet::track::mqtt::create("vgig");
    auto utilsTrack = ofxLiveSet::track::dmx::create("utils");

    lightBulbsTrack->addClip(clips::within::create())->setup();
    lightBulbsTrack->addClip(clips::anchor::create())->setup();
    lightBulbsTrack->addClip(clips::schwanensee::create())->setup();
    lightBulbsTrack->addClip(clips::firn::create())->setup();
    lightBulbsTrack->addClip(clips::newClip::create())->setup();
    lightBulbsTrack->addClip(clips::wind::create())->setup();
    lightBulbsTrack->addClip(clips::outro::create())->setup();

    strobeTrack->addClip(clips::strobe::create(21, 22), 3)->setup();

    rainMakerTrack->addClip(clips::spot::create())->setup();

    chimesTrack->addClip(clips::chimes::create(), 5)->setup();

    utilsTrack->addClip(clips::spares::create(), 3)->setup();
    utilsTrack->addClip(clips::still::create(), 7)->setup();
    utilsTrack->addClip(clips::midi2dmx::create())->setup();

    utilsTrack->mute();

    _session->addTrack(lightBulbsTrack);
    _session->addTrack(rainMakerTrack);
    _session->addTrack(strobeTrack);
    _session->addTrack(chimesTrack);
    _session->addTrack(mqttTrack);
    _session->addTrack(utilsTrack);

    for (auto track : _session->getTracks())
    {
        if (auto dmxTrack = std::dynamic_pointer_cast<ofxLiveSet::track::dmx>(track))
        {
            dmxTrack->setup(&_dmx);
        }
    }

    _session->setup();
    _session->openOscControlInPort(9000);
    _session->openMidiMapperInPort(1);
    _session->openMidiInPort(0);
    _session->stop();

    _session->renameScene(0, "within");
    _session->renameScene(1, "anchor");
    _session->renameScene(2, "schwanensee");
    _session->renameScene(3, "firn");
    _session->renameScene(4, "com");
    _session->renameScene(5, "lichtung");
    _session->renameScene(6, "outro");
    for (auto i = 7; i <= 8; i++)
    {
        _session->renameScene(i, "");
    }
    _session->fillWithNullClips();

    ofLogNotice() << ofGetWindowMode();
    ofLogNotice() << OF_WINDOW;
    ofLogNotice() << OF_FULLSCREEN;
}

void ofApp::exit()
{
    for (auto i = 1; i <= 24; i++)
    {
        _dmx.setLevel(i, 0);
    }
    _dmx.setLevel(KLS_SPOTLIGHT_CHANNEL, 0);
    _dmx.setLevel(KLS_STROBECHANNEL, 0);
    _dmx.update();
}

void ofApp::update()
{
    _visualisation.update();
    _session->setPreview(_visualisation.getFbo());
#ifdef SENDDMX
    _dmx.update();
#endif
}

void ofApp::draw()
{
    _session->drawGui();
}

void ofApp::keyPressed(int key)
{
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
void ofApp::gotMessage(ofMessage msg) {}
void ofApp::dragEvent(ofDragInfo dragInfo) {}
