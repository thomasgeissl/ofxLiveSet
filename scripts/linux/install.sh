#!/bin/sh

curl -O https://openframeworks.cc/versions/v0.10.1/of_v0.10.1_linux64gcc6_release.tar.gz
tar -zxvf of_v0.10.1_linux64gcc6_release.tar.gz
rm of_v0.10.1_linux64gcc6_release.tar.gz
cd of_v0.10.1_linux64gcc6_release/addons
git clone https://github.com/thomasgeissl/ofxLiveSet.git
cd ofxLiveSet/scripts
./install_examples-dependencies.sh
./compile_examples.sh