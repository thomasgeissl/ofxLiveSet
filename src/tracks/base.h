#pragma once
#include "ofMain.h"
#include "../clips/base.h"
#include "../clips/null.h"

namespace ofxLiveSet
{
    namespace track
    {
        class base
        {
        public:
            typedef std::shared_ptr<base> pointer;
            static pointer create(std::string name)
            {
                return std::make_shared<base>(name);
            }
            base(std::string name = "") : _clip(nullptr), _focused(false)
            {
                _parameters.setName(name);
                _name.set("name", name);
                _stop.set("stop");
                _mute.set("mute", false);
                _solo.set("solo", false);
                _gain.set("gain", 1, 0, 1);

                _controls.setName("control");
                //        _parameters.add(_name);
                _controls.add(_stop);
                _controls.add(_solo);
                _controls.add(_mute);
                _controls.add(_gain);
                _parameters.add(_controls);

                _clipTriggers.setName("clips");
                _parameters.add(_clipTriggers);

                _stop.addListener(this, &base::onStop);

                auto ioName = name == "" ? "IO" : name + " IO";
                _ioParameters.setName(ioName);
            }

            virtual void setup() {}

            virtual void update()
            {
                if (_clip != nullptr)
                {
                    _clip->update();
                }
            }

            virtual void draw()
            {
                if (_clip)
                {
                    _clip->draw();
                }
            }

            void start()
            {
                if (_clip != nullptr)
                {
                    _clip->start();
                }
            }

            void stop()
            {
                if (_clip != nullptr)
                {
                    _clip->stop();
                }
            }

            clip::base::pointer addClip(clip::base::pointer clip)
            {
                _clips.push_back(clip);
                _clipTriggers.add(clip->_active);
                ofAddListener(clip->_started, this, &base::onClipStarted);
                ofAddListener(clip->_stopped, this, &base::onClipStopped);

                return clip;
            }

            clip::base::pointer addClip(clip::base::pointer clip, int index)
            {
                while (_clips.size() < index)
                {
                    auto nullClip = clip::nullClip::create();
                    _clipTriggers.add(nullClip->_active);
                    _clips.push_back(nullClip);
                ofAddListener(nullClip->_started, this, &base::onClipStarted);
                ofAddListener(nullClip->_stopped, this, &base::onClipStopped);
                }
                _clips.push_back(clip);
                _clipTriggers.add(clip->_active);
                ofAddListener(clip->_started, this, &base::onClipStarted);
                ofAddListener(clip->_stopped, this, &base::onClipStopped);

                return clip;
            }
            std::vector<clip::base::pointer> getClips()
            {
                return _clips;
            }

            void setClip(clip::base::pointer clip)
            {
                _clip = clip;
            }

            void trigger(int index)
            {
                stop();
                if (index < _clips.size())
                {
                    _clip = _clips[index];
                    if (_clip != nullptr)
                    {
                        _clip->start();
                    }
                }
            }

            void mute(bool value = true)
            {
                _mute = value;
            }

            void setSolo(bool value = true)
            {
                _solo = value;
            }

            void onClipStarted(const void *sender, bool &value)
            {
                for (auto clip : _clips)
                {
                    if (clip.get() != sender)
                    {
                        clip->stop();
                    }
                    else
                    {
                        _clip = clip;
                    }
                }
            }

            void onClipStopped(const void *sender, bool &value)
            {
                if (_clip.get() == sender)
                {
                    _clip->stop();
                    _clip = nullptr;
                }
            }

            void onStop()
            {
                stop();
            }

            void setGain(float value)
            {
                _gain = value;
            }

            clip::base::pointer getClip()
            {
                return _clip;
            }

            clip::base::pointer getClip(int index){
                if(index < 0 || index >= _clips.size()){
                    return nullptr;
                }
                return _clips[index];
            }

            std::string getName() {
                return _name;
            }

            bool isMuted()
            {
                return _mute;
            }
            bool isSoloed()
            {
                return _solo;
            }


            std::vector<clip::base::pointer> _clips;
            clip::base::pointer _clip;
            ofParameterGroup _parameters;
            ofParameterGroup _controls;
            ofParameter<std::string> _name;
            ofParameter<void> _stop;
            ofParameter<bool> _mute;
            ofParameter<bool> _solo;
            ofParameter<float> _gain;
            ofParameterGroup _clipTriggers;

            ofParameterGroup _ioParameters;
            bool _focused;
        };
    }; // namespace track
};     // namespace ofxLiveSet
