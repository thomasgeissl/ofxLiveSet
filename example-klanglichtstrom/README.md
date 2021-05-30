# example-klanglichtstrom
## description
audio reactive dmx live set for klinglichtstrom performance. 

### controllable dmx devices
- light bulbs
    - dimmer 0: ch 1
    - dimmer 1: ch 7
    - dimmer 2: ch 13
    - dimmer 3: ch 19
- light bulb (chimes)
    - dimmer 4: ch 25
- strobe light: ch 30
- spot light: ch 40

## installation
* install dependencies: `./scripts/install_dependencies.sh`
* if your ide/make system cannot handle ofxMidi as a local_addon then move it to the global addons, adapt `addons.make` and rerun the project generator