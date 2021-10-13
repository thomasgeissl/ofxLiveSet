#pragma once
#include "clips/osc.h"

namespace clips {
    class organDrone : public ofxLiveSet::clip::osc {
    public:
        typedef std::shared_ptr<organDrone> pointer;
        static pointer create()
        {
            return std::make_shared<organDrone>();
        }
        organDrone() : osc()
        {
            _name = "drone";
            _active.setName("drone");
            addParameter(_note.set("note", 60, 0, 127));
            addParameter(_velocity.set("velocity", 127, 0, 127));
            addParameter(_channel.set("channel", 1, 1, 16));
        }
        
        void update(){
		ofxOscMessage message;
		// /ofOSC2MIDI 1 144 60 127
		message.setAddress("/ofOSC2MIDI");
		message.addIntArg(1);
		message.addIntArg(144);
		message.addIntArg(60);
		message.addIntArg(127);
		sendOscMessage(message);
	}
        void onActiveChange(bool & value){
            if(value){
            }else{
            }
        }
	ofParameter<int> _note;
	ofParameter<int> _velocity;
	ofParameter<int> _channel;
    };
};
