#pragma once
#include "clips/osc.h"

namespace clips
{
    class organDrone : public ofxLiveSet::clip::osc
    {
    public:
        typedef std::shared_ptr<organDrone> pointer;
        static pointer create()
        {
            return std::make_shared<organDrone>();
        }
        organDrone() : osc()
        {
            setName("drone");
            addParameter(_interval.set("interval", 520, 50, 5000));
            addParameter(_trigger.set("trigger"));
            addParameter(_duration.set("duration", 50, 0, 5000));
            addParameter(_note.set("note", 60, 0, 127));
            addParameter(_velocity.set("velocity", 127, 0, 127));
            addParameter(_channel.set("channel", 1, 1, 16));
            addParameter(_blackout.set("blackout"));

            _active.addListener(this, &organDrone::onActiveChange);
            _trigger.addListener(this, &organDrone::onTrigger);
            _blackout.addListener(this, &organDrone::onBlackout);
        }

        void update()
        {
            auto timestamp = ofGetElapsedTimeMillis();
            for (auto note = 0; note <= 127; note++)
            {
                if (_timestamps[note] > 0 && _timestamps[note] + _duration > timestamp)
                {
                    _timestamps[note] = -1;
                    ofxOscMessage message;
                    // /ofOSC2MIDI 1 144 60 127
                    message.setAddress("/ofOSC2MIDI");
                    message.addIntArg(_channel);
                    message.addIntArg(MidiStatus::MIDI_NOTE_OFF);
                    message.addIntArg(note);
                    message.addIntArg(0);
                    sendOscMessage(message);
                }
            }
        }
        void onActiveChange(bool &value)
        {
            if (value)
            {
            }
            else
            {
                onBlackout();
            }
        }
        void onTrigger()
        {
            if (!_active)
                return;
            ofxOscMessage message;
            // /ofOSC2MIDI 1 144 60 127
            message.setAddress("/ofOSC2MIDI");
            message.addIntArg(_channel);
            message.addIntArg(MidiStatus::MIDI_NOTE_ON);
            message.addIntArg(_note);
            message.addIntArg(_velocity);
            _timestamps[_note] = ofGetElapsedTimeMillis();
            sendOscMessage(message);
        }
        void onBlackout()
        {
            for (auto note = 0; note < 127; note++)
            {
                ofxOscMessage message;
                // /ofOSC2MIDI 1 144 60 127
                message.setAddress("/ofOSC2MIDI");
                message.addIntArg(_channel);
                message.addIntArg(MidiStatus::MIDI_NOTE_OFF);
                message.addIntArg(note);
                message.addIntArg(0);
                sendOscMessage(message);
            }
        }
        ofParameter<void> _trigger;
        ofParameter<int> _note;
        ofParameter<int> _velocity;
        ofParameter<int> _channel;
        ofParameter<int> _duration;
        ofParameter<int> _interval;
        ofParameter<void> _blackout;

        u_int64_t _timestamps[127];
    };
};
