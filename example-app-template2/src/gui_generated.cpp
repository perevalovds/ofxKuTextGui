#include "gui_generated.h"
//Auto-generated GUI file for ofxKuTextGui, 2024-02-10-10-08-43-578

Parameters params;
//--------------------------------------------------------------
Parameters::Parameters() {
	_FPS_=30;
	_scr_w_=1024;
	_scr_h_=768;
	fps_=30;
	Start=0;
	Enable=0;
	path="d:\text.txt";
	hidden_value=0;
}

//--------------------------------------------------------------
void Parameters::setup(ofxKuTextGui &gui, string fileName) {
	gui.addPage("SETTINGS");
	gui.addDummy("SCREEN");
	gui.addInt("*FPS",_FPS_,30,1,100,1,10);
	gui.addInt("*scr_w",_scr_w_,1024,1,2000,1,10);
	gui.addInt("*scr_h",_scr_h_,768,1,2000,1,10);
	gui.addFloat("-fps",fps_,30,0,100,100,10);
	gui.addTab();
	gui.addDummy("");
	gui.addButton("Start",Start);
	gui.addStringList("Enable",Enable,0,2,"OFF","ON");
	gui.addString("path",path,"d:\text.txt");
	gui.addInt("~hidden_value",hidden_value,0,0,10,1,10);
	gui.set_var_color("*FPS", ofColor(255,140,255));
	gui.set_var_color("*scr_w", ofColor(255,140,255));
	gui.set_var_color("*scr_h", ofColor(255,140,255));
	gui.set_var_editable("-fps", false);
	gui.set_var_visibility("~hidden_value", false);
	gui.set_var_editable("~hidden_value", false);
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

