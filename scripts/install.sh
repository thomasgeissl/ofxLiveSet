#!/bin/sh
# TODO: automatically get os
echo "What OS are you using? (0): OSX, (1): LINUX, (any other): WINDOWS"
read OS
if [ $OS == 0 ] 
then 
    curl -O https://openframeworks.cc/versions/v0.10.1/of_v0.10.1_osx_release.zip
    unzip -a of_v0.10.1_osx_release.zip
    rm of_v0.10.1_osx_release.zip
    cd of_v0.10.1_osx_release/addons
elif [ $OS == 1 ] 
then
        curl -O https://openframeworks.cc/versions/v0.10.1/of_v0.10.1_linux64gcc6_release.tar.gz
        tar -zxvf of_v0.10.1_linux64gcc6_release.tar.gz
        rm of_v0.10.1_linux64gcc6_release.tar.gz
        of_v0.10.1_linux64gcc6_release/scripts/linux/ubuntu/install_dependencies.sh
        cd of_v0.10.1_linux64gcc6_release/addons
else echo "windows support is not yet implemented" && exit
fi

git clone https://github.com/thomasgeissl/ofxLiveSet.git
cd ofxLiveSet/scripts
./install_examples-dependencies.sh
./compile_examples.sh