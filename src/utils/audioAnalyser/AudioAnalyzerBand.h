
#pragma once

#include "ofxPDSP.h"


class AudioAnalyzerBand : public pdsp::Patchable{

public:
    AudioAnalyzerBand(){
        addModuleInput("left", bandGain);

        // before patching signals to an envelope follower you need to pass it to a detector
        // the choices are: pdsp::AbsoluteValue, pdsp::PositiveValue or pdsp::RMSDetector
        // patching to the blackhole process the modules without outputting sound
        bandGain >> loCut >> hiCut >> rms >> envFollowerRMS;//  >> engine.blackhole();
        hiCut >> absolute >> envFollowerPeak;// >> engine.blackhole();
        // an envelope follower smoothes the signal 
        // with a settable in_attack() and in_release() values
    
        // difference sum the peak signal and the inverted rms signal and output values if the result is positive
        // in this way difference could be a better choice for detecting note onsets
        envFollowerPeak >> difference;// >> engine.blackhole();
        envFollowerRMS * -1.0f >> difference;         
    
        gainControl >> bandFader >> bandGain.in_mod();    

        // setting up controls and analyzerGui ---------------------
        loPitchControl >> p2fLo >> hiCut.in_freq();              
        hiPitchControl >> p2fHi >> loCut.in_freq();              
    
        rmsAttackControl   >> envFollowerRMS.in_attack();
        rmsReleaseControl  >> envFollowerRMS.in_release();
        peakAttackControl  >> envFollowerPeak.in_attack();
        peakReleaseControl >> envFollowerPeak.in_release();
        
        _parameters.setName( "analyser band" );
        _parameters.add( gainControl.set( "band input gain", 0, -48, 24 ));  
        _parameters.add( loPitchControl.set( "band low", 43.0f, 20.0f, 136.0f ));  
        _parameters.add( loFreqValue.set(    "band lo hz", pdsp::p2f(43.0f), 25000.0f, 26000.0f ));  
        _parameters.add( hiPitchControl.set( "band hi", 100.0f, 20.0f, 136.0f ));  
        _parameters.add( hiFreqValue.set(    "band hi hz", pdsp::p2f(100.0f), 25000.0f, 26000.0f ));  
        _parameters.add( rmsAttackControl.set( "rms env attack ms", 50, 20, 250 ));  
        _parameters.add( rmsReleaseControl.set( "rms env release ms", 100, 20, 500 ));  
        _parameters.add( peakAttackControl.set( "peak env attack ms", 5, 1, 25 ));
        _parameters.add( peakReleaseControl.set( "peak env release ms", 10, 1, 50 ));
        _parameters.add( onsetThreshold.set("onset threshold", 0.05f, 0.01f, 1.0f) ); //controls how much a signal must surpass the average value to be considered a note onset

        // you need to add this listeners to update the hertz labels
        loPitchControl.getOFParameterFloat().addListener(this, &AudioAnalyzerBand::freqLoChanged);
        hiPitchControl.getOFParameterFloat().addListener(this, &AudioAnalyzerBand::freqHiChanged);
    }
    void update(){

    }
    ofParameterGroup _parameters;

    float meter_peak() const;
    float meter_rms() const;
    float meter_onset() const;
    
private:
	void freqLoChanged(float & pitch);
	void freqHiChanged(float & pitch);

    pdsp::AbsoluteValue     absolute;
    pdsp::EnvelopeFollower  envFollowerPeak;

    pdsp::RMSDetector       rms;
    pdsp::EnvelopeFollower  envFollowerRMS;

    pdsp::PositiveValue     difference;

    pdsp::HighCut           loCut;
    pdsp::LowCut            hiCut;

    pdsp::Parameter     hiFreqControl;
    pdsp::Parameter     rmsAttackControl;
    pdsp::Parameter     rmsReleaseControl;
    pdsp::Parameter     peakAttackControl;
    pdsp::Parameter     peakReleaseControl;

    pdsp::Amp        bandGain;
    pdsp::DBtoLin    bandFader;
    pdsp::Parameter  gainControl;            
    
    pdsp::PitchToFreq       p2fLo;
    pdsp::Parameter         loPitchControl;
    ofParameter<float>      loFreqValue;

    pdsp::PitchToFreq       p2fHi;
    pdsp::Parameter         hiPitchControl;
    ofParameter<float>      hiFreqValue;

    ofParameter<float>      onsetThreshold;   
    ofParameter<int> _analyserId;   
};

