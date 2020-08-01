#pragma once
//Auto-generated GUI file for ofxKuTextGui, 2020-07-20-09-43-58-715

#include "ofMain.h"
#include "ofxKuTextGui.h"

struct Parameters {
	float show_zoom;
	int Analyze;
	string FILE;
	string _FILE_;
	int seconds;
	int _seconds_;
	int FPS;
	int _FPS_;
	int scr_w;
	int _scr_w_;
	int scr_h;
	int _scr_h_;
    Parameters();
    void setup(ofxKuTextGui &gui, string fileName);
    void save();
    string fileName_;
    ofxKuTextGui *gui_;
};

extern Parameters params;
#define PRM params.
