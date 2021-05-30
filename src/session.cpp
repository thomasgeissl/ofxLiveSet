#include "./config.h"

#include "ofMain.h"
#include "./session.h"
#include "./gui/Theme.h"

#include "./session.soundAnalyser.cpp"
#include "./utils/IconsFontAwesome5.h"
#include "./gui/ofxImGuiHelpers.h"


ofxLiveSet::session::session() : _fps(ofxMovingAverage<float>(60))
#if OFXLIVESET_USE_SOUNDANALYSER
    , _soundAnalyser(ofxSoundAnalyser::Analyser(8000))
#endif
{
    ofSetEscapeQuitsApp(false);
    ofAddListener(ofEvents().update, this, &ofxLiveSet::session::onUpdate, OF_EVENT_ORDER_AFTER_APP);
    ofAddListener(ofEvents().draw, this, &ofxLiveSet::session::onDraw, OF_EVENT_ORDER_BEFORE_APP);
    ofAddListener(ofEvents().exit, this, &ofxLiveSet::session::onExit, OF_EVENT_ORDER_AFTER_APP);
    // ofAddListener(ofEvents().keyPressed, this, &ofxLiveSet::session::onKeyPressed, OF_EVENT_ORDER_AFTER_APP);

    _gui.setup();
    // ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::GetIO().MouseDrawCursor = false;
    _gui.setTheme(new Theme());

    _defaultKeyMappingEnabled.set("defaultKeyMappingEnabled", true);
    _oscControlEnabled.addListener(this, &ofxLiveSet::session::onOscControlEnabledChange);
    _oscControlEnabled.set("oscControlEnabled", false);
    _autoResizeGraphicTracksEnabled.set("autoResizeGraphicTracksEnabled", true);

    _settings.add(_defaultKeyMappingEnabled);
    _settings.add(_oscControlEnabled);
    _settings.add(_autoResizeGraphicTracksEnabled);

    _engine.listDevices();
}
void ofxLiveSet::session::setup()
{
    // inputs
    _soundAnalyser.addListener(this);

    _midiIn.openVirtualPort("ofxLiveSet");
    _midiIn.ignoreTypes(false, false, false);
    _midiIn.addListener(this);
    _midiIn.setVerbose(true);

    for (auto &track : _tracks)
    {
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

    _active.addListener(this, &ofxLiveSet::session::onActiveChange);

    auto scenes = 0;
    for (auto track : _tracks)
    {
        scenes = std::max(scenes, (int)(track->_clips.size()));
    }
    _sceneTriggers.resize(scenes);
    _sceneInformation.resize(scenes);
    auto i = 0;
    for (auto sceneTrigger : _sceneTriggers)
    {
        sceneTrigger.set("scene " + ofToString(i++), false);
        _sceneTriggerGroup.add(sceneTrigger);
        sceneTrigger.addListener(this, &ofxLiveSet::session::onSceneTrigger);
    }

    for (auto &track : _tracks)
    {
        for (auto clip : track->_clips)
        {
            ofAddListener(clip->_started, this, &ofxLiveSet::session::onClipStarted);
        }
    }

    for (auto &track : _tracks)
    {
        _midiMapper.addParameters(track->_controls, true);
        _keyMapper.addParameters(track->_controls);
        _oscMapper.addParameters(track->_controls);
        for (auto clip : track->_clips)
        {
            _midiMapper.addParameters(clip->_parameters, true);
            _keyMapper.addParameters(clip->_parameters);
            _oscMapper.addParameters(clip->_parameters);
        }
    }
    _midiMapper.addParameter(_gain);

    if (ofFile::doesFileExist("mappings/mapping.midi.json"))
    {
        _midiMapper.loadMapping(ofToDataPath("mappings/mapping.midi.json"));
    }
    if (ofFile::doesFileExist("mappings/mapping.key.json"))
    {
        _keyMapper.loadMapping(ofToDataPath("mappings/mapping.key.json"));
    }
    if (ofFile::doesFileExist("mappings/mapping.osc.json"))
    {
        _oscMapper.loadMapping(ofToDataPath("mappings/mapping.osc.json"));
    }

    _fbo.allocate(ofGetWidth(), ofGetHeight());
    _rawFbo.allocate(ofGetWidth(), ofGetHeight());


       // Add fontawesome fonts by merging new glyphs
    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    _gui.addFont("fa-regular-400.ttf", 12.f, &config, icon_ranges);
}

void ofxLiveSet::session::setupAudioEngine(int id, int inChannels, int outChannels)
{
    _engine.setDeviceID(id);
    _engine.setChannels(inChannels, outChannels);
    _engine.setup(44100, 512, 3);

    // connect audio tracks
    for (auto &track : _tracks)
    {
        auto audioTrack = std::dynamic_pointer_cast<ofxLiveSet::track::audio>(track);
        if (audioTrack != nullptr)
        {
            _engine.audio_in(0) >> audioTrack->in("left");
            _engine.audio_in(1) >> audioTrack->in("right");

            audioTrack->out("left") >> _engine.audio_out(0);
            audioTrack->out("right") >> _engine.audio_out(1);

            ofAddListener(audioTrack->_inputsChanged, this, &ofxLiveSet::session::onInputChannelsChange);
        }
    }
}
void ofxLiveSet::session::openMidiInPort(int index)
{
    _midiIn.openPort(index);
}
void ofxLiveSet::session::openVirtualMidiInPort(std::string name)
{
    _midiIn.openVirtualPort(name);
}
void ofxLiveSet::session::openMidiMapperInPort(int index)
{
    _midiMapper.openMidiPort(index);
}
void ofxLiveSet::session::openOscControlInPort(int port)
{
    _oscReceiver.setup(port);
}
void ofxLiveSet::session::update()
{
#if OFXLIVESET_USE_SOUNDANALYSER
    _soundAnalyser.update();
#endif

    if (_active)
    {
        _timestamp = ofGetElapsedTimeMillis();
        _timestampString = ofToString((int)((_timestamp - _startedTimestamp) / 1000));
        for (auto &track : _tracks)
        {
            track->update();
        }
    }

    //    TODO: add more commands, check oscparamsync with multiple parameter groups having the same name,
    while (_oscControlEnabled && _oscReceiver.hasWaitingMessages())
    {
        ofxOscMessage m;
        _oscReceiver.getNextMessage(m);

        if (m.getAddress() == "/ofxSession/scene/trigger")
        {
            auto index = m.getArgAsInt(0);
            triggerScene(index);
        }
        else if (m.getAddress() == "/ofxSession/stop")
        {
            stop();
        }
        else if (m.getAddress() == "/ofxSession/track/stop")
        {
            auto index = m.getArgAsInt(0);
            if (index < _tracks.size())
            {
                _tracks[index]->stop();
            }
        }
        else if (m.getAddress() == "/ofxSession/clip/toggle")
        {
            auto trackIndex = m.getArgAsInt(0);
            auto clipIndex = m.getArgAsInt(1);

            if (trackIndex < _tracks.size())
            {
                auto track = _tracks[trackIndex];
                if (clipIndex < track->_clips.size())
                {
                    auto clip = track->_clips[clipIndex];
                    clip->toggle();
                }
            }
        }
    }
}
void ofxLiveSet::session::draw()
{
    _rawFbo.begin();
    ofClear(255, 0);
    for (auto &track : _tracks)
    {
        track->draw();
    }
    _rawFbo.end();

    _fbo.begin();
    ofClear(255, 0);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(255, _gain * 255);
    _rawFbo.draw(0, 0);
    _fbo.end();
}
void ofxLiveSet::session::drawGui()
{
    _gui.begin();
    drawMenuGui();
 

        auto yOffset = 19;
        auto height = ofGetHeight() - yOffset;
        ImGui::SetNextWindowPos(ImVec2(0, yOffset), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(ofGetWidth(), height));
        ImGui::Begin("Splitter test", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

        static float w = 200.0f;
        static float lowerW = 800.0f;
        static float h = 400.0f;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));

        ImGui::BeginChild("child1", ImVec2(w, h), true);
        if(_midiMapper.getActiveMappingParameter()){
            drawMidiMapperGui();
        }
        // else if(_keyMapper.getActiveMappingParameter()){
        //     drawKeyMapperGui();
        // }
        else if(_showBrowser)
        {
            drawBrowserGui();
        }
        ImGui::EndChild();


        ImGui::SameLine();
        ImGui::InvisibleButton("vsplitter", ImVec2(8.0f,h)); 
        if (ImGui::IsItemActive())
            w += ImGui::GetIO().MouseDelta.x;
        ImGui::SameLine();


        ImGui::BeginChild("child2", ImVec2(0, h), true);
            drawSessionGui();
        ImGui::EndChild();


        ImGui::InvisibleButton("hsplitter", ImVec2(-1,8.0f));
        if (ImGui::IsItemActive())
            h += ImGui::GetIO().MouseDelta.y;


        ImGui::BeginChild("child3", ImVec2(lowerW,-1), true);
            if(_showInfo){
                drawInfoGui();
                auto padding = ImVec2(10, 0);
                ImVec2 p0 = ImGui::GetCursorScreenPos();
                ImGui::SetCursorScreenPos(p0 + padding);
                ImGui::SameLine();
            }
            drawTrackGui();
            ImGui::SameLine();
            drawClipGui();
        ImGui::EndChild();


        ImGui::SameLine();
        ImGui::InvisibleButton("lvsplitter", ImVec2(8.0f, -1));
        if (ImGui::IsItemActive())
            lowerW += ImGui::GetIO().MouseDelta.x;
        ImGui::SameLine();


        ImGui::BeginChild("child4", ImVec2(0,-1));
            if(_showPreview){
                drawPreviewGui();
            }
        ImGui::EndChild();



        ImGui::PopStyleVar();
        ImGui::End();


    drawPreferencesGui();
    if(_showDemo){
        ImGui::ShowDemoWindow();
    }
    if(_showStyleEditor){
        ImGui::Begin("Dear ImGui Style Editor");
        ImGui::ShowStyleEditor();
        ImGui::End();
    }
    _gui.end();
}
void ofxLiveSet::session::drawMenuGui()
{
       if(ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("ofxLiveSet"))
        {
            if(ImGui::MenuItem("About"))
            {
            }
            if(ImGui::MenuItem("Preferences"))
            {
                ImGui::OpenPopup("Preferences");
            }
            if(ImGui::MenuItem("Quit"))
            {
                ofExit();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("New"))
            {
                //Do something
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Browser", NULL, _showBrowser)) {
                _showBrowser = !_showBrowser;
            }
            if (ImGui::MenuItem("Info", NULL, _showInfo)) {
                _showInfo = !_showInfo;
            }
            if (ImGui::MenuItem("Preview", NULL, _showPreview)) {
                _showPreview = !_showPreview;
            }
            if (ImGui::MenuItem("Demo", NULL, _showDemo)) {
                _showDemo = !_showDemo;
            }
            if (ImGui::MenuItem("Style editor", NULL, _showStyleEditor)) {
                _showStyleEditor = !_showStyleEditor;
            }
            ImGui::EndMenu();
        }

        if(_active)
        {
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (float)24);
            auto time = ofGetElapsedTimeMillis() - _startedTimestamp;
            std::string text = ofToString(time/1000) + +":" +ofToString(time - time/1000, 2);
            ImGui::Text(text.c_str());
        }
        ImGui::SameLine(ImGui::GetWindowWidth()-150);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 1, 1.0));
        ImGui::Button("Key");
        ImGui::PopStyleColor(1);
        ImGui::SameLine();
        bool activeMidiMapper = _midiMapper.getActiveMappingParameter();
        if(activeMidiMapper)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0, 0, 0, 1.0));
            if(ImGui::Button("MIDI"))
            {
                _midiMapper.setActiveMapping(false);
            }
            ImGui::PopStyleColor(1);
        }
        else{
            if(ImGui::Button("MIDI"))
            {
                _midiMapper.setActiveMapping(true);
            }
        }
        ImGui::SameLine();
        auto fps = ofToString((int)(_fps.add(ofGetFrameRate())));
        fps += "fps";
        ImGui::Text(fps.c_str());
        ImGui::EndMainMenuBar();
    }
}
void ofxLiveSet::session::drawBrowserGui()
{
    ImGui::Text("browserGui");
}
void ofxLiveSet::session::drawSessionGui()
{
    // ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor(255, 0, 0));
    
    ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersH |ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
    if (ImGui::BeginTable("table_scrollx", _tracks.size()+1, flags))
    {
        ImGui::TableSetupScrollFreeze(1, 1);
        ImGui::TableSetupColumn("Scene", ImGuiTableColumnFlags_NoHide | ImGuiTableFlags_SizingFixedSame);
        auto maxNumberOfClips = 0;
        for(auto & track : _tracks){
            if(track->_clips.size() > maxNumberOfClips){
                maxNumberOfClips = track->_clips.size();
            }
            ImGui::TableSetupColumn(ofToUpper(track->getName()).c_str(), ImGuiTableColumnFlags_WidthFixed, 100);
        }
        ImGui::TableHeadersRow();


        // stop buttons
        ImGui::TableNextRow();
        for(auto i = 0; i < _tracks.size() + 1; i++)
        {
            ImGui::TableSetColumnIndex(i);
            std::string label = "stop##track_"; label += ofToString(i);
            ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(40.0f/255, 0.0/255, 40.0/255, 1.000f));
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
            if(ImGui::Button(label.c_str())){
                if(i == 0){
                    stop();
                }else{
                    _tracks[i - 1]->stop();
                }
            }
        }
            ImGui::TableNextRow();
        for(auto i = 0; i < _tracks.size() + 1; i++)
        {
            ImGui::TableSetColumnIndex(i);
            std::string label = "solo##track_"; label += ofToString(i);
            if(i == 0){
            }else{
                bool value = _tracks[i-1]->isSoloed();
            ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(40.0f/255, 0.0/255, 40.0/255, 1.000f));
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                if(ImGui::Checkbox(label.c_str(), &value)){
                    _tracks[i-1]->setSolo(value);
                }
            }
        }
        ImGui::TableNextRow();
        for(auto i = 0; i < _tracks.size() + 1; i++)
        {
            ImGui::TableSetColumnIndex(i);
            std::string label = "mute##track_"; label += ofToString(i);

            if(i == 0){
            }else{
                bool value = _tracks[i-1]->isMuted();
            ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(40.0f/255, 0.0/255, 40.0/255, 1.000f));
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                if(ImGui::Checkbox(label.c_str(), &value)){
                    _tracks[i-1]->mute(value);
                }
            }
        }
        ImGui::Separator();

        for (int row = 0; row < maxNumberOfClips; row++)
        {
            ImGui::TableNextRow();
    
            for (int column = 0; column < _tracks.size() + 1; column++)
            {
                auto track = column == 0 || column > _tracks.size() ? nullptr : _tracks[column -1];
                // Both TableNextColumn() and TableSetColumnIndex() return true when a column is visible or performing width measurement.
                // Because here we know that:
                // - A) all our columns are contributing the same to row height
                // - B) column 0 is always visible,
                // We only always submit this one column and can skip others.
                // More advanced per-column clipping behaviors may benefit from polling the status flags via TableGetColumnFlags().
                if (!ImGui::TableSetColumnIndex(column) && column > 0)
                    continue;
                if (column == 0){
                    ImGui::GetStyle().Colors;
                    ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(1.000f, 0.391f, 0.000f, 1.000f));
                    ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                    if(row < _sceneTriggers.size()){
                        auto scene = _sceneTriggers[row];
                        // ImGui::Text("Line %d", row);
                        bool value = false;
                        if(ImGui::Checkbox(scene.getName().c_str(), &value )){
                            triggerScene(row);
                        }
                    }
                }
                else{
                    if(track){
                        auto clip = track->getClip(row);
                        if(clip){
                            auto active = clip->isActive();
                            if(active)
                            {
                                ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(1.000f, 0.391f, 0.000f, 1.000f));
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                            }
                            std::string id = "##"; id+=clip->getName();
                            if(ImGui::Checkbox(id.c_str(), &active )){
                                clip->setActive(true);
                            }
                            if(std::dynamic_pointer_cast<ofxLiveSet::clip::nullClip>(clip)){
                            }else{
                                ImGui::SameLine();
                                if(ImGui::Button((clip->getName()+"##focus").c_str())){
                                    _focusedClip = clip;
                                    _focusedTrack = getTrack(clip);
                                }
                            }
                        }
                    }
                }
            }
        }
        ImGui::EndTable();
    }
    // ImGui::PopStyleColor();
}
void ofxLiveSet::session::drawInfoGui()
{
    ImGui::BeginChild("info", ImVec2(200, 0), true);
        ImGui::Text("info");
    ImGui::EndChild();
}
void ofxLiveSet::session::drawTrackGui()
{
    ImGui::BeginChild("track", ImVec2(200, 0), true);
    // ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(1.000f, 0.391f, 0.000f, 1.000f));
    // ImGui::SetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
    // ImGui::Button("trackbutton");
    ImGui::EndChild();
}
void ofxLiveSet::session::drawClipGui()
{
    auto title = !_focusedClip ? "clip" : _focusedClip->getName();
    title += "##clipGui";
    ImGui::BeginChild(title.c_str(), ImVec2(0, 0), true);
        if(_focusedClip){
            ofxImGuiHelpers::ParameterGroup(_focusedClip->_parameters);
        }
    ImGui::EndChild();
}
void ofxLiveSet::session::drawPreviewGui()
{
    if(_preview.isAllocated()){
        ImTextureID textureID = ( ImTextureID )( uintptr_t )_preview.getTexture().getTextureData().textureID ;
        auto size = ImGui::GetContentRegionAvail() ; // for example
        ImGui::Image( textureID, glm::vec2(_preview.getWidth(), _preview.getHeight())) ;
        // ofxImGui::AddImage(_preview.getTexture(), glm::vec2(_preview.getWidth(), _preview.getHeight()));
    }
}
void ofxLiveSet::session::drawMidiMapperGui()
{
    if(ImGui::Button("clear"))
    {
        _midiMapper.clear();
    }
}
void ofxLiveSet::session::drawKeyMapperGui()
{
}
void ofxLiveSet::session::drawOscMapperGui()
{
}
void ofxLiveSet::session::drawPreferencesGui()
{
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("Preferences", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ofLogNotice() << "pref popup modal";
            ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
            ImGui::Separator();

            //static int unused_i = 0;
            //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

            static bool dont_ask_me_next_time = false;
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
            ImGui::PopStyleVar();

            if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
}
void ofxLiveSet::session::exit()
{
    std::string mappingsDirectoryPath = ofToDataPath("mappings");
    std::string mappingsBackupsDirectoryPath = ofToDataPath("mappings/backups");
    ofDirectory mappingsDirectory(mappingsDirectoryPath);
    ofDirectory mappingsBackupsDirectory(mappingsBackupsDirectoryPath);
    if (!mappingsDirectory.exists())
    {
        mappingsDirectory.create(true);
    }
    if (!mappingsBackupsDirectory.exists())
    {
        mappingsBackupsDirectory.create(true);
    }
    if (ofFile::doesFileExist("mappings/mapping.midi.json"))
    {
        ofFile::moveFromTo("mappings/mapping.midi.json", "mappings/backups/" + ofGetTimestampString() + "_mapping.midi.json");
    }
    _midiMapper.saveMapping(ofToDataPath("mappings/mapping.midi.json"));

    if (ofFile::doesFileExist("mappings/mapping.key.json"))
    {
        ofFile::moveFromTo("mappings/mapping.key.json", "mappings/backups/" + ofGetTimestampString() + "_mapping.key.json");
    }
    _keyMapper.saveMapping(ofToDataPath("mappings/mapping.key.json"));

    if (ofFile::doesFileExist("mappings/mapping.osc.json"))
    {
        ofFile::moveFromTo("mappings/mapping.osc.json", "mappings/backups/" + ofGetTimestampString() + "_mapping.osc.json");
    }
    _oscMapper.saveMapping(ofToDataPath("mappings/mapping.osc.json"));
}

void ofxLiveSet::session::onUpdate(ofEventArgs &e)
{
    update();
}

void ofxLiveSet::session::onDraw(ofEventArgs &e)
{
    draw();
}
void ofxLiveSet::session::onExit(ofEventArgs &e)
{
    exit();
}
void ofxLiveSet::session::onWindowResized(ofResizeEventArgs &e)
{
    onWindowResized(e.width, e.height);
}
void ofxLiveSet::session::onWindowResized(float width, float height)
{
    _fbo.allocate(width, height);
    _rawFbo.allocate(width, height);
    _rawFbo.begin();
    ofClear(255, 0);
    _rawFbo.end();
    for (auto track : _tracks)
    {
        auto graphicTrack = std::dynamic_pointer_cast<ofxLiveSet::track::graphic>(track);
        if (graphicTrack != nullptr)
        {
            graphicTrack->resize(width, height);
        }
    }
}
void ofxLiveSet::session::onKeyPressed(ofKeyEventArgs &e)
{
    int key = e.key;
    onKeyPressed(key);
}
void ofxLiveSet::session::onKeyPressed(int key)
{
    _keyMapper.keyPressed(key);
    if (_defaultKeyMappingEnabled)
    {
        switch (key)
        {
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
            triggerScene(key - 48);
            break;

        case OF_KEY_LEFT:
        {
            // _focusedTrack = std::max(0, _focusedTrack - 1);
            break;
        }
        case OF_KEY_RIGHT:
        {
            // _focusedTrack = std::min((int)(_tracks.size()) - 1, (int)((_focusedTrack + 1) % _tracks.size()));
            break;
        }
        case OF_KEY_UP:
        {
            // _focusedClip = std::max(0, _focusedClip - 1);
            break;
        }
        case OF_KEY_DOWN:
        {
            // _focusedClip = std::min((int)(_tracks[_focusedTrack]->_clips.size()) - 1, _focusedClip + 1);
            break;
        }
        case OF_KEY_RETURN:
        {
            if (_focusedClip != nullptr)
            {
                _focusedClip->toggle();
            }
            break;
        }
        case ' ':
        {
            toggle();
        }
        }
    }
}
void ofxLiveSet::session::toggle()
{
    _active = !_active;
}
void ofxLiveSet::session::start()
{
    _startedTimestamp = ofGetElapsedTimeMillis();
    for (auto track : _tracks)
    {
        track->start();
    }
}
void ofxLiveSet::session::pause()
{
}
void ofxLiveSet::session::stop()
{
    _timestampString = "";
    for (auto track : _tracks)
    {
        track->stop();
    }
}
ofxLiveSet::track::base::pointer ofxLiveSet::session::addTrack(track::base::pointer track)
{
    _tracks.push_back(track);
    return track;
}

void ofxLiveSet::session::triggerScene(int index)
{
    for (auto track : _tracks)
    {
        track->stop();
        track->trigger(index);
    }
    if (index < _sceneInformation.size())
    {
        // _infoPanel.setInfo(_sceneInformation[index]);
    }
}

ofxLiveSet::clip::base::pointer ofxLiveSet::session::getClip(int track, int index)
{
    if (track >= _tracks.size())
    {
        return nullptr;
    }
    if (index >= _tracks[track]->_clips.size())
    {
        return nullptr;
    }
    return _tracks[track]->_clips[index];
}
std::vector<ofxLiveSet::track::base::pointer> ofxLiveSet::session::getTracks()
{
    return _tracks;
}
ofxLiveSet::track::base::pointer ofxLiveSet::session::getTrack(ofxLiveSet::clip::base::pointer clip)
{
    for(auto track : _tracks)
    {
        for(auto clipToCompareWith : track->getClips())
        {
            if(clipToCompareWith == clip) return track;
        }
    }
    return nullptr;
}

void ofxLiveSet::session::renameScene(int index, std::string name)
{
    if (index >= _sceneTriggers.size())
    {
        ofLogError("ofxLiveSet::session") << "cannot rename scene. index (" << index << ") out of bounds. (" << _sceneTriggers.size() << ")";
        return;
    }
    _sceneTriggers[index].setName(name);
}
void ofxLiveSet::session::onSceneTrigger(const void *sender, bool &value)
{
    if (!value)
    {
        return;
    }
    auto i = 0;
    for (auto sceneTrigger : _sceneTriggers)
    {
        if (sceneTrigger)
        {
            sceneTrigger = false;
            triggerScene(i);
        }
        i++;
    }
}

void ofxLiveSet::session::onActiveChange(bool &value)
{
    if (value)
    {
        start();
    }
    else
    {
        stop();
    }
}

void ofxLiveSet::session::onClipStarted(const void *sender, bool &value)
{
    auto clip = (clip::base *)(sender);
    if (dynamic_cast<ofxLiveSet::clip::nullClip *>(clip) != nullptr)
    {
        return;
    }
    if (!_active)
    {
        _active = true;
    }
    for (auto &track : _tracks)
    {
        for (auto &clipToCompareWith : track->_clips)
        {
            if (clip == clipToCompareWith.get())
            {
                _focusedTrack = track;
                // _focusedClip = clip;
                // _focusedClip = clipIndex;
            }
        }
    }
}

void ofxLiveSet::session::onInputChannelsChange(const void *sender, std::pair<int, int> &value)
{
    auto audioTrack = (track::audio *)(sender);
    if (audioTrack != nullptr)
    {
        // TODO: how can only audio_in be unpatched
        audioTrack->in("left").disconnectIn();
        audioTrack->in("right").disconnectIn();

        auto leftChannel = value.first;
        auto rightChannel = value.second;

        if (leftChannel != -1)
        {
            _engine.audio_in(leftChannel) >> audioTrack->in("left");
        }
        if (rightChannel != -1)
        {
            _engine.audio_in(rightChannel) >> audioTrack->in("right");
        }
        // TODO: reconnect clips
        for (auto &clip : audioTrack->_clips)
        {
            auto audioClip = std::dynamic_pointer_cast<clip::audio>(clip);
            if (audioClip != nullptr)
            {
                audioClip->out("left") >> audioTrack->_leftAmp;
                audioClip->out("right") >> audioTrack->_rightAmp;
            }
        }
    }
}

void ofxLiveSet::session::onOscControlEnabledChange(bool &value)
{
    if (value)
    {
        _oscReceiver.start();
    }
    else
    {
        _oscReceiver.stop();
    }
}

void ofxLiveSet::session::newMidiMessage(ofxMidiMessage &message)
{
    for (auto &track : _tracks)
    {
        auto clip = dynamic_pointer_cast<ofxLiveSet::clip::midiReactive>(track->_clip);
        if (clip != nullptr)
        {
            auto status = message.status;
            if (status == MIDI_NOTE_ON)
            {
                clip->setNoteOn(message.pitch, message.velocity);
            }
            else if (status == MIDI_NOTE_OFF)
            {
                clip->setNoteOff(message.pitch, message.velocity);
            }
        }
    }
}

void ofxLiveSet::session::setPreview(ofFbo fbo){
    _preview = fbo;
}
