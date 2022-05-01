#if OFXLIVESET_USE_SOUNDANALYSER
void ofxLiveSet::session::onPeakEnergy(ofxSoundAnalyser::FloatArg &arg)
{
    for (auto &track : _tracks)
    {
        auto clip = dynamic_pointer_cast<ofxLiveSet::clip::hasSoundAnalyserInput>(track->getClip());
        if (clip != nullptr)
        {
            clip->setPeakEnergy(arg.getId(), arg.getValue());
        }
    }
}
void ofxLiveSet::session::onPitch(ofxSoundAnalyser::FloatArg &arg)
{
    for (auto &track : _tracks)
    {
        auto clip = dynamic_pointer_cast<ofxLiveSet::clip::hasSoundAnalyserInput>(track->getClip());
        if (clip != nullptr)
        {
            clip->setPitch(arg.getId(), arg.getValue());
        }
    }
}
void ofxLiveSet::session::onRootMeanSquare(ofxSoundAnalyser::FloatArg &arg)
{
    for (auto &track : _tracks)
    {
        auto clip = dynamic_pointer_cast<ofxLiveSet::clip::hasSoundAnalyserInput>(track->getClip());
        if (clip != nullptr)
        {
            clip->setRootMeanSquare(arg.getId(), arg.getValue());
        }
    }
}
void ofxLiveSet::session::onFftMagnitudeSpectrum(ofxSoundAnalyser::FloatVecArg &arg)
{
    for (auto &track : _tracks)
    {
        auto clip = dynamic_pointer_cast<ofxLiveSet::clip::hasSoundAnalyserInput>(track->getClip());
        if (clip != nullptr)
        {
            clip->setFftMagnitudeSpectrum(arg.getId(), arg.getValue());
        }
    }
}
void ofxLiveSet::session::onMelFrequencySpectrum(ofxSoundAnalyser::FloatVecArg &arg)
{
    for (auto &track : _tracks)
    {
        auto clip = dynamic_pointer_cast<ofxLiveSet::clip::hasSoundAnalyserInput>(track->getClip());
        if (clip != nullptr)
        {
            clip->setMelFrequencySpectrum(arg.getId(), arg.getValue());
        }
    }
}
void ofxLiveSet::session::onSpectralCentroid(ofxSoundAnalyser::FloatArg &arg)
{
    for (auto &track : _tracks)
    {
        auto clip = dynamic_pointer_cast<ofxLiveSet::clip::hasSoundAnalyserInput>(track->getClip());
        if (clip != nullptr)
        {
            clip->setSpectralCentroid(arg.getId(), arg.getValue());
        }
    }
}
void ofxLiveSet::session::onSpectralCrest(ofxSoundAnalyser::FloatArg &arg)
{
    for (auto &track : _tracks)
    {
        auto clip = dynamic_pointer_cast<ofxLiveSet::clip::hasSoundAnalyserInput>(track->getClip());
        if (clip != nullptr)
        {
            clip->setSpectralCrest(arg.getId(), arg.getValue());
        }
    }
}
void ofxLiveSet::session::onSpectralDifference(ofxSoundAnalyser::FloatArg &arg)
{
    for (auto &track : _tracks)
    {
        auto clip = dynamic_pointer_cast<ofxLiveSet::clip::hasSoundAnalyserInput>(track->getClip());
        if (clip != nullptr)
        {
            clip->setSpectralDifference(arg.getId(), arg.getValue());
        }
    }
}
void ofxLiveSet::session::onSpectralFlatness(ofxSoundAnalyser::FloatArg &arg)
{
    for (auto &track : _tracks)
    {
        auto clip = dynamic_pointer_cast<ofxLiveSet::clip::hasSoundAnalyserInput>(track->getClip());
        if (clip != nullptr)
        {
            clip->setSpectralFlatness(arg.getId(), arg.getValue());
        }
    }
}
void ofxLiveSet::session::onZeroCrossingRate(ofxSoundAnalyser::FloatArg &arg)
{
    for (auto &track : _tracks)
    {
        auto clip = dynamic_pointer_cast<ofxLiveSet::clip::hasSoundAnalyserInput>(track->getClip());
        if (clip != nullptr)
        {
            clip->setZeroCrossingRate(arg.getId(), arg.getValue());
        }
    }
}
void ofxLiveSet::session::onChord(ofxSoundAnalyser::FloatArg &arg)
{
    for (auto &track : _tracks)
    {
        auto clip = dynamic_pointer_cast<ofxLiveSet::clip::hasSoundAnalyserInput>(track->getClip());
        if (clip != nullptr)
        {
            clip->setChord(arg.getId(), arg.getValue());
        }
    }
}
void ofxLiveSet::session::onChromogram(ofxSoundAnalyser::FloatVecArg &arg)
{
    for (auto &track : _tracks)
    {
        auto clip = dynamic_pointer_cast<ofxLiveSet::clip::hasSoundAnalyserInput>(track->getClip());
        if (clip != nullptr)
        {
            clip->setChromogram(arg.getId(), arg.getValue());
        }
    }
}
#endif
