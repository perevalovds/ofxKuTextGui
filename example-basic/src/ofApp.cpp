#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	gui.addPage("page1");	
	gui.addInt("countX", countX, countX, 1, 20, 1, 10);
	gui.addInt("countY", countY, countY, 1, 20, 1, 10);

	gui.addFloat("step", step, step, 0, 200, 200, 20);
	gui.addFloat("size", size, size, 0, 200, 200, 20);

	gui.addTab();
	gui.addInt("red", red, red, 0, 255, 1, 10);
	gui.addInt("green", green, green, 0, 255, 1, 10);
	gui.addInt("blue", blue, blue, 0, 255, 1, 10);

	gui.addPage("page2");
	gui.addVar("countX");	//use addVar(..) to add the variables which exists on the other pages
	gui.addVar("countY");
	gui.addTab();
	gui.addString("folder1", folder1, folder1);
	gui.addString("folder2", folder2, folder2);
	
	gui.loadFromFile("params.ini");	//load values from a file after all variables are added
}

//--------------------------------------------------------------
void ofApp::exit() {
	gui.saveToFile("params.ini");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(64);
	ofSetColor(red, green, blue);
	ofFill();
	for (int y = 0; y < countY; y++) {
		for (int x = 0; x < countX; x++) {
			ofDrawRectangle(x*step + 50, y*step + 200, size, size);
		}
	}
	ofDrawBitmapStringHighlight("ofxKuTextGui demo\n\nKeys: 1,2 - switch page, cursor - select variable, [,] - slow change, {,} - fast change", 20, 20);


	gui.draw(20, 80);
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
