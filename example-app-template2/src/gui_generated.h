#pragma once
//Auto-generated GUI file for ofxKuTextGui, 2024-02-10-10-08-43-578

#include "ofMain.h"
#include "ofxKuTextGui.h"

struct Parameters {
	int FPS;
	int _FPS_;	// const
	int scr_w;
	int _scr_w_;	// const
	int scr_h;
	int _scr_h_;	// const
	float fps_;	// output
	int Start;
	int Enable;
	string path;
	int hidden_value;	// hidden
    Parameters();
    void setup(ofxKuTextGui &gui, string fileName);
    void save();
    string fileName_;
    ofxKuTextGui *gui_;
};

extern Parameters params;
#define PRM params.
