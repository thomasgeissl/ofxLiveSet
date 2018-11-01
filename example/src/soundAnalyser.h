#pragma once
#include "ofMain.h"
#include "ofxOsc.h"

class soundAnalyser {
public:
    soundAnalyser(int port = 8000){
        _oscReceiver.setup(port);
    }
    void setup(){}
    void update(){
        while(_oscReceiver.hasWaitingMessages()) {
            ofxOscMessage m;
            _oscReceiver.getNextMessage(m);
            
//        TODO: regex capture id
            if(m.getAddress() == "/1/peakEnergy") {
                auto peakEnergy = m.getArgAsFloat(0);
                auto id = 1;
                std::pair<int, float> value(id, peakEnergy);
                _peakEnergyEvent.notify(value);
            }
            
            if(m.getAddress() == "/1/pitch") {
                auto pitch = m.getArgAsFloat(0);
                auto id = 1;
                std::pair<int, float> value(id, pitch);
                _pitchEvent.notify(value);
            }
            
            if(m.getAddress() == "/1/rms") {
                auto rms = m.getArgAsFloat(0);
                auto id = 1;
                std::pair<int, float> value(id, rms);
                _rootMeanSquareEvent.notify(value);
            }
            
            if(m.getAddress() == "/1/fft") {
                for(auto i = 0; i < m.getNumArgs(); i++){
                    std::vector<float> fft;
                    fft.push_back(m.getArgAsFloat(i));
                    auto id = 1;
                    std::pair<int, std::vector<float>> value(id, fft);
                    _fftMagnitudeSpectrumEvent.notify(value);
                }
            }
            
            if(m.getAddress() == "/1/mel") {
                for(auto i = 0; i < m.getNumArgs(); i++){
                    std::vector<float> mel;
                    mel.push_back(m.getArgAsFloat(i));
                    auto id = 1;
                    std::pair<int, std::vector<float>> value(id, mel);
                    _melFrequencySpectrumEvent.notify(value);
                }
            }
        }
    }
    
    
    ofEvent<std::pair<int,float>> _peakEnergyEvent; //(id, value)
    ofEvent<std::pair<int,float>> _pitchEvent; //(id, value)
    ofEvent<std::pair<int,std::vector<float>>> _fftMagnitudeSpectrumEvent; //(id, value)
    ofEvent<std::pair<int,std::vector<float>>> _melFrequencySpectrumEvent; //(id, value)
    ofEvent<std::pair<int,float>> _rootMeanSquareEvent; //(id, value)
    ofEvent<std::pair<int,float>> _speactralCentroidEvent; //(id, value)
    ofEvent<std::pair<int,float>> _speactralCrestEvent; //(id, value)
    ofEvent<std::pair<int,float>> _speactralDifferenceEvent; //(id, value)
    ofEvent<std::pair<int,float>> _speactralFlatnessEvent; //(id, value)
    ofEvent<std::pair<int,float>> _zeroCrossingRateEvent; //(id, value)

    ofxOscReceiver _oscReceiver;
};
