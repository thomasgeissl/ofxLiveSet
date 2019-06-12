#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxPDSP.h"
#include "ofxMidi.h"
#include "ofxSoundAnalyser.h"
#include "ofxMidiMapper.h"
#include "ofxKeyMapper.h"
#include "ofxOscMapper.h"
#include "./tracks/base.h"
#include "./tracks/audio.h"
#include "./tracks/graphic.h"
#include "./clips/soundReactive.h"
#include "./clips/midiReactive.h"
#include "./gui/infoPanel.h"

#include "utils/mqttSynchroniser.h"

namespace ofxLiveSet {
class session: public ofxMidiListener, public ofxSoundAnalyserListener {
public:
	session() : _soundAnalyser(ofxSoundAnalyser(8000)){
        ofSetEscapeQuitsApp(false);
		ofAddListener(ofEvents().update, this, &session::onUpdate, OF_EVENT_ORDER_AFTER_APP);
        ofAddListener(ofEvents().draw, this, &session::onDraw, OF_EVENT_ORDER_BEFORE_APP);
        ofAddListener(ofEvents().exit, this, &session::onExit, OF_EVENT_ORDER_AFTER_APP);
        // ofAddListener(ofEvents().keyPressed, this, &session::onKeyPressed, OF_EVENT_ORDER_AFTER_APP);

        _defaultKeyMappingEnabled.set("defaultKeyMappingEnabled", true);
        _oscControlEnabled.set("oscControlEnabled", true);
        _oscControlEnabled.addListener(this, &session::onOscControlEnabledChange);
        _mqttSynchroniserEnabled.set("mqttSynchroniserEnabled", true);
        _autoResizeGraphicTracksEnabled.set("autoResizeGraphicTracksEnabled", true);

        _settings.add(_defaultKeyMappingEnabled);
        _settings.add(_oscControlEnabled);
        _settings.add(_mqttSynchroniserEnabled);
        _settings.add(_autoResizeGraphicTracksEnabled);

        _engine.listDevices();
	}
    void setup() {
        // inputs
        _soundAnalyser.addListener(this);
        
        _midiIn.openVirtualPort("ofxLiveSet");
        _midiIn.ignoreTypes(false, false, false);
        _midiIn.addListener(this);
        _midiIn.setVerbose(true);


        for(auto & track : _tracks){
            track->setup();
        }
        
        
        _parameters.setName("session");
        _controls.setName("controls");
        _name.set("name", "session");
        _active.set("active", false);
        _timestampString.set("time", "0");
        _mute.set("mute", false);
        _gain.set("gain", 1, 0, 1); //TODO: does make gain sense for a session with audio, dmx and midi tracks?

//        _parameters.add(_name);
        _controls.add(_active);
        _controls.add(_timestampString);
        _controls.add(_mute);
        _controls.add(_gain);
        _parameters.add(_controls);

        _sceneTriggerGroup.setName("scenes");
        _parameters.add(_sceneTriggerGroup);

        _active.addListener(this, &session::onActiveChange);

        
        auto scenes = 0;
        for(auto track : _tracks){
            scenes = std::max(scenes, (int)(track->_clips.size()));
        }
        _sceneTriggers.resize(scenes);
        _sceneInformation.resize(scenes);
        auto i = 0;
        for(auto sceneTrigger : _sceneTriggers){
            sceneTrigger.set("scene "+ofToString(i++), false);
            _sceneTriggerGroup.add(sceneTrigger);
            sceneTrigger.addListener(this, &session::onSceneTrigger);
        }

        
        for(auto & track : _tracks){
            for(auto clip : track->_clips){
                ofAddListener(clip->_started, this, &session::onClipStarted);
            }
        }
        
        _focusedTrack.set("focusedTrack", 0);
        _focusedClip.set("focusedClip", 0);

        for(auto & track : _tracks){
            _midiMapper.addParameters(track->_controls, true);
            _keyMapper.addParameters(track->_controls);
            _oscMapper.addParameters(track->_controls);
            for(auto clip : track->_clips){
                _midiMapper.addParameters(clip->_parameters, true);
                _keyMapper.addParameters(clip->_parameters);
                _oscMapper.addParameters(clip->_parameters);
            }
        }
        _midiMapper.addParameter(_gain);

                
        if(ofFile::doesFileExist("mapping.midi.json")){
            _midiMapper.loadMapping(ofToDataPath("mapping.midi.json"));
        }
        if(ofFile::doesFileExist("mapping.key.json")){
            _keyMapper.loadMapping(ofToDataPath("mapping.key.json"));
        }
        if(ofFile::doesFileExist("mapping.osc.json")){
            _oscMapper.loadMapping(ofToDataPath("mapping.osc.json"));
        }

        _mqttSynchroniser.setup();
        for(auto & track : _tracks){
            std::string prefix = "ofxLiveSet/mqttSynchroniser/session/"+ofToString(track->_name)+"/parameters/";
            _mqttSynchroniser.addParameters(track->_parameters, prefix);
            prefix =  "ofxLiveSet/mqttSynchroniser/session/"+ofToString(track->_name)+"/ioParameters/";
            _mqttSynchroniser.addParameters(track->_ioParameters, prefix);
            for(auto & clip : track->_clips){
                prefix = "ofxLiveSet/mqttSynchroniser/session/"+ofToString(track->_name)+"/"+ofToString(clip->_name)+"/parameters/";
                _mqttSynchroniser.addParameters(clip->_parameters, prefix);
            }
        }

        _fbo.allocate(ofGetWidth(), ofGetHeight());
        _rawFbo.allocate(ofGetWidth(), ofGetHeight());
    }
    void setupGui(float panelWidth = 200){
        ofAddListener(ofEvents().windowResized, this, &session::onWindowResized, OF_EVENT_ORDER_AFTER_APP);

        ofxPanel::setDefaultWidth(panelWidth);
        ofxPanel::setDefaultFillColor(ofColor::green);
        _midiMapper.getParameters().setName("midi mapper");
        _midiMapperPanel.setup(_midiMapper.getParameters());
        _midiMapperPanel.setPosition(0, ofGetHeight()/2);
        _midiMapperPanel.setHeaderBackgroundColor(ofColor::green);
        _keyMapper.getParameters().setName("key mapper");
        _keyMapperPanel.setup(_keyMapper.getParameters());
        _keyMapperPanel.setPosition(_midiMapperPanel.getPosition().x, _midiMapperPanel.getPosition().y + _midiMapperPanel.getHeight());
        _keyMapperPanel.setHeaderBackgroundColor(ofColor::green);
        _oscMapper.getParameters().setName("osc mapper");
        _oscMapperPanel.setup(_oscMapper.getParameters());
        _oscMapperPanel.setPosition(_keyMapperPanel.getPosition().x, _keyMapperPanel.getPosition().y + _keyMapperPanel.getHeight());
        _oscMapperPanel.setHeaderBackgroundColor(ofColor::green);

        ofxPanel::setDefaultFillColor(ofColor::green);
//        _scenesPanel.setDefaultBorderColor(ofColor::green);
        _scenesPanel.setup(_parameters);
        _scenesPanel.setHeaderBackgroundColor(ofColor::green);
        _scenesPanel.setPosition(ofGetWidth() - _scenesPanel.getWidth(), 0);
        _scenesPanel.setUseTTF(true);

        auto x = 0;
        auto y = 0;
        for(auto & track : _tracks){
            track->setupGui(); 
            track->_gui.setPosition(x, y);
            track->_ioGui.setPosition(_midiMapperPanel.getPosition().x + _midiMapperPanel.getWidth(), ofGetHeight()/2);

            x += track->_gui.getWidth() + 1; //TODO: get border width

            for(auto clip : track->_clips){
                clip->_gui.setPosition(track->_ioGui.getPosition().x + track->_ioGui.getWidth(), ofGetHeight()/2);
            }
        }
        _infoPanel.setup();
    }
    void setupAudioEngine(int id, int inChannels = 2, int outChannels = 2){
        _engine.setDeviceID(id);
        _engine.setChannels(inChannels, outChannels);
        _engine.setup(44100, 512, 3); 

        // connect audio tracks
        for(auto & track : _tracks){
            auto audioTrack = dynamic_cast<ofxLiveSet::track::audio *>(track);
            if (audioTrack != nullptr) {
                _engine.audio_in(0) >> audioTrack->in("left");
                _engine.audio_in(1) >> audioTrack->in("right");

                audioTrack->out("left") >> _engine.audio_out(0);
                audioTrack->out("right") >> _engine.audio_out(1);

                ofAddListener(audioTrack->_inputsChanged, this, &session::onInputChannelsChange);
            }
        }
    }
    void openMidiMapperInPort(int index){
        _midiMapper.openMidiPort(index);
    }
    void openOscControlInPort(int port){
        _oscReceiver.setup(port);
    }
	void update(){
        _soundAnalyser.update();
        if(_mqttSynchroniserEnabled){
            _mqttSynchroniser.update();
        }

        if(_active){
            _timestamp = ofGetElapsedTimeMillis();
            _timestampString = ofToString((int)((_timestamp - _startedTimestamp)/1000));
            for (auto & track : _tracks)
            {
                track->update();
            }
        }
        auto i = 0;
        for (auto & track : _tracks)
        {
            track->_focused = _focusedTrack == i++;
            track->update();
        }

//    TODO: add more commands, check oscparamsync with multiple parameter groups having the same name, 
        while(_oscControlEnabled && _oscReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            _oscReceiver.getNextMessage(m);

            if(m.getAddress() == "/ofxSession/scene/trigger"){
                auto index = m.getArgAsInt(0);
                triggerScene(index);
            }
            else if(m.getAddress() == "/ofxSession/stop"){
                stop();
            }
            else if(m.getAddress() == "/ofxSession/track/stop"){
                auto index = m.getArgAsInt(0);
                if(index < _tracks.size()){
                    _tracks[index]->stop();
                }
            }
            else if(m.getAddress() == "/ofxSession/clip/toggle"){
                auto trackIndex = m.getArgAsInt(0);
                auto clipIndex = m.getArgAsInt(1);

                if(trackIndex < _tracks.size()){
                    auto track = _tracks[trackIndex];
                    if(clipIndex < track->_clips.size()){
                        auto clip = track->_clips[clipIndex];
                        clip->toggle();
                    }
                }
            }
        }
	}
	void draw(){
        _rawFbo.begin();
        ofClear(255,0);
		for (auto & track : _tracks){
            track->draw();
		}
        _rawFbo.end();
        _fbo.begin();
            ofClear(255,0);
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            ofSetColor(255, _gain * 255);
            _rawFbo.draw(0,0);
        _fbo.end();

	}
    void drawGui(){
        for (auto & track : _tracks){
            track->drawGui();
		}
        _scenesPanel.setPosition(ofGetWidth() - _scenesPanel.getWidth(),0); //TODO: only set position on resize

        _scenesPanel.draw();
        _midiMapperPanel.draw();
        _keyMapperPanel.draw();
        _oscMapperPanel.draw();

        if(_focusedTrack < _tracks.size() && _focusedClip < _tracks[_focusedTrack]->_clips.size()){
            _tracks[_focusedTrack]->_clips[_focusedClip]->_gui.draw();
        }
        _infoPanel.draw(0, ofGetHeight() - 100, ofGetWidth()/4, 100);
    }
    void exit(){
        if(ofFile::doesFileExist("mapping.midi.json")){
            ofFile::moveFromTo("mapping.midi.json", ofGetTimestampString()+"_mapping.midi.json");
        }
        _midiMapper.saveMapping(ofToDataPath("mapping.midi.json"));
        
        if(ofFile::doesFileExist("mapping.key.json")){
            ofFile::moveFromTo("mapping.key.json", ofGetTimestampString()+"_mapping.key.json");
        }
        _keyMapper.saveMapping(ofToDataPath("mapping.key.json"));
        
        if(ofFile::doesFileExist("mapping.osc.json")){
            ofFile::moveFromTo("mapping.osc.json", ofGetTimestampString()+"_mapping.osc.json");
        }
        _oscMapper.saveMapping(ofToDataPath("mapping.osc.json"));
    }
    
	void onUpdate(ofEventArgs &e){
		update();
	}

	void onDraw(ofEventArgs &e){
		draw();
	}
    void onExit(ofEventArgs &e){
        exit();
    }
    void onWindowResized(ofResizeEventArgs &e){
        onWindowResized(e.width, e.height);
    }
    void onWindowResized(float width, float height){
        _fbo.allocate(width, height);
        _rawFbo.allocate(width, height);
        _rawFbo.begin();
        ofClear(255,0);
        _rawFbo.end();
        for (auto track : _tracks){
            auto graphicTrack = dynamic_cast<ofxLiveSet::track::graphic *>(track);
            if(graphicTrack != nullptr){
                graphicTrack->resize(width, height);
            }
		}
    }
    void onKeyPressed(ofKeyEventArgs & e){
        int key = e.key;
        onKeyPressed(key);
    }
    void onKeyPressed(int key){
        _keyMapper.keyPressed(key);
        if(_defaultKeyMappingEnabled){
            switch(key){
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    triggerScene(key-48);
                    break;

                case OF_KEY_LEFT: {
                    _focusedTrack = std::max(0, _focusedTrack-1);
                    break;
                }
                case OF_KEY_RIGHT: {
                    _focusedTrack = std::min((int)(_tracks.size())-1, (int)((_focusedTrack+1) % _tracks.size()));
                    break;
                }
                case OF_KEY_UP: {
                    _focusedClip = std::max(0, _focusedClip-1);
                    break;
                }
                case OF_KEY_DOWN: {
                    _focusedClip = std::min((int)(_tracks[_focusedTrack]->_clips.size())-1, _focusedClip+1);
                    break;
                }
                case OF_KEY_RETURN: {
                    auto clip = getClip(_focusedTrack, _focusedClip);
                    if(clip != nullptr){
                        clip->toggle();
                    }
                    break;
                }
                case ' ': {
                    toggle();
                }
            }
        }
    }
    void toggle(){
        _active = !_active;
    }
	void start(){
        _startedTimestamp = ofGetElapsedTimeMillis();
        for (auto track : _tracks)
        {
            track->start();
        }
	}
	void pause()
	{
	}
	void stop(){
        _timestampString = "";
		for (auto track : _tracks)
		{
			track->stop();
		}
	}
    track::base * addTrack(track::base *track)
	{
		_tracks.push_back(track);
        return track;
	}
    
    void triggerScene(int index){
        for (auto track : _tracks){
            track->stop();
            track->trigger(index);
        }
        if(index < _sceneInformation.size()){
            _infoPanel.setInfo(_sceneInformation[index]);
        }
    }
    
    clip::base* getClip(int track, int index){
        if(track >= _tracks.size()){ return nullptr; }
        if(index >= _tracks[track]->_clips.size()){ return nullptr; }
        return _tracks[track]->_clips[index];
    }
    
    void renameScene(int index, std::string name){
        if(index >= _sceneTriggers.size()){
            ofLogError("ofxLiveSet::session") << "cannot rename scene. index (" << index << ") out of bounds. ("<<_sceneTriggers.size()<<")";
            return;
        }
        _sceneTriggers[index].setName(name);
    }
    void onSceneTrigger(const void* sender, bool & value) {
        if(!value){ return; }
        auto i = 0;
        for(auto sceneTrigger : _sceneTriggers) {
            if(sceneTrigger){
                sceneTrigger = false;
                triggerScene(i);
            }
            i++;
        }
    }
    
    void onActiveChange(bool & value){
        if(value){
            start();
        }else{
            stop();
        }
    }

    void onClipStarted(const void* sender, bool & value) {
        auto clip = (clip::base *) (sender);
        auto nullClip = dynamic_cast<ofxLiveSet::clip::nullClip *>(clip);
        if (nullClip != nullptr){return;}
        if(!_active){
            _active = true;
        }
        auto trackIndex = 0;
        auto clipIndex = 0;
        for(auto & track : _tracks){
            for(auto & clipToCompareWith : track->_clips){
                if(clip == clipToCompareWith){
                    _focusedTrack = trackIndex;
                    _focusedClip = clipIndex;
                }
                clipIndex++;
            }
            clipIndex = 0;
            trackIndex++;
        }
    }

    void onInputChannelsChange(const void* sender, std::pair<int, int> & value){
        auto audioTrack = (track::audio *) (sender);
        if(audioTrack != nullptr){
            // TODO: how can only audio_in be unpatched
            audioTrack->in("left").disconnectIn();
            audioTrack->in("right").disconnectIn();

            auto leftChannel = value.first;
            auto rightChannel = value.second;

            if(leftChannel != -1){
                _engine.audio_in(leftChannel) >> audioTrack->in("left");
            }
            if(rightChannel != -1){
                _engine.audio_in(rightChannel) >> audioTrack->in("right");
            }
            // TODO: reconnect clips
            for(auto & clip : audioTrack->_clips){
                auto audioClip = (clip::audio *) (clip);
                if(audioClip != nullptr){
                    audioClip->out("left") >> audioTrack->_leftAmp;
                    audioClip->out("right") >> audioTrack->_rightAmp;
                }
            }
        }
    }
    
    void onOscControlEnabledChange(bool & value){
        if(value){
            _oscReceiver.start();
        }else{
            _oscReceiver.stop();
        }
    }
    void onPeakEnergy(std::pair<int, float> & value){
        for(auto & track : _tracks){
            auto clip = dynamic_cast<ofxLiveSet::clip::soundReactive *>(track->_clip);
            if (clip != nullptr) {
                clip->setPeakEnergy(value.first, value.second);
            }
        }
    }
    void onPitch(std::pair<int, float> & value){
        int note = std::round((value.second > 0 ? 17.3123405046 * log(.12231220585 * value.second) : -1500));
    
        for(auto & track : _tracks){
            auto clip = dynamic_cast<ofxLiveSet::clip::soundReactive *>(track->_clip);
            if (clip != nullptr) {
                clip->setPitch(value.first, note);
            }
        }
    }
    void onRootMeanSquare(std::pair<int, float> & value){
        for(auto & track : _tracks){
            auto clip = dynamic_cast<ofxLiveSet::clip::soundReactive *>(track->_clip);
            if (clip != nullptr) {
                clip->setRootMeanSquare(value.first, value.second);
            }
        }
    }
    void onFftMagnitudeSpectrum(std::pair<int, std::vector<float>> & value){
        for(auto & track : _tracks){
            auto clip = dynamic_cast<ofxLiveSet::clip::soundReactive *>(track->_clip);
            if (clip != nullptr) {
                clip->setFftMagnitudeSpectrum(value.first, value.second);
            }
        }
    }
    void onMelFrequencySpectrum(std::pair<int, std::vector<float>> & value){ 
        for(auto & track : _tracks){
            auto clip = dynamic_cast<ofxLiveSet::clip::soundReactive *>(track->_clip);
            if (clip != nullptr) {
                clip->setMelFrequencySpectrum(value.first, value.second);
            }
        }
    }
    void newMidiMessage(ofxMidiMessage& message){
        for(auto & track : _tracks){
            auto clip = dynamic_cast<ofxLiveSet::clip::midiReactive *>(track->_clip);
            if (clip != nullptr) {
                auto status = message.status;
                if(status == MIDI_NOTE_ON){
                    clip->setNoteOn(message.pitch, message.velocity);
                }else if(status == MIDI_NOTE_OFF){
                    clip->setNoteOff(message.pitch, message.velocity);
                }
            }
        }
    }
    pdsp::Engine _engine;
	std::vector<track::base *> _tracks;
    std::vector<ofxLiveSet::information> _sceneInformation;
    std::mutex _mutex;

    //outputs
    ofFbo _fbo;
    ofFbo _rawFbo;
    std::vector<ofFbo> _fbos;

    // inputs
    ofxMidiIn _midiIn;
    ofxSoundAnalyser _soundAnalyser;
    ofxOscReceiver _oscReceiver;
    ofxMidiMapper _midiMapper;
    ofxKeyMapper _keyMapper;
    ofxOscMapper _oscMapper;

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

    ofxLiveSet::mqttSynchroniser _mqttSynchroniser;
};
}; // namespace ofxLiveSet
