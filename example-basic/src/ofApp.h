#pragma once

#include "ofMain.h"
#include "ofxKuTextGui.h"

//Please exclude ofxKuTextGuiRemote.h/cpp from this project - it requires ofxOsc addon, but not used in this example,
//and ofxKuTextGuiPreset.h/cpp - it reauires ofxKu addon, but not used here too.


class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

    ofTrueTypeFont gui_font;
	ofxKuTextGui gui;
	int page = 0;
	int countX = 5;
	int countY = 5;
	float size = 30;
	float step = 50;

	int enabled = 0;

	int red = 255;
	int green = 255;
	int blue = 255;

	string folder1 = "folderA";
	string folder2 = "folderB";

	int save_btn = 0;
	int load_btn = 0;
};
