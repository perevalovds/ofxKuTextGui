#pragma once
//Auto-generated GUI file for ofxKuTextGui, 2024-06-30-21-33-38-163

#include "ofMain.h"
#include "ofxKuTextGui.h"

struct Parameters {
    enum PageEnum: int {
        Settings = 0,
        Conditions = 1,
        Templates = 2
    };
    PageEnum pageEnum() { return PageEnum(gui_->pageIndex()); } 
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
	int Projection;
	int Wall_dilate;
	int Wall_erode;
	int Wall_filter;
	int Wall_raw_;	// output
	int Wall_X0_CM;
	int Wall_X1_CM;
	int Wall_Y_CM;
	int Wall_SclY;
	int Floor_dilate;
	int Floor_erode;
	int Floor_filter;
	int Floor_raw_;	// output
	int Floor_X0_CM;
	int Floor_X1_CM;
	int Floor_Y_CM;
	int Floor_SclY;
    Parameters();
    void setup(ofxKuTextGui &gui, string fileName);
    void applyConstChanges();
    void save();
    string fileName_;
    ofxKuTextGui *gui_;
};

extern Parameters params;
#define PRM params.
