#pragma once

/*
 This example demonstrates transmitting and controlling interface parameters 
 via networking using OSC protocol, 
 see also https://github.com/perevalovds/KuRemote app which indended to control such GUIs remotely.
 
Please exclude ofxKuTextGuiPreset.h/cpp from this project - it requires ofxKu addon, but not used in this example
 */


#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
