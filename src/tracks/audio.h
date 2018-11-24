#pragma once
#include "ofMain.h"
#include "base.h"
#include "ofxPDSP.h"
#include "../clips/audio.h"

namespace ofxLiveSet
{
namespace track
{
class audio : public base, public pdsp::Patchable
{
public:
	audio(std::string name = "") : base(name){
		_leftInChannel.set("leftIn", 0, -1, 16);
		_rightInChannel.set("rightIn", -1, -1, 16);
		_outputParameters.add(_leftInChannel);
		_outputParameters.add(_rightInChannel);

		addModuleInput("left", _leftAmp);
		addModuleInput("right", _rightAmp);
		// TODO: audio effects
        addModuleOutput("left", _leftAmp );
		addModuleOutput("right", _rightAmp );

		_leftAmp.set(1);
		_rightAmp.set(1);

		_gain.addListener(this, &audio::onGainChange);
		_leftInChannel.addListener(this, &audio::onInputChange);
		_rightInChannel.addListener(this, &audio::onInputChange);
	}
	void onGainChange(float & value){
		_leftAmp.set(value);
		_rightAmp.set(value);
	}
	void onInputChange(int & value){
		auto inputs = std::pair<int, int>(_leftInChannel, _rightInChannel);
		_inputsChanged.notify(this, inputs);
	}

	clip::base* addClip(clip::base *clip){
		auto audioClip = dynamic_cast<ofxLiveSet::clip::audio *>(clip);
        if (audioClip != nullptr) {
			audioClip->out("left") >> _leftAmp;
			audioClip->out("right") >> _rightAmp;
		}
		return base::addClip(clip);
	}
	pdsp::Amp _leftAmp;
	pdsp::Amp _rightAmp;

	ofParameter<int> _leftInChannel;
	ofParameter<int> _rightInChannel;
	ofEvent<std::pair<int,int>> _inputsChanged;
};
}; // namespace track
}; // namespace ofxLiveSet
