#include "ofApp.h"

//Please exclude ofxKuTextGuiRemote.h/cpp from this project - it requires ofxOsc addon, but not used in this example,
//and ofxKuTextGuiPreset.h/cpp - it reauires ofxKu addon, but not used here too.

//--------------------------------------------------------------
void ofApp::setup(){
    gui_font.load("fonts/frabk.ttf", 15);
    gui.set_font(&gui_font, 0, 0);

	gui.set_tab_w(180);
	//gui.set_dummy_color(255);
	
	gui.addPage("page1");	
	gui.addInt("countX", countX, countX, 1, 20, 1, 10);
	gui.addInt("countY", countY, countY, 1, 20, 1, 10);
	gui.addDummy();

	gui.addFloat("step", step, step, 0, 200, 200, 20);
	gui.addFloat("size", size, size, 0, 200, 200, 20);

	gui.addDummy();

	gui.addDummy("Stringlist:");
	vector<string> list;
	list.push_back("OFF");
	list.push_back("ON");
	gui.addStringList("enabled", enabled, 1, list);

	gui.addTab();
	gui.addInt("red", red, red, 0, 255, 1, 10);
	gui.addInt("green", green, green, 0, 255, 1, 10);
	gui.addInt("blue", blue, blue, 0, 255, 1, 10);

	gui.addDummy();
	gui.addDummy("Buttons:");
	gui.addButton("save", save_btn);
	gui.addButton("load", load_btn);

	gui.addPage("page2");
	gui.addVar("countX");	//use addVar(..) to add the variables which exists on the other pages
	gui.addVar("countY");
	gui.addTab();
	gui.addString("folder1", folder1, folder1);
	gui.addString("folder2", folder2, folder2);

	//Changing colors
	gui.set_var_color("enabled", ofColor(255, 140, 140));

	gui.loadFromFile("params.ini");	//load values from a file after all variables are added
}

//--------------------------------------------------------------
void ofApp::exit() {
	gui.saveToFile("params.ini");
}

//--------------------------------------------------------------
void ofApp::update(){
	gui.update();

	//Checking buttons
	//Buttons fires only once after pressing, and then set to 0 automatically
	if (save_btn) {
		gui.saveToFile("params.ini");
	}
	if (load_btn) {
		gui.loadFromFile("params.ini");
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(64);
	if (enabled) {
		ofSetColor(red, green, blue);
		ofFill();
		for (int y = 0; y < countY; y++) {
			for (int x = 0; x < countX; x++) {
				ofDrawRectangle(x*step + 50, y*step + 320, size, size);
			}
		}
	}
	ofDrawBitmapStringHighlight("ofxKuTextGui demo\n\nKeys: 1,2 - switch page, cursor - select variable, [,] - slow change, {,} - fast change\n\nmouse left/right button - slow/fast change", 50, 20);


	gui.draw(60, 120);
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
