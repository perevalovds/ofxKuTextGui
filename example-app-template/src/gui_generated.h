#pragma once
//Auto-generated GUI file for ofxKuTextGui, 2024-02-10-10-04-24-999

#include "ofMain.h"
#include "ofxKuTextGui.h"

struct Parameters {
	float show_zoom;
	int Analyze;
	int Result_;	// output
	string FILE;
	string _FILE_;	// const
	int seconds;
	int _seconds_;	// const
	int FPS;
	int _FPS_;	// const
	int scr_w;
	int _scr_w_;	// const
	int scr_h;
	int _scr_h_;	// const
	int hidden_value;	// hidden
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
