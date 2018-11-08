#!/bin/sh
cd "$(dirname "$0")"
cd ..

mkdir local_addons
cd local_addons

git clone https://github.com/npisanti/ofxDmx.git
cd ofxDmx
git checkout f14699718284ad89efaf703b3694ca79e7a254b3
cd ..

git clone https://github.com/danomatika/ofxMidi.git
cd ofxMidi
git checkout c80fba1ca451cd06a459c0835cc32fb6f51610ed
cd ..

git clone https://github.com/thomasgeissl/ofxMidiMapper.git
cd ofxMidiMapper
git checkout 87e0591da8a2a7876065c33db1aa120502b41063
cd ..

git clone https://github.com/thomasgeissl/ofxKeyMapper.git
cd ofxKeyMapper
git checkout 307c4174769745730fa20c8c4cd8f0570c2558fb
cd .. 

git clone https://github.com/256dpi/ofxMQTT.git
cd ofxMQTT
git checkout eb916eddadf5e867d78cabbc6ca8d4e8ba3b38d4
cd ..

git clone https://github.com/thomasgeissl/ofxLiveSet.git
cd ofxLiveSet
git checkout f605434a451ca959805695b3786c5bab5c0b1478
cd ..

git clone https://github.com/thomasgeissl/ofxSoundAnalyser.git
cd ofxSoundAnalyser
git checkout 9708ff0759feebfa59935fda1b67f1a4184806d9
cd ..
