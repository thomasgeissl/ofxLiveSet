#pragma once
#include "./strobe.h"

namespace clips
{
    class generatorStrobe : public clips::strobe, public ofxLiveSet::clip::hasOscInput
    {
    public:
        typedef std::shared_ptr<generatorStrobe> pointer;
        static pointer create(int frequencyChannel, int velocityChannel)
        {
            return std::make_shared<generatorStrobe>(frequencyChannel, velocityChannel);
        }
        generatorStrobe(int frequencyChannel, int velocityChannel) : strobe(frequencyChannel, velocityChannel)
        {
            _name = "generatorStrobe";
            _active.setName("generatorStrobe");

            _config.setName("config");
            _config.add(_treshold.set("treshold", 100, 0, 127));
            _parameters.add(_config);
        }

        void onOscMessage(ofxOscMessage message)
        {
            if (message.getAddress() == "/kls/io/crank")
            {
                auto intensity = message.getArgAsInt(0);
                auto threshold = 100;
                if (intensity < threshold)
                {
                    _frequency = 0;
                }
                else
                {
                    _frequency = ofMap(intensity, threshold, 127, 15, 255);
                }
            }
        }

        ofParameterGroup _config;
        ofParameter<int> _treshold;
    };
};
