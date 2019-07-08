#pragma once
#include "clips/graphic.h"

namespace clips {
    class gameOfLifeCell {
        public:
            gameOfLifeCell() : _isAlive(false), _numberOfAliveNeighbours(0) {}
            bool _isAlive;
            int _numberOfAliveNeighbours;

    };
    class gameOfLife : public ofxLiveSet::clip::graphic {
    public:
        typedef std::shared_ptr<gameOfLife> pointer;
        static pointer create(std::string name="gameOfLife")
        {
            return std::make_shared<gameOfLife>(name);
        }
        gameOfLife(std::string name = "gameOfLife") : graphic(name) {
            _name = name;
            _active.setName(_name);
            _xAmount.set("xAmount", 75, 20, 100);
            _primaryColor.set("primaryColor", ofColor::white);
            _backgroundColor.set("backgroundColor", ofColor::darkRed);
            _speed.set("speed", 1, 0, 10);
            _cellLength.set("cellLength", 25, 10, 100);

            _parameters.add(_xAmount);
            _parameters.add(_primaryColor);
            _parameters.add(_backgroundColor);
            _parameters.add(_speed);
            _parameters.add(_cellLength);

        }
        int isCellAlive(int x, int y){
            if(x < 0 || x >= _cells.size()){
                return 0;
            }
            if(y < 0 || y >= _cells[x].size()){
                return 0;
            }
            if(_cells[x][y]._isAlive){
                return 1;
            }
            return 0;
        }
        void update(){
            auto timestamp = ofGetElapsedTimeMillis();
            if(timestamp - _timestamp < 500){
                return;
            }
            _timestamp = timestamp;
            auto newX = ((int)(_width/_cellLength));
            auto newY = ((int)(_height/_cellLength));
            if(newX != _cells.size() || newY != _cells[0].size() ){
                _cells.resize((int)(_width/_cellLength));
                for(auto column = 0; column < _cells.size(); column++){
                    _cells[column].resize((int)(_height/_cellLength));
                }
                int x = ofRandom(20);
                int y = ofRandom(20);

                for(auto i = 0; i < 4; i++){
                    for(auto j = 0; j < 4; j++){
                        _cells[x+i][y+j]._isAlive = true;
                        _cells[x+10+i][y+10+j]._isAlive = true;
                    }
                }

            }
            begin();
                ofTranslate(-_width/2, -_height/2, 0);
                for(auto x = 0; x < _cells.size(); x++){
                    for(auto y = 0; y < _cells[x].size(); y++){
                        _cells[x][y]._numberOfAliveNeighbours += isCellAlive(x-1, y-1);
                        _cells[x][y]._numberOfAliveNeighbours += isCellAlive(x-1, y);
                        _cells[x][y]._numberOfAliveNeighbours += isCellAlive(x-1, y+1);
                        _cells[x][y]._numberOfAliveNeighbours += isCellAlive(x, y-1);
                        _cells[x][y]._numberOfAliveNeighbours += isCellAlive(x, y+1);
                        _cells[x][y]._numberOfAliveNeighbours += isCellAlive(x+1, y-1);
                        _cells[x][y]._numberOfAliveNeighbours += isCellAlive(x+1, y);
                        _cells[x][y]._numberOfAliveNeighbours += isCellAlive(x+1, y+1);
                        if(_cells[x][y]._isAlive){
                            ofSetColor(ofColor::red);
                        }else{
                            ofSetColor(ofColor::gray);
                        }
                        ofDrawRectangle(x*_cellLength + 1, y*_cellLength +1 , _cellLength - 2 , _cellLength - 2);
                    }                  
                }
                for(auto x = 0; x < _cells.size(); x++){
                    for(auto y = 0; y < _cells[x].size(); y++){
                        if(_cells[x][y]._isAlive){
                            ofSetColor(ofColor::red);
                            if(_cells[x][y]._numberOfAliveNeighbours > 3 || _cells[x][y]._numberOfAliveNeighbours < 2){
                                _cells[x][y]._isAlive = false;
                            }
                        }else{
                            ofSetColor(ofColor::gray);
                            if(_cells[x][y]._numberOfAliveNeighbours >= 2 && _cells[x][y]._numberOfAliveNeighbours <= 3){
                                _cells[x][y]._isAlive = true;
                            }
                        }
                        ofDrawRectangle(x*_cellLength + 1, y*_cellLength +1 , _cellLength - 2 , _cellLength - 2);
                    }                  
                }

            end();
            setNewFrame();
        }
        ofParameter<int> _xAmount;
        ofParameter<ofColor> _primaryColor;
        ofParameter<ofColor> _backgroundColor;
        ofParameter<float> _speed;
        ofParameter<float> _cellLength;

        std::vector<std::vector<gameOfLifeCell>> _cells;

        u_int64_t _timestamp;
    };
};