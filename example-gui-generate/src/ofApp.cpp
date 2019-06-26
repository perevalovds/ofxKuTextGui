#include "ofApp.h"

//Please exclude ofxKuTextGuiRemote.h/cpp from this project - it requires ofxOsc addon, but not used in this example,
//and ofxKuTextGuiPreset.h/cpp - it reauires ofxKu addon, but not used here too.


#include "ofxKuTextGui.h"
#include "ofxKuTextGuiGen.h"
#include "gui_generated.h"

ofxKuTextGui gui;


int page;
int list1;

//--------------------------------------------------------------
void ofApp::setup(){
    PRM setup(gui,"param.ini");
    
    
    //Access to parameters
    ofSetFrameRate(PRM _FPS_);
}

//--------------------------------------------------------------
void ofApp::exit(){
	PRM save();
}

//--------------------------------------------------------------
void ofApp::update(){
    //Access to parameters
    PRM fps_ = ofGetFrameRate();
	
	gui.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw(20,20);
    
    //Access to parameters
    ofSetColor(0);
    ofDrawBitmapString("OSC port: " + ofToString(PRM send_port), 20, 150);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'g') {   //Generate .H and .CPP files for parameters
        ofxKuTextGuiGen::generateCPP("gui-script.ini",
                                     "../../src/", "gui_generated",
                                     "Parameters", "params", "PRM");
    }
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

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
