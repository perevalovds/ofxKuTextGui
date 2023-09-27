#pragma once
//Auto-generated GUI file for ofxKuTextGui, 2023-09-27-08-43-38-801

#include "ofMain.h"
#include "ofxKuTextGui.h"

struct Parameters {
	int FPS;
	int _FPS_;
	int scr_w;
	int _scr_w_;
	int scr_h;
	int _scr_h_;
	float fps_;
    Parameters();
    void setup(ofxKuTextGui &gui, string fileName);
    void save();
    string fileName_;
    ofxKuTextGui *gui_;
};

extern Parameters params;
#define PRM params.
