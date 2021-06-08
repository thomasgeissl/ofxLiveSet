#pragma once
#include "./config.h"

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxPDSP.h"
#include "ofxMidi.h"
#include "ofxImGui.h"
#include "ofxMidiMapper.h"
#include "ofxKeyMapper.h"
#include "ofxOscMapper.h"
#include "ofxMovingAverage.h"
#include "./tracks/base.h"
#include "./tracks/audio.h"
#include "./tracks/graphic.h"
#include "./clips/soundReactive.h"
#include "./clips/midiReactive.h"
#include "./gui/infoPanel.h"

#if OFXLIVESET_USE_SOUNDANALYSER
#include "ofxSoundAnalyser.h"
#endif

namespace ofxLiveSet
{
        class session : public ofxMidiListener
#if OFXLIVESET_USE_SOUNDANALYSER
            ,
                        public ofxSoundAnalyser::Listener
#endif
        {
        public:
                typedef std::shared_ptr<session> pointer;
                static pointer create()
                {
                        return std::make_shared<session>();
                }
                session();

                void setup();
                void setupAudioEngine(int id, int inChannels = 2, int outChannels = 2);
                void openMidiInPort(int index);
                void openVirtualMidiInPort(std::string name);
                void openMidiMapperInPort(int index);
                void openOscControlInPort(int port);
                void update();
                void draw();
                void drawGui();
                void drawMenuGui();
                void drawBrowserGui();
                void drawSessionGui();
                void drawInfoGui();
                void drawTrackGui();
                void drawClipGui();
                void drawPreviewGui();
                void drawMidiMapperGui();
                void drawKeyMapperGui();
                void drawOscMapperGui();
                void drawPreferencesGui();
                void exit();

                void onUpdate(ofEventArgs &e);
                void onDraw(ofEventArgs &e);
                void onExit(ofEventArgs &e);
                void onWindowResized(ofResizeEventArgs &e);
                void onWindowResized(float width, float height);
                void onKeyPressed(ofKeyEventArgs &e);
                void onKeyPressed(int key);
                void toggle();
                void start();
                void pause();
                void stop();
                track::base::pointer addTrack(track::base::pointer track);
                void fillWithNullClips();
                void triggerScene(int index);
                clip::base::pointer getClip(int track, int index);
                std::vector<track::base::pointer> getTracks();
                track::base::pointer getTrack(clip::base::pointer);

                session *renameScene(int index, std::string name);
                session *insertSceneAbove(int index);

                void onSceneTrigger(const void *sender, bool &value);
                void onActiveChange(bool &value);
                void onClipStarted(const void *sender, bool &value);
                void onInputChannelsChange(const void *sender, std::pair<int, int> &value);
                void onOscControlEnabledChange(bool &value);

                void onPreviousTrackTriggered();
                void onNextTrackTriggered();
                void onPreviousClipTriggered();
                void onNextClipTriggered();

#if OFXLIVESET_USE_SOUNDANALYSER
                void onPeakEnergy(ofxSoundAnalyser::FloatArg &arg);
                void onPitch(ofxSoundAnalyser::FloatArg &arg);
                void onRootMeanSquare(ofxSoundAnalyser::FloatArg &arg);
                void onFftMagnitudeSpectrum(ofxSoundAnalyser::FloatVecArg &arg);
                void onMelFrequencySpectrum(ofxSoundAnalyser::FloatVecArg &arg);
                void onSpectralCentroid(ofxSoundAnalyser::FloatArg &varg);
                void onSpectralCrest(ofxSoundAnalyser::FloatArg &varg);
                void onSpectralDifference(ofxSoundAnalyser::FloatArg &arg);
                void onSpectralFlatness(ofxSoundAnalyser::FloatArg &arg);
                void onZeroCrossingRate(ofxSoundAnalyser::FloatArg &arg);
                void onChord(ofxSoundAnalyser::FloatArg &arg);
                void onChromogram(ofxSoundAnalyser::FloatVecArg &arg);
#endif

                void newMidiMessage(ofxMidiMessage &message);
                void scanMidiInPorts();
                void setPreview(ofFbo fbo);
                ofFbo getFbo()
                {
                        return _fbo;
                }

        private:
                pdsp::Engine _engine;
                std::vector<track::base::pointer> _tracks;
                std::vector<ofxLiveSet::information> _sceneInformation;

                //outputs
                ofFbo _fbo;
                ofFbo _rawFbo;
                std::vector<ofFbo> _fbos;

                // inputs
                std::vector<std::shared_ptr<ofxMidiIn>> _midiIns;
                ofxMidiIn _midiIn;
                ofxOscReceiver _oscReceiver;
                ofxMidiMapper _midiMapper;
                ofxKeyMapper _keyMapper;
                ofxOscMapper _oscMapper;
#if OFXLIVESET_USE_SOUNDANALYSER
                ofxSoundAnalyser::Analyser _soundAnalyser;
#endif

                // parameters
                ofParameterGroup _parameters;
                ofParameterGroup _controls;
                ofParameter<std::string> _name;
                ofParameter<bool> _active;
                ofParameter<std::string> _timestampString;
                ofParameter<bool> _mute;
                ofParameter<float> _gain;

                ofParameterGroup _navigationControls;
                ofParameter<void> _previousTrackTrigger;
                ofParameter<void> _nextTrackTrigger;
                ofParameter<void> _previousClipTrigger;
                ofParameter<void> _nextClipTrigger;

                // TODO: remove vector
                ofParameterGroup _sceneTriggerGroup;
                std::vector<ofParameter<bool>> _sceneTriggers;

                ofParameterGroup _settings;
                ofParameter<bool> _defaultKeyMappingEnabled;
                ofParameter<bool> _oscControlEnabled;
                ofParameter<bool> _autoResizeGraphicTracksEnabled;

                ofxLiveSet::track::base::pointer _focusedTrack;
                ofxLiveSet::clip::base::pointer _focusedClip;

                u_int64_t _timestamp;
                u_int64_t _startedTimestamp;

                ofFbo _preview;

                bool _fullscreen = true;
                bool _showPreferences = false;
                bool _showBrowser = true;
                bool _showInfo = false;
                bool _showPreview = true;
                bool _showDemo = false;
                bool _showStyleEditor = false;

                ofxImGui::Gui _gui;

                ofxMovingAverage<float> _fps;
        };
}; // namespace ofxLiveSet
