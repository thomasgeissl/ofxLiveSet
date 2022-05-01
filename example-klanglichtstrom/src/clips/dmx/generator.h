#pragma once
#include "ofxLiveSet.h"
#include "../../dmx.config.h"

namespace clips
{
    class generator : public ofxLiveSet::clip::dmx, public ofxLiveSet::clip::hasSoundAnalyserInput, public ofxLiveSet::clip::hasOscInput
    {
    public:
        typedef std::shared_ptr<generator> pointer;
        static pointer create()
        {
            return std::make_shared<generator>();
        }
        generator() : ofxLiveSet::clip::dmx(), ofxLiveSet::clip::hasSoundAnalyserInput(), _lastIndex(0)
        {
            _name = "generator";

            _active.setName(_name);

            // addParameter(_soundAnalyserId);
            addParameter(_intensity.set("intensity", 0, 0, 127));
            addParameter(_threshold.set("threshold", .1, 0, 1));
            addParameter(_peakEnergyDebounceTime.set("debounce", 300, 30, 500));
            addParameter(_minValue.set("minValue", 0, 0, 255));
            addParameter(_maxValue.set("maxValue", 100, 0, 255));

            _config.setName("config");
            _config.add(_minDistance.set("minDistance", 12, 1, KLS_LIGHTBULBSCOUNT));
            _parameters.add(_config);

            _meters.setName("meters");
            _meters.add(_peakEnergy.set("peakEnergy", 0, 0, 5));
            _parameters.add(_meters);

            _timestamp = ofGetElapsedTimeMillis();

            _values.resize(KLS_LIGHTBULBSCOUNT);
            _timestamps.resize(KLS_LIGHTBULBSCOUNT);
        }

        void update()
        {
            auto timestamp = ofGetElapsedTimeMillis();

            for (auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++)
            {
                if (timestamp - _timestamps[i] < 1000)
                {
                    _values[i] = ofMap(timestamp, _timestamps[i], _timestamps[i] + 1000, _maxValue, _minValue);
                    setDmxValue(i + 1, _values[i]);
                }
            }
        }
        void stop()
        {
            for (auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++)
            {
                setDmxValue(i + 1, 0);
            }
            base::stop();
        }
        void setPeakEnergy(int analyserId, float value)
        {
            if (analyserId != _soundAnalyserId)
            {
                return;
            }
            _peakEnergy = value;
            if (value < _threshold)
            {
                return;
            }
            auto timestamp = ofGetElapsedTimeMillis();
            if (timestamp - _timestamp < _peakEnergyDebounceTime)
            {
                return;
            }
            _timestamp = timestamp;

            auto i = ofRandom(KLS_LIGHTBULBSCOUNT);
            while (abs(_lastIndex - i) < _minDistance)
            {
                i = ofRandom(KLS_LIGHTBULBSCOUNT);
            }
            _lastIndex = i;
            _timestamps[i] = ofGetElapsedTimeMillis();
            _values[i] = _maxValue * value;
        }

        void onOscMessage(ofxOscMessage message)
        {
            if (message.getAddress() == "/kls/io/crank")
            {
                ofLogNotice("clip::generator") << message.getAddress();
                // if(message.isIntArg(0){
                    _intensity = message.getArgAsInt(0);
                // })
            }else if(message.getAddress() == "/kls/io/midi2osc"){
                ofLogNotice() << message.getAddress();
                if(message.getArgAsString(0) == "noteon"){
                    auto note = message.getArgAsInt(1);
                    ofLogNotice() << "set section "<< note;
                }
            }
        }

        ofParameter<int> _value;
        std::vector<int> _values;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;

        ofParameter<float> _threshold;
        ofParameter<int> _peakEnergyDebounceTime;

        ofParameterGroup _meters;
        ofParameter<float> _peakEnergy;
        ofParameter<float> _intensity;

        ofParameterGroup _config;
        ofParameter<int> _minDistance;

        u_int64_t _timestamp;
        std::vector<u_int64_t> _timestamps;
        int _lastIndex;
    };
};
