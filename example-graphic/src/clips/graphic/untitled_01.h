#pragma once
#include "clips/graphic.h"

namespace clips {
    class untitled_01 : public ofxLiveSet::clip::graphic {
    public:
        typedef std::shared_ptr<untitled_01> pointer;
        static pointer create(std::string name="untitled_01")
        {
            return std::make_shared<untitled_01>(name);
        }
        
        untitled_01(std::string name = "untitled_01") : graphic(name){
            _name = name;
            _active.setName(_name);
            _primaryColor.set("primaryColor", ofColor::white);
            // _xAmount.addListener(this, &untitled_00::onXAmountOrYAmountChange);
            // _yAmount.addListener(this, &untitled_00::onXAmountOrYAmountChange);
            _speed.set("speed", 1, 0, 10);
            _parameters.add(_primaryColor);
            _parameters.add(_speed);
	
            _isShaderDirty = true;
            ofBoxPrimitive box;
            box.set(2);
            _vbo = box.getMesh();
	
            ofDisableArbTex();
            ofLoadImage(_tex, "vboMesh/depth_image.png");
            ofEnableArbTex();

            _cam.setupPerspective(false,60,0,10000);
            _cam.disableMouseInput();
        }
        void update(){
            if (_isShaderDirty){
		// most of the instanced drawing magic happens in the shaders:
#ifdef USE_PROGRAMMABLE_GL
		// if we are using programmable GL, we load the GLSL version 150 shader pair.
		_shader.load("vboMesh/shaders/instanced.vert", "vboMesh/shaders/instanced.frag");
#else
		// if we are using fixed function GL, we load the GLSL version 120 shader pair.
		_shader.load("vboMesh/shaders/instanced_120.vert", "vboMesh/shaders/instanced_120.frag");
#endif
		        GLint err = glGetError();
		        if (err != GL_NO_ERROR){
			        ofLogNotice() << "Load Shader came back with GL error:	" << err;
		        }
		
		        _isShaderDirty = false;
	        }
            begin();
            ofDisableAlphaBlending();
	        ofEnableArbTex();
            ofSetColor(_primaryColor);
            _cam.begin();
            _shader.begin();
            _shader.setUniformTexture("tex0", _tex, 0);
            _shader.setUniform1f("timeValue", ((ofGetElapsedTimeMillis() % 30000) / 30000.0f)*_speed);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            _vbo.drawInstanced(OF_MESH_FILL, 128*128);
            glDisable(GL_CULL_FACE);
            _shader.end();
            _cam.end();
            end();
            setNewFrame();
        }

        ofParameter<ofColor> _primaryColor;
        ofParameter<int> _xAmount;
        ofParameter<int> _yAmount;
        ofParameter<float> _speed;
        ofParameter<void> _bang;

        std::vector<float> _alphas;
        std::vector<float> _timeOffsets;
        u_int64_t _bangTimestamp;

        ofVboMesh	_vbo;
        ofEasyCam	_cam;
        ofTexture	_tex;

        bool _isShaderDirty;
    };
};