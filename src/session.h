#pragma once
#include "./config.h"

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxPDSP.h"
#include "ofxMidi.h"
#include "ofxMidiMapper.h"
#include "ofxKeyMapper.h"
#include "ofxOscMapper.h"
#include "./tracks/base.h"
#include "./tracks/audio.h"
#include "./tracks/graphic.h"
#include "./clips/soundReactive.h"
#include "./clips/midiReactive.h"
#include "./gui/infoPanel.h"

#if OFXLIVESET_USE_SOUNDANALYSER
#include "ofxSoundAnalyser.h"
#endif

#if OFXLIVESET_USE_MQTTSYNCHRONISER
#include "utils/mqttSynchroniser.h"
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
        void setupGui(float panelWidth = 200);
        void setupAudioEngine(int id, int inChannels = 2, int outChannels = 2);
        void openMidiInPort(int index);
        void openVirtualMidiInPort(std::string name);
        void openMidiMapperInPort(int index);
        void openOscControlInPort(int port);
        void update();
        void draw();
        void drawGui();
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
        void triggerScene(int index);
        clip::base::pointer getClip(int track, int index);

        void renameScene(int index, std::string name);
        void onSceneTrigger(const void *sender, bool &value);
        void onActiveChange(bool &value);
        void onClipStarted(const void *sender, bool &value);
        void onInputChannelsChange(const void *sender, std::pair<int, int> &value);
        void onOscControlEnabledChange(bool &value);

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

        pdsp::Engine _engine;
        std::vector<track::base::pointer> _tracks;
        std::vector<ofxLiveSet::information> _sceneInformation;
        std::mutex _mutex;

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

        // TODO: remove vector
        ofParameterGroup _sceneTriggerGroup;
        std::vector<ofParameter<bool>> _sceneTriggers;

        ofParameterGroup _settings;
        ofParameter<bool> _defaultKeyMappingEnabled;
        ofParameter<bool> _oscControlEnabled;
        ofParameter<bool> _mqttSynchroniserEnabled;
        ofParameter<bool> _autoResizeGraphicTracksEnabled;

        ofParameter<int> _focusedTrack;
        ofParameter<int> _focusedClip;

        u_int64_t _timestamp;
        u_int64_t _startedTimestamp;

        // gui
        ofxPanel _scenesPanel;
        ofxPanel _midiMapperPanel;
        ofxPanel _keyMapperPanel;
        ofxPanel _oscMapperPanel;
        ofxPanel _settingsPanel;
        gui::infoPanel _infoPanel;

        ofxPanel _effectsPanel;

#if OFXLIVESET_USE_MQTTSYNCHRONISER
        ofxLiveSet::mqttSynchroniser _mqttSynchroniser;
#endif
    };
}; // namespace ofxLiveSet
