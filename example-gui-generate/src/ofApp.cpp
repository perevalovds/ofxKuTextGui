#include "ofApp.h"

//Please exclude ofxKuTextGuiRemote.h/cpp from this project - it requires ofxOsc addon, but not used in this example,
//and ofxKuTextGuiPreset.h/cpp - it reauires ofxKu addon, but not used here too.


#include "ofxKuTextGui.h"
#include "ofxKuTextGuiGen.h"
#include "gui_generated.h"

ofTrueTypeFont gui_font;
ofxKuTextGui gui;


int page;
int list1;

//--------------------------------------------------------------
void ofApp::setup(){
    gui_font.load("fonts/frabk.ttf", 15);
    gui.set_font(&gui_font, 0, 0);
    
    PRM setup(gui,"settings.ini");

	gui.set_dummy_color(255);
    
    
    //Access to constant parameter
    ofSetFrameRate(PRM FPS);	//*FPS means it's constant value, its defined as FPS, and its dynamic version is PRM _FPS_
	//PRM FPS is not changing until restarting app
}

//--------------------------------------------------------------
void ofApp::exit(){
	PRM save();
}

//--------------------------------------------------------------
void ofApp::update(){
    //Access to output parameter
    PRM fps_ = ofGetFrameRate();		//"-fps" value is defined as fps_, meaning it's output value
	
	gui.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(PRM backR, PRM backG, PRM backB);
	ofDrawBitmapStringHighlight("Demo of generating GUI H/CPP files from GUI script\n\nPress Shift+G to re-generate gui_generated.h/cpp files from gui-script.ini file", 20, 20);

    gui.draw(20,80);
    
    //Access to normal parameter, its named literally: send_port is again send_port:
    //ofDrawBitmapStringHighlight("OSC port: " + ofToString(PRM send_port), 20, 300);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'G') {   //Generate .H and .CPP files for parameters
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
