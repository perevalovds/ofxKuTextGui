#include "ofApp.h"

//Please exclude ofxKuTextGuiRemote.h/cpp from this project - it requires ofxOsc addon, but not used in this example,
//and ofxKuTextGuiPreset.h/cpp - it reauires ofxKu addon, but not used here too.


#include "ofxKuTextGui.h"
#include "ofxKuTextGuiGen.h"

ofTrueTypeFont gui_font;
ofxKuTextGui gui;

//--------------------------------------------------------------
void ofApp::setup(){
    gui_font.load("fonts/frabk.ttf", 15);
    gui.set_font(&gui_font, 0, 0);
    
	ofxKuTextGuiGen::createGuiFromFile(gui, "gui-script.ini");
	gui.loadFromFile("param.ini");
    
}

//--------------------------------------------------------------
void ofApp::exit(){
	gui.saveToFile("param.ini");
}

//--------------------------------------------------------------
void ofApp::update(){
    //Access to parameters
    gui.float_("-fps") = ofGetFrameRate();
	
	gui.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
	//Access to parameters
	ofBackground(gui.int_("backR"), gui.int_("backG"), gui.int_("backB"));


	ofDrawBitmapStringHighlight("Demo of creating GUI from GUI script dynamically", 20, 20);

    gui.draw(20,80);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    gui.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	gui.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
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
