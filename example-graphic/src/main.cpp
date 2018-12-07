#include "ofMain.h"
#include "ofApp.h"
#include "renderApp.h"
#include "ofAppGLFWWindow.h"

//#define USETWOWINDOWS

int main( ){
	// #ifdef USETWOWINDOWS
	ofGLFWWindowSettings settings;
    // settings.setSize(1024, 768);
	// settings.setPosition(ofVec2f(300,0));
	// settings.resizable = true;
	// shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

    // settings.setSize(1024, 768);
	// settings.setPosition(ofVec2f(0,0));
	// settings.resizable = true;
	// // uncomment next line to share main's OpenGL resources with gui
	// settings.shareContextWith = mainWindow;	
	// shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
	// guiWindow->setVerticalSync(false);

	// shared_ptr<ofApp> mainApp(new ofApp);
	// // mainApp->setupGui();
	// // TODO: dont know why it is working, might be because of the OF_EVENT_ORDER_AFTER_APP
	// // ofAddListener(guiWindow->events().draw,mainApp.get(),&ofApp::drawGui);

	// ofRunApp(mainWindow, mainApp);
	// ofRunMainLoop();
	// #else
	// ofSetupOpenGL(1024, 768, OF_WINDOW);
	// ofRunApp(new ofApp());
	// #endif

    settings.setSize(1024, 768);
	settings.setPosition(ofVec2f(0,0));
	settings.resizable = true;
	shared_ptr<ofAppBaseWindow> controlWindow = ofCreateWindow(settings);

    settings.setSize(1024, 768);
	settings.setPosition(ofVec2f(0,0));
	settings.resizable = true;
	settings.shareContextWith = controlWindow;
	shared_ptr<ofAppBaseWindow> renderWindow = ofCreateWindow(settings);

	shared_ptr<ofApp> controlApp(new ofApp);
	shared_ptr<renderApp> renderAppPtr(new renderApp);
	controlApp->_renderApp = renderAppPtr;

	ofRunApp(controlWindow, controlApp);
	ofRunApp(renderWindow, renderAppPtr);
	ofRunMainLoop();
}