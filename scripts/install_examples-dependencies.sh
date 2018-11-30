#!/bin/sh
cd "$(dirname "$0")"
cd ..

for script in ./example*/scripts/install_dependencies.sh
do
    $script
done