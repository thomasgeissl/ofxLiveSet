#!/bin/sh

git clone https://github.com/npisanti/ofxDmx.git
cd ofxDmx
git checkout f14699718284ad89efaf703b3694ca79e7a254b3
cd ..

git clone https://github.com/danomatika/ofxMidi.git
cd ofxMidi
git checkout c80fba1ca451cd06a459c0835cc32fb6f51610ed
cd ..
cp -R ofxMidi ../../..

git clone https://github.com/thomasgeissl/ofxMidiMapper.git
cd ofxMidiMapper
git checkout 1d8b857d2d459b1559962b3b0d9a19a1b34630cc
cd ..

git clone https://github.com/thomasgeissl/ofxKeyMapper.git
cd ofxKeyMapper
git checkout 409af696a6d74ed78c435a6d767754cbfc7caddd
cd .. 

git clone https://github.com/thomasgeissl/ofxOscMapper.git
cd ofxOscMapper
git checkout 7b90ac5f49bba1bca6348d8adb22c9133fc81283
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

git clone https://github.com/npisanti/ofxPDSP.git
cd ofxPDSP
git checkout 7704691099b43b4af4bc8b9bd8140a91e6f6da1e
cd ..

git clone https://github.com/npisanti/ofxAudioFile.git
cd ofxAudioFile
git checkout 53636bbe5964a83d195bfce8eba3ecfc5180a1b8
cd ..

git clone https://github.com/npisanti/ofxSIMDFloats.git
cd ofxSIMDFloats
git checkout d861d55c33305340c623ff10aacdc9b87b3dd73e
cd ..
