#include "gui_generated.h"
//Auto-generated GUI file for ofxKuTextGui, 2021-09-05-10-33-30-336

Parameters params;
//--------------------------------------------------------------
Parameters::Parameters() {
	show_zoom=0;
	Analyze=0;
	_FILE_="05_2.36";
	_seconds_=20;
	_FPS_=30;
	_scr_w_=1024;
	_scr_h_=768;
	smooth_float=0;
	smooth_int=5;
	time_smoothing=0.1;
}

//--------------------------------------------------------------
void Parameters::setup(ofxKuTextGui &gui, string fileName) {
	gui.addPage("Main");
	gui.addDummy("Analyze");
	gui.addFloat("show_zoom",show_zoom,0,0,5,100,10);
	gui.addButton("Analyze",Analyze);
	gui.addDummy("File");
	gui.addString("*FILE",_FILE_,"05_2.36");
	gui.addInt("*seconds",_seconds_,20,1,1000,1,10);
	gui.addDummy("Screen");
	gui.addInt("*FPS",_FPS_,30,1,100,1,10);
	gui.addInt("*scr_w",_scr_w_,1024,1,2000,1,10);
	gui.addInt("*scr_h",_scr_h_,768,1,2000,1,10);
	gui.addTab();
	gui.addTab();
	gui.addDummy("SmoothedValue");
	gui.addFloat("smooth_float",smooth_float,0,0,10,100,10);
	gui.addInt("smooth_int",smooth_int,5,-10,10,1,10);
	gui.addDummy("");
	gui.addFloat("time_smoothing",time_smoothing,0.1,0,10,1000,100);
	gui.set_var_color("show_zoom", ofColor(140,140,255));
	gui.set_var_color("Analyze", ofColor(140,140,255));
	gui.set_var_color("*FILE", ofColor(200,200,0));
	gui.set_var_color("*seconds", ofColor(200,200,0));
	gui.set_var_color("*FPS", ofColor(255,140,255));
	gui.set_var_color("*scr_w", ofColor(255,140,255));
	gui.set_var_color("*scr_h", ofColor(255,140,255));
	gui.set_var_color("smooth_float", ofColor(255,100,100));
	gui.set_var_color("smooth_int", ofColor(255,100,100));
	gui.set_var_color("time_smoothing", ofColor(200,200,200));
	fileName_ = fileName;
	gui_ = &gui;
	gui.loadFromFile(fileName);
	FILE = _FILE_;
	seconds = _seconds_;
	FPS = _FPS_;
	scr_w = _scr_w_;
	scr_h = _scr_h_;
}

//--------------------------------------------------------------
void Parameters::save() {
	gui_->saveToFile(fileName_);
}

//--------------------------------------------------------------

