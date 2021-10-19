#pragma once
#include "ofMain.h"
#include "base.h"
#include "ofxPDSP.h"
#include "./hasMidiInput.h"
#include "./hasOscInput.h"
#include "../clips/audio.h"
#include "../utils/audioAnalyser/AudioAnalyzerBand.h"

namespace ofxLiveSet
{
	namespace track
	{
		class audio : public base, public pdsp::Patchable, public hasMidiInput, public hasOscInput
		{
		public:
			typedef std::shared_ptr<audio> pointer;
			static pointer create(std::string name)
			{
				return std::make_shared<audio>(name);
			}
			audio(std::string name = "") : base("OFXLIVESET_TRACK_AUDIO", name)
			{
				_ioParameters.add(_midiInputParameters);
				_leftInChannel.set("leftIn", 0, -1, 16);
				_rightInChannel.set("rightIn", -1, -1, 16);
				_ioParameters.add(_leftInChannel);
				_ioParameters.add(_rightInChannel);

				addModuleInput("left", _leftAmp);
				addModuleInput("right", _rightAmp);
				// TODO: audio effects
				addModuleOutput("left", _leftAmp);
				addModuleOutput("right", _rightAmp);

				_leftAmp.set(1);
				_rightAmp.set(1);

				_gain.addListener(this, &audio::onGainChange);
				_mute.addListener(this, &audio::onMuteChange);
				_leftInChannel.addListener(this, &audio::onInputChange);
				_rightInChannel.addListener(this, &audio::onInputChange);

				osc >> _leftAmp;
				osc >> _rightAmp;
			}
			void onGainChange(float &value)
			{
				_leftAmp.set(value);
				_rightAmp.set(value);
			}
			void onMuteChange(bool &value)
			{
				if (value)
				{
					_leftAmp.set(0);
					_rightAmp.set(0);
				}
				else
				{
					_leftAmp.set(_gain);
					_rightAmp.set(_gain);
				}
			}
			void onInputChange(int &value)
			{
				auto inputs = std::pair<int, int>(_leftInChannel, _rightInChannel);
				_inputsChanged.notify(this, inputs);
			}

			clip::base::pointer addClip(clip::base::pointer clip)
			{
				auto audioClip = dynamic_pointer_cast<ofxLiveSet::clip::audio>(clip);
				if (audioClip != nullptr)
				{
					audioClip->out("left") >> _leftAmp;
					audioClip->out("right") >> _rightAmp;
				}
				return base::addClip(clip);
			}

			void newMidiMessage(ofxMidiMessage & message){
				
			}
			pdsp::Amp _leftAmp;
			pdsp::Amp _rightAmp;

			pdsp::VAOscillator osc;

			ofParameter<int> _leftInChannel;
			ofParameter<int> _rightInChannel;
			ofEvent<std::pair<int, int>> _inputsChanged;
		};
	}; // namespace track
};	   // namespace ofxLiveSet
