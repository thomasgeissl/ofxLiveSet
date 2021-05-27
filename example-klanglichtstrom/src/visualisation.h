#pragma once
#include "ofMain.h"
#include "ofxDmx.h"

class visualisation : public ofBaseDraws
{
public:
    void setup(ofxDmx *dmx)
    {
        _dmx = dmx;
        // _fbo.allocate(ofGetWidth()/2, ofGetHeight()/2);
        ofFbo::Settings fboSettings ;
        fboSettings.width = ofGetWidth()/2;
        fboSettings.height = ofGetHeight()/2;
        fboSettings.internalformat = GL_RGBA ;
        fboSettings.textureTarget = GL_TEXTURE_2D ;
        _fbo.allocate( fboSettings ) ;
    }
    void update()
    {
        for (auto i = 0; i < 24; i++)
        {
            _values[i] = _dmx->getLevel(i+1);
        }

        _fbo.begin();
        ofClear(255, 0, 0, 64);
        drawDimmer(0, _fbo.getWidth()/4, _fbo.getHeight()/4);
        drawDimmer(1, _fbo.getWidth()/4*3, _fbo.getHeight()/4);
        drawDimmer(2, _fbo.getWidth()/4, _fbo.getHeight()/4*3);
        drawDimmer(3, _fbo.getWidth()/4*3, _fbo.getHeight()/4*3);
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

    void drawDimmer(int index, float cx, float cy)
    {
        auto increment = 2 * glm::pi<float>()/6;
        auto angle = 0;
        auto length = 50;
        for(auto i = 0; i < 6; i++)
        {
            float x = length * std::cos(angle);
            float y = length * std::sin (angle);
            angle += increment;
            ofSetColor(255, 255, 255, _values[index*6 + i]);
            ofDrawCircle(cx + x, cy + y, 10);
        }
    }
    ofxDmx *_dmx;
    int _values[25];
    ofFbo _fbo;
};