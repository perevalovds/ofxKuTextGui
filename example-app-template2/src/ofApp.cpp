#include "ofApp.h"

#include "ofxKuTextGui.h"
#include "ofxKuTextGuiGen.h"
#include "gui_generated.h"

ofTrueTypeFont gui_font;
ofxKuTextGui gui;
float flash_ = -1000;	//flash time
string Title = "YourApp 1.01";

//--------------------------------------------------------------
void ofApp::setup(){
    cout << "-----------------------------------------------------------------" << endl;
    cout << Title << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "Keys: Esc - exit, Return - hide/show GUI" << endl;
    cout << "Cursor,mouse click - choose parameter" << endl;
    cout << "[,], drag with LMB - change parameter slowly" << endl;
    cout << "{,}, drag with RMB - change parameter faster" << endl;
    cout << "s, l - load / save params" << endl;
    cout << "Shift+g - generate code using gui/gui-script.ini" << endl;
    cout << "-----------------------------------------------------------------" << endl;
 
    gui_font.load("fonts/frabk.ttf", 15);
    gui.set_font(&gui_font, 0, 0);

    PRM setup(gui, "settings.ini");
    gui.set_dummy_color(255);

    //--------------------------------------
   //Screen
    ofSetWindowTitle(Title);
    ofSetWindowShape(PRM scr_w, PRM scr_h);
    ofSetFrameRate(PRM FPS);
}

//--------------------------------------------------------------
void ofApp::exit() {
    cout << "Exit..." << endl;
}

//--------------------------------------------------------------
void ofApp::load() {
    cout << "Load..." << endl;
    gui.loadFromFile("settings.ini");
    flash();
}

//--------------------------------------------------------------
void ofApp::save() {
    cout << "Save..." << endl;
    PRM save();
    flash();
}

//--------------------------------------------------------------
void ofApp::flash() {
    flash_ = ofGetElapsedTimef();
}


//--------------------------------------------------------------
void ofApp::update(){
    //Access to output parameter
    PRM fps_ = ofGetFrameRate();		//"-fps" value is defined as fps_, meaning it's output value

    gui.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    float time = ofGetElapsedTimef();
    float W = ofGetWidth();
    float H = ofGetHeight();

    gui.draw(20, 80);

    float flash = ofMap(time, flash_, flash_ + 0.3, 0.3, 0, true);
    if (flash > 0) {
        ofSetColor(255, flash * 255);
        ofFill();
        ofDrawRectangle(0, 0, W, H);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'G') {   //Generate .H and .CPP files for parameters
        ofxKuTextGuiGen::generateCPP("gui/gui-script.ini",
            "../../src/", "gui_generated",
            "Parameters", "params", "PRM");
        flash();
    }

    if (key == 's') save();
    if (key == 'l') load();

    gui.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    gui.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
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
