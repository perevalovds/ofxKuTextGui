#pragma once
//Auto-generated GUI file for ofxKuTextGui, 2017-02-25-15-57-50-449

#include "ofMain.h"
#include "ofxKuTextGui.h"

struct Parameters {
	int _FPS_;
	int _w_;
	int _h_;
	float fps_;
	string send_host;
	int send_port;
	int list;
    Parameters();
    void setup_gui(ofxKuTextGui &gui);
};

extern Parameters params;
#define PRM params.
