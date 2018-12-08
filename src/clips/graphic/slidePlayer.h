#pragma once
#include "ofMain.h"
#include "./graphic.h"

namespace ofxLiveSet
{
    namespace clip
    {
        class slidePlayer : public graphic
        {
        public:
            slidePlayer(std::string path, std::string name = "slidePlayer") : graphic(name){
                _name = name;
                _active.setName(_name);
                _openFileChooser.set("openFileChooser");
                _openFileChooser.addListener(this, &slidePlayer::onOpenFileChooser);

                _previous.set("previous");
                _previous.addListener(this, &slidePlayer::onPrevious);
                _next.set("next");
                _next.addListener(this, &slidePlayer::onNext);

                _currentIndex.set("currentIndex", 0, 0, 0);
                _currentIndex.addListener(this, &slidePlayer::onCurrentIndexChange);

                _parameters.add(_openFileChooser);
                _parameters.add(_previous);
                _parameters.add(_next);
                _parameters.add(_currentIndex);

                
                setup(path);
            }
            void update(){}
            void setup(std::string path){
                //TODO
                // check if dir
                // get number of slides inside the dir
                // iterate through directory and load image
                auto size = 1;
                _slides.resize(size);
                _currentIndex.setMax(size-1);
                _currentIndex = 0;
                auto index = 0;
                onCurrentIndexChange(index);
            }
            void onOpenFileChooser(){
                _active = false;
                ofFileDialogResult openFileResult= ofSystemLoadDialog("select a video sample"); 
                if (openFileResult.bSuccess){
                    auto path = openFileResult.getPath();
                    setup(path);
                }
            }

            void onPrevious(){
                _currentIndex = std::max(0, _currentIndex-1);
            }
            void onNext(){
                _currentIndex = std::min((int)(_slides.size()) - 1, _currentIndex+1);
            }
            void onCurrentIndexChange(int & value){
                begin();
                // TODO: draw image
                end();
                setNewFrame();
            }
            std::vector<ofImage> _slides;
            ofParameter<void> _openFileChooser;
            ofParameter<int> _currentIndex;
            ofParameter<void> _previous;
            ofParameter<void> _next;
        };
    }; // namespace clip
}; // namespace ofxLiveSet