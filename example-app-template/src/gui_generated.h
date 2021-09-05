#pragma once
//Auto-generated GUI file for ofxKuTextGui, 2021-09-05-10-33-30-336

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
	float smooth_float;
	int smooth_int;
	float time_smoothing;
    Parameters();
    void setup(ofxKuTextGui &gui, string fileName);
    void save();
    string fileName_;
    ofxKuTextGui *gui_;
};

extern Parameters params;
#define PRM params.
