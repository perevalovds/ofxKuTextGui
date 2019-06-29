#pragma once
//Auto-generated GUI file for ofxKuTextGui, 2019-06-29-11-06-04-745

#include "ofMain.h"
#include "ofxKuTextGui.h"

struct Parameters {
	int FPS;
	int _FPS_;
	int w;
	int _w_;
	int h;
	int _h_;
	int backR;
	int backG;
	int backB;
	float fps_;
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
