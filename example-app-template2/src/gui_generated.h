#pragma once
//Auto-generated GUI file for ofxKuTextGui, 2024-06-28-10-07-33-950

#include "ofMain.h"
#include "ofxKuTextGui.h"

struct Parameters {
	int FPS;
	int _FPS_;	// const
	int Screen_W;
	int _Screen_W_;	// const
	int Screen_H;
	int _Screen_H_;	// const
	float fps_;	// output
	int Start;
	int Enabled;
	int Model;
	int RGB;
	string Path;
	int hidden_value;	// hidden
	int option1;
	int Enable_AB;
	int Enable_C;
	int Item_Setup;
	int Pos11;
	int Pos12;
	int Pos21;
	int Pos22;
    Parameters();
    void setup(ofxKuTextGui &gui, string fileName);
    void applyConstChanges();
    void save();
    string fileName_;
    ofxKuTextGui *gui_;
};

extern Parameters params;
#define PRM params.
