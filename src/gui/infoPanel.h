#pragma once
#include "ofMain.h"

namespace ofxLiveSet{
    struct information{
        std::string _text = "";
        ofLogLevel _level = OF_LOG_NOTICE;
        int _duration = 3000;
    };
    namespace gui{
        class infoPanel : public ofBaseDraws{
        public:
            void setup(){
                _width = ofGetWidth()/4;
                _height = 100;
                _verdana14.load("verdana.ttf", 14, true, true);
                _verdana14.setLineHeight(18.0f);
                _verdana14.setLetterSpacing(1.037);
            }
            void draw(float x, float y, float w, float h) const {
                auto timestamp = ofGetElapsedTimeMillis();
                auto fadeOutTime = 1000;
                if(timestamp > _fadeOutTimestamp || _text.empty()){
                    return;
                }
                ofRectangle background;
                background.x = x;
                background.y = y;
                background.width = w;
                background.height = h;
                ofSetColor(ofColor(127,127,127));
                ofDrawRectRounded(background, 16);
                ofNoFill();
                switch(_logLevel){
                    case OF_LOG_ERROR:{
                        ofSetColor(ofColor::red);
                        break;
                    }
                    case OF_LOG_FATAL_ERROR:{
                        ofSetColor(ofColor::red);
                        break;
                    }
                    case OF_LOG_WARNING:{
                        ofSetColor(ofColor::yellow);
                        break;
                    }
                    default:
                        ofSetColor(ofColor::black);
                }
                ofDrawRectRounded(background, 16);
                ofFill();
                ofSetColor(ofColor(16,16,16));
                _verdana14.drawString(_text, x + 20, y + 24);
            }
            void setWidth(float value){
                _width = value;
            }
            void setHeight(float value){
                _height = value;
            }
            float getHeight() const {
                return _height;
            }
            
            float getWidth() const {
                return _width;
            }
            
            void setInfo(ofxLiveSet::information info){
                _logLevel = info._level;
                _text = info._text;
                _fadeOutTimestamp = ofGetElapsedTimeMillis() + info._duration;
            }
            void setInfo(std::string text, int durationMs = 5000, ofLogLevel level = OF_LOG_NOTICE){
                _logLevel = level;
                _text = text;
                _fadeOutTimestamp = ofGetElapsedTimeMillis() + durationMs;
            }
            
            float _width;
            float _height;
            ofTrueTypeFont _verdana14;
            ofLogLevel _logLevel;
            std::string _text;
            int _fadeOutTimestamp;
            
        };
    }; // namespace gui
}; // namespace ofxLiveSet

