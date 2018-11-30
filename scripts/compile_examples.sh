#!/bin/sh
cd "$(dirname "$0")"
cd ..

for example in ./example*/
do
    cd $example
    make
    cd ..
done
