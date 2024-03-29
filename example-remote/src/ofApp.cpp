#include "ofApp.h"

/*
 This example demonstrates transmitting and controlling interface parameters
 via networking using OSC protocol,
 see also https://github.com/perevalovds/KuRemote app which indended to control such GUIs remotely.

Please exclude ofxKuTextGuiPreset.h/cpp from this project - it requires ofxKu addon, but not used in this example
 */

#include "ofxKuTextGuiRemote.h"


ofTrueTypeFont gui_font;
ofxKuTextGui gui;
ofxKuTextGuiRemoteServer server;
ofxKuTextGuiRemoteClient client;

int a = 5;
float b = 0.5;
//--------------------------------------------------------------
void ofApp::setup(){
    server.setup(12345, this, &gui);
    client.setup("localhost", 12345, 12346);
    
    gui_font.load("fonts/frabk.ttf", 15);
    gui.set_font(&gui_font, 0, 0);
    
    gui.addInt("a", a, a, 0, 10, 1, 10);
    gui.addFloat("b", b, b, 0, 1, 100, 10);
}

//--------------------------------------------------------------
void ofApp::update(){
    server.update();
    client.update();
	gui.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    client.draw(20, 20);
    gui.draw(300, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    gui.keyPressed(key);
    //client.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    //client.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    //client.mouseDragged(x, y, button);
	gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //client.mousePressed(x, y, button);
	gui.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    //client.mouseReleased();
	gui.mouseReleased();
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
