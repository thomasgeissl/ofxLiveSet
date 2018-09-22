#pragma once

#include "ofMain.h"
#include "project.h"
#include "tracks/graphic.h"
#include "tracks/audio.h"
#include "clips/graphic.h"
#include "clips/audio.h"
#include "gui/session.h"

#include "visuals/rects.h"
#include "visuals/lines.h"

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
	private:
		ofxLiveSet::project _project;
		ofxLiveSet::session *_session;
    
    ofxLiveSet::gui::session _gui;
};
