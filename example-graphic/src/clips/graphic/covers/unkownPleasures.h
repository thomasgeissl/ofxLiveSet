#pragma once
#include "clips/graphic.h"
// TODO: credits
namespace clips {
    class unkownPleasures : public ofxLiveSet::clip::graphic {
    public:
        unkownPleasures(std::string name = "unkownPleasures") : graphic(name){
            _name = name;
            _active.setName(_name);
            
            _primaryColor.set("primaryColor", ofColor::white);
            _numberOfLines.set("numberOfLines", 80, 1, 200);
            _numberOfPoints.set("numberOfPoints", 80, 1, 200);

            _parameters.add(_primaryColor);
            _parameters.add(_numberOfLines);
            _parameters.add(_numberOfPoints);
        }

        void update(){
            begin();
            ofSetColor(_primaryColor);
            ofSetLineWidth(1.2);

            auto x = -_width/2;
            auto y = -_height/2;
            auto offsetX = _width / _numberOfPoints;
            auto offsetY = _height / _numberOfLines;
            for (auto i = 0; i < _numberOfLines; i++) {
                ofPolyline polyline;
                int nModes = ofRandom(1, 4);
                std::vector<float> mus;
                std::vector<float> sigmas;
                for (auto j = 0; j < nModes; j++) {
                    mus.push_back(ofRandom(-50, 50));
                    sigmas.push_back(randNormal(24, 30));
                }
                float w = y;
                for (auto k = 0; k < _numberOfPoints; k++) {
                    x += offsetX;
                    float noise = 0;
                    for (auto l = 0; l < nModes; l++) {
                        noise += normalPDF(x, mus[l], sigmas[l]);
                    }
                    auto yy = 0.3 * w + 0.7 * (y - 600 * noise + noise * ofRandom(0, 1.0) * 200 + ofRandom(0, 1.0));
                    polyline.lineTo(x, yy);
                    w = yy;
                }
                polyline.draw();
                x = -_width/2;
                y += offsetY;
            }

            end();
            setNewFrame();
        }
        float normalPDF(float x, float mu, float sigma) {
            float sigma2 = pow(sigma, 2);
            float numerator = exp(-pow((x - mu), 2) / (2 * sigma2));
            float denominator = sqrt(2 * 3.14 * sigma2);
            return numerator / denominator;
        }
        float randNormal(float mu, float sigma) {
            float sum = 0;
            for (auto i = 0; i < 6; i++) {
                sum += ofRandom(-1, 1);
            }
            return mu + sigma * sum / 6;
        }

        ofParameter<ofColor> _primaryColor;
        ofParameter<int> _numberOfLines;
        ofParameter<int> _numberOfPoints;
    };
};