#pragma once
//Auto-generated GUI file for ofxKuTextGui, 2024-02-10-10-18-57-816

#include "ofMain.h"
#include "ofxKuTextGui.h"

struct Parameters {
	int FPS;
	int _FPS_;	// const
	int hidden_value;	// hidden
	int w;
	int _w_;	// const
	int h;
	int _h_;	// const
	int backR;
	int backG;
	int backB;
	float fps_;	// output
	string send_host;
	int send_port;
	int list;
	int render;
    Parameters();
    void setup(ofxKuTextGui &gui, string fileName);
    void save();
    string fileName_;
    ofxKuTextGui *gui_;
};

extern Parameters params;
#define PRM params.
