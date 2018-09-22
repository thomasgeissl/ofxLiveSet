#pragma once
#include "ofMain.h"
#include "../tracks/base.h"

namespace ofxLiveSet
{
    namespace gui
    {
        class track{
        public:
            
            track(){
            };
            void setup(ofxLiveSet::track::base* track){
                _track = track;
            }
            void update(){
                
            }
            void draw(){
//                ofSetColor(ofColor(255,0,0));
//                ofDrawRectangle(0,0, ofGetWidth(), ofGetHeight()/10);
//                auto trackIndex = 0;
//                auto trackWidth = ofGetWidth()/_session->_tracks.size();
//                for(auto track : _session->_tracks){
//                    ofSetColor(ofRandom(255), 0, ofRandom(255));
//                    ofDrawRectangle(trackIndex*trackWidth, 0, trackWidth, ofGetHeight());
//                    trackIndex++;
//                }
            }
            void onUpdate(ofEventArgs &e)
            {
                update();
            }
            
            void onDraw(ofEventArgs &e)
            {
                draw();
            }
            
            ofxLiveSet::track::base *_track;
        };
    };
};
