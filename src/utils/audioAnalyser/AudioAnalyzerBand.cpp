
#include "AudioAnalyzerBand.h"

float AudioAnalyzerBand::meter_peak() const { 
    return envFollowerPeak.meter_output(); 
}

float AudioAnalyzerBand::meter_rms() const { 
    return envFollowerRMS.meter_output(); 
}

float AudioAnalyzerBand::meter_onset() const {         
    // the onset threshold will control how much a signal must surpass the average value to be considered a note onset
    if( difference.meter_output() > onsetThreshold){
        return envFollowerPeak.meter_output(); 
    }else{
        return 0.0f;
    }
}

void AudioAnalyzerBand::freqLoChanged(float & pitch) {
    loFreqValue = pdsp::p2f(pitch);
}

void AudioAnalyzerBand::freqHiChanged(float & pitch) {
    hiFreqValue = pdsp::p2f(pitch);    
}
