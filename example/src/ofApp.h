#pragma once
#define USEDMX

#include "ofMain.h"
#include "project.h"
#include "tracks/graphic.h"
#include "tracks/audio.h"

#include "clips/graphic.h"
#include "clips/videoGrabber.h"
#include "clips/audio.h"

#include "./soundAnalyser.h"

#ifdef USEDMX
#include "ofxDmx.h"
#include "tracks/dmx.h"
#include "clips/dmx.h"
#include "./clips/dmx/sin.h"
#include "./clips/dmx/sines.h"
#include "./clips/dmx/rand.h"
#include "./clips/dmx/peak.h"
#include "./clips/dmx/frozen.h"
#endif

#include "gui/session.h"

#include "./clips/graphic/rects.h"
#include "./clips/graphic/lines.h"

class ofApp : public ofBaseApp {
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

		ofxLiveSet::project _project;
		ofxLiveSet::session *_session;
    
        soundAnalyser _soundAnalyser;
    
    
#ifdef USEDMX
        ofxDmx _dmx;
#endif
};