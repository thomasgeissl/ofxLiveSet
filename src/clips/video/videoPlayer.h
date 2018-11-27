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
            videoPlayer(std::string path, std::string name = "") : graphic(name){
                _openFileChooser.set("openFileChooser");
                _openFileChooser.addListener(this, &videoPlayer::onOpenFileChooser);
                _loop.set("loop", true);
                _parameters.add(_openFileChooser);
                _parameters.add(_loop);
                _player.load(path);
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
                    beginFboWithShaderIfActive();
                    ofClear(255,0);
                    ofTranslate(ofMap(_x, 0, 1, 0, _width), ofMap(_y, 0, 1, 0, _height));
                    _player.draw(-_player.getWidth()/2,-_player.getHeight()/2);
                    endFboWithShaderIfActive();
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
            void onStart(){
                _player.play();
            }
            void onStop(){
                _player.stop();
            }
            ofVideoPlayer _player;

            ofParameter<void> _openFileChooser;
            ofParameter<bool> _loop;
        };
    }; // namespace clip
}; // namespace ofxLiveSet