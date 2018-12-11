#!/bin/sh
# TODO: automatically get os
echo "What OS are you using? (0): OSX, (1): LINUX, (any other): WINDOWS"
read OS
if [ $OS == 0 ]
    then source ./osx/install.sh
    elif [ $OS == 1 ]
        then source ./linux/install.sh
    else source ./win/install.sh
fi

exit



