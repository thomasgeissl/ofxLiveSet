#pragma once
#include "ofxLiveSet.h"
#include "ofxMovingAverage.h"
#include "../../dmx.config.h"

namespace clips
{
    class wind : public ofxLiveSet::clip::dmx, public ofxLiveSet::clip::hasSoundAnalyserInput
    {
    public:
        typedef std::shared_ptr<wind> pointer;
        static pointer create()
        {
            return std::make_shared<wind>();
        }
        wind() : ofxLiveSet::clip::dmx(), ofxLiveSet::clip::hasSoundAnalyserInput(),
                 _leftPeakEnergyMovingAverage(ofxMovingAverage<float>(100)),
                 _rightPeakEnergyMovingAverage(ofxMovingAverage<float>(100))
        {
            _name = "lichtung";
            _active.setName(_name);
            // addParameter(_soundAnalyserId);
            // addParameter(_rightSoundAnalyserId.set("right analyser", 2, 0, 32));
            // addParameter(_static.set("static", false));
            // addParameter(_minValueChimes.set("minValueChimes", 0, 0, 255));
            // addParameter(_maxValueChimes.set("maxValueChimes", 0, 0, 255));
            // addParameter(_threshold.set("threshold", 0.8, 0, 1));
            addParameter(_peakFactor.set("peakFactor", 1.5, 0, 3));
            addParameter(_debounceTime.set("debounceTime", 150, 0, 1000));
            addParameter(_minValue.set("minValue", 0, 0, 255));
            addParameter(_maxValue.set("maxValue", 255, 0, 255));

            _positions.setName("config");
            _positions.add(_innerLeftLight.set("innerLeftLight", 11, 1, KLS_LIGHTBULBSCOUNT));
            _positions.add(_innerRightLight.set("innerRightLight", 13, 1, KLS_LIGHTBULBSCOUNT));

            _positions.add(_middleLeftLight.set("middleLeftLight", 2, 1, KLS_LIGHTBULBSCOUNT));
            _positions.add(_middleRightLight.set("middleRightLight", 20, 1, KLS_LIGHTBULBSCOUNT));

            _positions.add(_pingLeftIndex.set("pingLeftIndex", 4, 1, KLS_LIGHTBULBSCOUNT));
            _positions.add(_pongLeftIndex.set("pongLeftIndex", 8, 1, KLS_LIGHTBULBSCOUNT));
            _positions.add(_pingRightIndex.set("pingRightIndex", 17, 1, KLS_LIGHTBULBSCOUNT));
            _positions.add(_pongRightIndex.set("pongRightIndex", 22, 1, KLS_LIGHTBULBSCOUNT));

            addParameter(_pingPong.set("pingPong", false));
            addParameter(_pingPongTime.set("pingPongTime", 500, 0, 1000));
            _parameters.add(_positions);
            _meters.setName("meters");
            _meters.add(_leftPeakEnergy.set("leftPeakEnergy", 0, 0, 5));
            _meters.add(_rightPeakEnergy.set("rightPeakEnergy", 0, 0, 5));
            _parameters.add(_meters);
        }

        void update()
        {
            auto timestamp = ofGetElapsedTimeMillis();
            for (auto i = 0; i < KLS_LIGHTBULBSCOUNT; i++)
            {
                auto value = ofMap(timestamp - _timestamps[i], 0, 500, _maxValue, _minValue, true);
                _values[i] = value;
                setDmxValue(i + 1, value);
            }
            setDmxValue(_middleLeftLight, ofMap(_leftPeakEnergyMovingAverage.avg() * 4, 0, 2, _minValue, _maxValue));
            setDmxValue(_middleRightLight, ofMap(_rightPeakEnergyMovingAverage.avg() * 4, 0, 2, _minValue, _maxValue));
            if (_leftPeakEnergy > _leftPeakEnergyMovingAverage.avg() * _peakFactor)
            {
                peak(true);
            }
            if (_rightPeakEnergy > _rightPeakEnergyMovingAverage.avg() * _peakFactor)
            {
                peak(false);
            }
            if (_pingPong)
            {
                if (timestamp - _pingPongTimestamp > _pingPongTime)
                {
                    if (_pingPongLeft)
                    {
                        _values[_pingLeftIndex - 1] = _maxValue;
                        _timestamps[_pingLeftIndex - 1] = timestamp;
                        _values[_pongLeftIndex - 1] = _maxValue;
                        _timestamps[_pongLeftIndex - 1] = timestamp;
                    }
                    else
                    {
                        _values[_pingRightIndex - 1] = _maxValue;
                        _timestamps[_pingRightIndex - 1] = timestamp;
                        _values[_pongRightIndex - 1] = _maxValue;
                        _timestamps[_pongRightIndex - 1] = timestamp;
                    }
                    _pingPongTimestamp = timestamp;
                    _pingPongLeft = !_pingPongLeft;
                }
            }
        }
        void stop()
        {
            for (auto i = 1; i <= KLS_LIGHTBULBSCOUNT; i++)
            {
                std::pair<int, int> value(i, 0);
                _valueChangeEvent.notify(value);
            }
            base::stop();
        }

        void setPeakEnergy(int analyserId, float value)
        {
            auto timestamp = ofGetElapsedTimeMillis();
            if (analyserId == _soundAnalyserId)
            {
                _leftPeakEnergy = value;
                _leftPeakEnergyMovingAverage.add(value);
            }
            if (analyserId == _rightSoundAnalyserId)
            {
                _rightPeakEnergy = value;
                _rightPeakEnergyMovingAverage.add(value);
            }
        }
        void peak(bool left)
        {
            auto timestamp = ofGetElapsedTimeMillis();
            auto oldTimestamp = left ? _leftTimestamp : _rightTimestamp;
            if (timestamp - oldTimestamp > _debounceTime)
            {
                if (left)
                {
                    _leftTimestamp = timestamp;
                    _values[_innerLeftLight - 1] = _maxValue;
                    _timestamps[_innerLeftLight - 1] = timestamp;
                }
                else
                {
                    _rightTimestamp = timestamp;
                    _values[_innerRightLight - 1] = _maxValue;
                    _timestamps[_innerRightLight - 1] = timestamp;
                }
            }
        }

        ofParameter<int> _rightSoundAnalyserId;
        ofParameter<int> _minValue;
        ofParameter<int> _maxValue;

        ofParameterGroup _positions;
        ofParameter<int> _innerLeftLight;
        ofParameter<int> _middleLeftLight;
        ofParameter<int> _pingLeftIndex;
        ofParameter<int> _pongLeftIndex;
        ofParameter<int> _innerRightLight;
        ofParameter<int> _middleRightLight;
        ofParameter<int> _pingRightIndex;
        ofParameter<int> _pongRightIndex;
        ofParameter<float> _threshold;
        ofParameter<int> _debounceTime;
        ofParameter<float> _peakFactor;
        ofParameter<bool> _pingPong;
        ofParameter<int> _pingPongTime;

        ofParameterGroup _meters;
        ofParameter<float> _leftPeakEnergy;
        ofParameter<float> _rightPeakEnergy;
        ofxMovingAverage<float> _leftPeakEnergyMovingAverage;
        ofxMovingAverage<float> _rightPeakEnergyMovingAverage;

        u_int64_t _leftTimestamp;
        u_int64_t _rightTimestamp;
        u_int64_t _pingPongTimestamp;
        bool _pingPongLeft;
        int _values[KLS_LIGHTBULBSCOUNT];
        u_int64_t _timestamps[KLS_LIGHTBULBSCOUNT];
    };
};
