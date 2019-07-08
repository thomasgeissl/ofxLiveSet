#pragma once
#include "ofMain.h"
#include "./graphic.h"

namespace ofxLiveSet
{
    namespace clip
    {
        class videoPlayer : public graphic
        {
        public:
            typedef std::shared_ptr<videoPlayer> pointer;
            static pointer create(std::string path, std::string name="")
            {
                return std::make_shared<videoPlayer>(path, name);
            }
            videoPlayer(std::string path, std::string name = "") : graphic(name), _path(path){
                _name = name;
                _active.setName(_name);
                _openFileChooser.set("openFileChooser");
                _openFileChooser.addListener(this, &videoPlayer::onOpenFileChooser);
                _loop.set("loop", true);
                _speed.set("speed", 1, 0, 10);
                _speed.addListener(this, &videoPlayer::onSpeedChange);
                _volume.set("volume", 0, 0, 1);
                _volume.addListener(this, &videoPlayer::onVolumeChange);

                _parameters.add(_openFileChooser);
                _parameters.add(_loop);
                _parameters.add(_speed);
                _parameters.add(_volume);

                // _player.load(path);
                _player.setVolume(_volume);
            }
            void update(){
                _player.update();
                // TODO: set only on change via listener
                if(_loop){
                    _player.setLoopState(OF_LOOP_NORMAL);
                }else{
                    _player.setLoopState(OF_LOOP_NONE);
                }
                if (_player.isFrameNew()){
                    begin();
                    _player.draw(-_player.getWidth()/2,-_player.getHeight()/2);
                    end();
                }
                setNewFrame();
            }
            void onOpenFileChooser(){
                _active = false;
                //TODO: filter types: mov, mp4
                ofFileDialogResult openFileResult= ofSystemLoadDialog("select a video sample"); 
                if (openFileResult.bSuccess){
                    auto path = openFileResult.getPath();
                    _player.load(path);
                }
            }
            void onSpeedChange(float & value){
                _player.setSpeed(value);
            }
            void onVolumeChange(float & value){
                _player.setVolume(value);
            }
            void onStart(){
                _player.load(_path);
                _player.setVolume(_volume);
                _player.play();
            }
            void onStop(){
                _player.stop();
                _player.close();
            }
            ofVideoPlayer _player;

            ofParameter<void> _openFileChooser;
            ofParameter<bool> _loop;
            ofParameter<float> _speed;
            ofParameter<float> _volume;
            std::string _path;

        };
    }; // namespace clip
}; // namespace ofxLiveSet