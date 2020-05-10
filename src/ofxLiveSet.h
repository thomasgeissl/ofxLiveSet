#pragma once
#include "./config.h"
#include "./project.h"
#include "./session.h"
#include "./tracks/base.h"
#include "./tracks/midi.h"
#include "./clips/base.h"
// #include "./clips/midi.h"
#include "./clips/null.h"
#include "./clips/soundReactive.h"
#include "./clips/midiReactive.h"

#if OFXLIVESET_USE_GRAPHICS
#include "./tracks/graphic.h"
#include "./clips/graphic.h"
#include "./clips/graphic/video/videoGrabber.h"
#include "./clips/graphic/video/videoPlayer.h"
#include "./clips/graphic/slidePlayer.h"
#endif

#if OFXLIVESET_USE_AUDIO
#include "./tracks/audio.h"
#include "./clips/audio.h"
#include "./clips/audio/audioPlayer.h"
#endif

#if OFXLIVESET_USE_DMX
#include "./tracks/dmx.h"
#endif

#if OFXLIVESET_USE_LUA
#include "./clips/lua.h"
#endif
