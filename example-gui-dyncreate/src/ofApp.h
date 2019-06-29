#pragma once

//Please exclude ofxKuTextGuiRemote.h/cpp from this project - it requires ofxOsc addon, but not used in this example,
//and ofxKuTextGuiPreset.h/cpp - it reauires ofxKu addon, but not used here too.

//This example demonstrates dynamic loading GUI from a file, without source compilation

#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
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
