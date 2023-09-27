#include "gui_generated.h"
//Auto-generated GUI file for ofxKuTextGui, 2023-09-27-08-43-38-802

Parameters params;
//--------------------------------------------------------------
Parameters::Parameters() {
	_FPS_=30;
	_scr_w_=1024;
	_scr_h_=768;
	fps_=30;
}

//--------------------------------------------------------------
void Parameters::setup(ofxKuTextGui &gui, string fileName) {
	gui.addPage("SETTINGS");
	gui.addDummy("SCREEN");
	gui.addInt("*FPS",_FPS_,30,1,100,1,10);
	gui.addInt("*scr_w",_scr_w_,1024,1,2000,1,10);
	gui.addInt("*scr_h",_scr_h_,768,1,2000,1,10);
	gui.addFloat("-fps",fps_,30,0,100,100,10);
	gui.set_var_color("*FPS", ofColor(255,140,255));
	gui.set_var_color("*scr_w", ofColor(255,140,255));
	gui.set_var_color("*scr_h", ofColor(255,140,255));
	fileName_ = fileName;
	gui_ = &gui;
	gui.loadFromFile(fileName);
	FPS = _FPS_;
	scr_w = _scr_w_;
	scr_h = _scr_h_;
}

//--------------------------------------------------------------
void Parameters::save() {
	gui_->saveToFile(fileName_);
}

//--------------------------------------------------------------

