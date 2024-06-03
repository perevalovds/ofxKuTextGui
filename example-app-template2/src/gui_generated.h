#pragma once
//Auto-generated GUI file for ofxKuTextGui, 2024-06-03-18-39-49-246

#include "ofMain.h"
#include "ofxKuTextGui.h"

struct Parameters {
	int FPS;
	int _FPS_;	// const
	int scr_w;
	int _scr_w_;	// const
	int scr_h;
	int _scr_h_;	// const
	int Enabled;
	float fps_;	// output
	int Start;
	int Enable;
	string path;
	int hidden_value;	// hidden
    Parameters();
    void setup(ofxKuTextGui &gui, string fileName);
    void applyConstChanges();
    void save();
    string fileName_;
    ofxKuTextGui *gui_;
};

extern Parameters params;
#define PRM params.
