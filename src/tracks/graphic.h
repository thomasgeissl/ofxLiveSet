#pragma once
#include "ofMain.h"
#include "base.h"
#include "../clips/graphic.h"

#if OFXLIVESET_USE_DOTFRAG
#include "ofxDotFrag.h"
#endif

namespace ofxLiveSet
{
namespace track
{
class graphic : public base
{
public:
    typedef std::shared_ptr<graphic> pointer;
    static pointer create(std::string name)
    {
        return std::make_shared<graphic>(name);
    }
    graphic(std::string name = "") : base(name)
#if OFXLIVESET_USE_DOTFRAG
                                     ,
                                     _frags({
                                         new ofx::dotfrag::Delay(),
                                         new ofx::dotfrag::EchoTrace(),
                                         new ofx::dotfrag::FXAA(),
                                         new ofx::dotfrag::HSB(),
                                         new ofx::dotfrag::InvertStrobe(),
                                         new ofx::dotfrag::Mirror(),
                                         new ofx::dotfrag::MirrorAxis(),
                                         new ofx::dotfrag::Monochrome(),
                                         new ofx::dotfrag::RadialRemap(),
                                         new ofx::dotfrag::ThreeTones(),
                                         new ofx::dotfrag::Turbolence(),
                                         new ofx::dotfrag::Twist(),
                                     })

#endif
    {
        _xPosition.set("x", 0, 0, ofGetWidth());
        _yPosition.set("y", 0, 0, ofGetHeight());
        _width.set("width", ofGetWidth(), 0, ofGetWidth());
        _height.set("height", ofGetHeight(), 0, ofGetHeight());

        _width.addListener(this, &graphic::onWidthChange);
        _height.addListener(this, &graphic::onHeightChange);

        _shape.setName("shape");
        _shape.add(_xPosition);
        _shape.add(_yPosition);
        _shape.add(_width);
        _shape.add(_height);
        _ioParameters.add(_shape);
        _fbo.allocate(_width, _height);
        _fbo.begin();
        ofClear(255, 0);
        _fbo.end();

#if OFXLIVESET_USE_DOTFRAG
        for (auto &frag : _frags)
        {
            frag->allocate(_fbo);
            frag->active = false;
            _effectsParameters.add(frag->parameters);
        }
        _effectsParameters.setName("effects");
        _ioParameters.add(_effectsParameters);
#endif
    }
    virtual void draw()
    {
        auto clip = std::dynamic_pointer_cast<clip::graphic>(_clip);

        if (!_mute && clip != nullptr && clip->isFrameNew())
        {
            _fbo.begin();
            ofClear(255, 0);
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            ofSetColor(255, _gain * 255);
            clip->_fbo.draw(0, 0);
            _fbo.end();
        }
        if (clip == nullptr)
        {
            _fbo.begin();
            ofClear(255, 0);
            _fbo.end();
        }
#if OFXLIVESET_USE_DOTFRAG
        for (auto &frag : _frags)
        {
            frag->apply(_fbo);
        }
#endif
        _fbo.draw(_xPosition, _yPosition);
    }
    void resize(float width, float height)
    {
        _xPosition.setMax(_width);
        _yPosition.setMax(_height);
        _width.setMax(_width);
        _height.setMax(_height);

        _width = width;
        _height = height;
        _fbo.allocate(width, height);
        _fbo.begin();
        ofClear(255, 0);
        _fbo.end();
#if OFXLIVESET_USE_DOTFRAG
        for (auto &frag : _frags)
        {
            frag->allocate(_fbo);
            _effectsParameters.add(frag->parameters);
        }
#endif
        for (auto clip : _clips)
        {
            auto graphicClip = dynamic_pointer_cast<ofxLiveSet::clip::graphic>(clip);
            if (graphicClip != nullptr)
            {
                graphicClip->setSize(_width, _height);
            }
        }
    }
    void onWidthChange(float &value)
    {
        if (value == _width)
        {
            return;
        }
        resize(_width, _height);
    }
    void onHeightChange(float &value)
    {
        if (value == _height)
        {
            return;
        }
        resize(_width, _height);
    }
    ofFbo _fbo;
    ofParameterGroup _shape;
    ofParameter<float> _xPosition;
    ofParameter<float> _yPosition;
    ofParameter<float> _width;
    ofParameter<float> _height;

#if OFXLIVESET_USE_DOTFRAG
    ofParameterGroup _effectsParameters;
    std::vector<ofx::dotfrag::Base *> _frags;
#endif
};
}; // namespace track
}; // namespace ofxLiveSet
