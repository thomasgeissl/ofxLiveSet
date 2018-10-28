#pragma once
#include "ofMain.h"
#include "ofxOsc.h"

class soundAnalyser {
public:
    soundAnalyser(){
        _oscReceiver.setup(8000);
    }
    void setup(){}
    void update(){
        while(_oscReceiver.hasWaitingMessages()) {
            ofxOscMessage m;
            _oscReceiver.getNextMessage(m);
//            ofLogNotice() << m.getAddress() << " " << m.;
            
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
        }
    }
    
    
    ofEvent<std::pair<int,float>> _peakEnergyEvent; //(id, value)
    ofEvent<std::pair<int,float>> _pitchEvent; //(id, value)
    ofEvent<std::pair<int,float>> _fftMagnitudeSpectrumEvent; //(id, value), TODO: vector
    ofEvent<std::pair<int,float>> _melFrequencySpectrumEvent; //(id, value), TODO: vector
    ofEvent<std::pair<int,float>> _rootMeanSquareEvent; //(id, value)
    ofEvent<std::pair<int,float>> _speactralCentroidEvent; //(id, value)
    ofEvent<std::pair<int,float>> _speactralCrestEvent; //(id, value)
    ofEvent<std::pair<int,float>> _speactralDifferenceEvent; //(id, value)
    ofEvent<std::pair<int,float>> _speactralFlatnessEvent; //(id, value)
    ofEvent<std::pair<int,float>> _zeroCrossingRateEvent; //(id, value)

    ofxOscReceiver _oscReceiver;
};
