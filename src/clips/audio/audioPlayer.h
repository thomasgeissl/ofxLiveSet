#pragma once
#include "ofMain.h"
#include "../audio.h"

namespace ofxLiveSet
{
    namespace clip
    {
        class audioPlayer : public audio
        {
        public:
            audioPlayer(std::string name = "") : audio(name){
                _active.addListener(this, &audioPlayer::onActiveChange);
                _openFileChooser.set("openFileChooser");
                _openFileChooser.addListener(this, &audioPlayer::onOpenFileChooser);
                _parameters.add(_openFileChooser);
                _parameters.add( faderControl.set("volume", 0, -48, 24) );
                _parameters.add( sampleName.set("sample", "no sample"));
                samplePath.set("path", "no path");
                _parameters.add( pitchControl.set("pitch", 0, -24, 24));
                _parameters.add( smoothControl.set("fade ms", 0, 0, 50)); 
                
                samplePath.addListener(this, &audioPlayer::sampleChangedCall );
      
                patch();
                ofSetLogLevel(OF_LOG_VERBOSE);
            }
            void stop(){
                envGate.off();
            }

            void update(){
                // TODO: check playhead and restart if loop
            }
            void onActiveChange(bool & value){
                if(value){
                    if( sample.channels == 1 ){
                        sampler1.setSample( &sample, 0, 0 );
                    }else{
                        sampler1.setSample( &sample, 0, 1 );
                    }
                    envGate.trigger(1.0f);
                    sampleTrig.trigger(1.0f);
                }else{
                    stop();          
                }
            }
            void onOpenFileChooser(){
                _active = false;
                float fvalue = faderControl.get();
                faderControl.set(0.0f);
                    
                ofFileDialogResult openFileResult= ofSystemLoadDialog("select an audio sample"); 
                    
                    //Check if the user opened a file
                    if (openFileResult.bSuccess){
                        
                        string path = openFileResult.getPath();
                        
                        samplePath = path;
                    
                        ofLogVerbose("file loaded");
                        
                    }else {
                        ofLogVerbose("User hit cancel");
                    }

                    // switch to mono if the sample has just one channel
                    if( sample.channels == 1 ){
                        sampler1.setSample( &sample, 0, 0 );
                    }else{
                        sampler1.setSample( &sample, 0, 1 );
                    }
                                        
                    faderControl.set(fvalue);
                    _active = false;
            }


            void patch (){
                addModuleOutput("left", fader0 );
                addModuleOutput("right", fader1 );
                
                pitchControl >> sampler0.in_pitch();
                pitchControl >> sampler1.in_pitch();
                
                sampleTrig >> sampler0 >> amp0;
                envGate    >> env      >> amp0.in_mod();
                sampleTrig >> sampler1 >> amp1;
                            env      >> amp1.in_mod();
                
                            sampler0 >> amp0 >> fader0;
                            sampler1 >> amp1 >> fader1;
                            
                faderControl >> dBtoLin  >> fader0.in_mod();
                                dBtoLin  >> fader1.in_mod();
                
                sampler0.addSample( &sample, 0 );
                sampler1.addSample( &sample, 1 );

                smoothControl >> env.in_attack();
                smoothControl >> env.in_release();

                sample.setVerbose(true);
            }

            void sampleChangedCall( string & value ) {
                ofLogVerbose( "loading" + value );
                loadSample( samplePath );    

                auto v = ofSplitString(samplePath, "/" );
                sampleName = v[v.size()-1];
            }
                
            void loadSample( string path ) {
                sample.load( path );
            }

            void load( string path ) {
                samplePath = path;
            }
                
            float playhead() const {
                return sampler1.meter_position();
            }
            
            pdsp::Sampler       sampler0;
            pdsp::Sampler       sampler1;
            pdsp::ADSR          env;
            pdsp::Amp           amp0;
            pdsp::Amp           amp1;
            
            pdsp::Amp           fader0;
            pdsp::Amp           fader1;
            pdsp::DBtoLin       dBtoLin;


            pdsp::SampleBuffer  sample;    
            ofParameter<string> samplePath;
            ofParameter<string> sampleName;

            pdsp::TriggerControl    sampleTrig;
            pdsp::TriggerControl    envGate;

            ofParameter<void> _openFileChooser;
            ofParameter<bool> _loop;
            pdsp::Parameter     faderControl;    
            pdsp::Parameter     pitchControl;
            pdsp::Parameter     smoothControl;
        };
    }; // namespace clip
}; // namespace ofxLiveSet