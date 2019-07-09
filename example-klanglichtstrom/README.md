# example-klanglichtstrom
## description
audio reactive dmx live set for klinglichtstrom performance. 

### controllable dmx devices
* 1 - 16: light bulb
<!-- * 17: light bulb for chime -->
<!-- * 18,19,20: none -->
* 17, 18: strobe light
<!-- * 21, 22: strobe light -->

## installation
* install dependencies: `./scripts/install_dependencies.sh`
* if your ide/make system cannot handle ofxMidi as a local_addon then move it to the global addons, adapt `addons.make` and rerun the project generator