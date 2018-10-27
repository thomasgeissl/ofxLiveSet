#pragma once
#include "ofMain.h"
#include "ofxMQTT.h"

//namespace ofxLiveSet {
//namespace utils {
class mqttSynchroniser {
public:
            mqttSynchroniser() {
    //            ofAddListener(ofEvents().draw, this, &session::onDraw, OF_EVENT_ORDER_AFTER_APP);
    //            ofAddListener(ofEvents().update, this, &session::onUpdate, OF_EVENT_ORDER_AFTER_APP);
            }
    void setup() {
                _client.begin("localhost", 1883);
                _client.connect("openframeworks");
    
                ofAddListener(_client.onOnline, this, &mqttSynchroniser::onOnline);
                ofAddListener(_client.onOffline, this, &mqttSynchroniser::onOffline);
                ofAddListener(_client.onMessage, this, &mqttSynchroniser::onMessage);
            }
            void update() {
                _client.update();
            }
    
    //        void exit(){
    //            _client.disconnect();
    //        }
    
            void onOnline(){
                ofLog() << "online";
    
                _client.subscribe("hello");
            }
    
            void onOffline(){
                ofLog() << "offline";
            }
    
            void onMessage(ofxMQTTMessage &msg){
                ofLog() << "message: " << msg.topic << " - " << msg.payload;
            }
    
            void draw()
            {
            }
            void onUpdate(ofEventArgs &e)
            {
                update();
            }
    
            void onDraw(ofEventArgs &e)
            {
                draw();
            }
    
            ofxMQTT _client;
};
//}; // namespace utils
//}; // namespace ofxLiveSet
