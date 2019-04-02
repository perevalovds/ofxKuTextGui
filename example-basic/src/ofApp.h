#pragma once

#include "ofMain.h"
#include "ofxKuTextGui.h"

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


	ofxKuTextGui gui;
	int page = 0;
	int countX = 5;
	int countY = 5;
	float size = 30;
	float step = 50;

	int red = 255;
	int green = 255;
	int blue = 255;

	string folder1 = "folderA";
	string folder2 = "folderB";

	int render = 0;
};
