#!/bin/sh
cd "$(dirname "$0")"
cd ..

mkdir local_addons
cd local_addons

source "../../scripts/install_dependencies.sh"

git clone https://github.com/darrenmothersele/ofxBeat.git
cd ofxBeat
git checkout 71600c8e2620ce3fb0a6afac6dcb22a540d9c7b1
cd ..