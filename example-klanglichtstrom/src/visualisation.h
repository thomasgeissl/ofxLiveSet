#pragma once
#include "ofMain.h"
#include "ofxDmx.h"

class visualisation : public ofBaseDraws
{
public:
    void setup(ofxDmx *dmx)
    {
        _dmx = dmx;
        _fbo.allocate(ofGetWidth()/2, ofGetHeight()/2);
    }
    void update()
    {
        for (auto i = 1; i < 24; i++)
        {
            _values[i] = _dmx->getLevel(i);
        }
        auto x = 0;
        auto y = 0;
        auto width = _fbo.getWidth();
        auto height = _fbo.getHeight();
        auto offset = width / 4;
        auto radius = 20;
        auto xPos = x;
        auto yPos = y;
        _fbo.begin();
        ofClear(255, 0);
         for (auto row = 0; row < 2; row++)
        {
            for (auto column = 0; column < 2; column++)
            {
                int channel = (row * 2 + column) * 4 + 1;
                ofSetColor(255, 255, 255, _values[channel]);
                ofDrawCircle(xPos, yPos, radius);
                ofSetColor(255, 255, 255, 32);
                ofDrawBitmapString(ofToString(channel), xPos - 3, yPos + 3);
                ofNoFill();
                ofDrawCircle(xPos, yPos, radius);
                ofFill();

                ofSetColor(255, 255, 255, _values[channel + 1]);
                ofDrawCircle(xPos + offset, yPos, radius);
                ofSetColor(255, 255, 255, 32);
                ofDrawBitmapString(ofToString(channel + 1), xPos + offset - 3, yPos + 3);
                ofNoFill();
                ofDrawCircle(xPos + offset, yPos, radius);
                ofFill();

                ofSetColor(255, 255, 255, _values[channel + 2]);
                ofDrawCircle(xPos, yPos + offset, radius);
                ofSetColor(255, 255, 255, 32);
                ofDrawBitmapString(ofToString(channel + 2), xPos - 3, yPos + offset + 3);
                ofNoFill();
                ofDrawCircle(xPos, yPos + offset, radius);
                ofFill();

                ofSetColor(255, 255, 255, _values[channel + 3]);
                ofDrawCircle(xPos + offset, yPos + offset, radius);
                ofSetColor(255, 255, 255, 32);
                ofDrawBitmapString(ofToString(channel + 3), xPos + offset - 3, yPos + offset + 3);
                ofNoFill();
                ofDrawCircle(xPos + offset, yPos + offset, radius);
                ofFill();

                xPos += offset * 2;
            }
            yPos += height / 2;
            xPos = x;
        }
        _fbo.end();
    }
    void draw(float x, float y, float width, float height) const
    {
        _fbo.draw(x, y);
    }
    float getHeight() const
    {
        return 0;
    }
    float getWidth() const
    {
        return 0;
    }

    ofFbo getFbo() {
        return _fbo;
    }
    ofxDmx *_dmx;
    int _values[24];
    ofFbo _fbo;
};