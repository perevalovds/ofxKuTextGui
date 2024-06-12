#include "gui_generated.h"
//Auto-generated GUI file for ofxKuTextGui, 2024-06-12-15-09-45-832

Parameters params;
//--------------------------------------------------------------
Parameters::Parameters() {
	_FPS_=30;
	_Screen_W_=1024;
	_Screen_H_=768;
	fps_=30;
	Start=0;
	Enabled=0;
	Model=0;
	RGB=0;
	Path="d:\text.txt";
	hidden_value=0;
}

//--------------------------------------------------------------
void Parameters::setup(ofxKuTextGui &gui, string fileName) {
	gui.addPage("SETTINGS");
	gui.addDummy("SCREEN");
	gui.addInt("*FPS",_FPS_,30,1,100,1,10);
	gui.addInt("*Screen_W",_Screen_W_,1024,1,2000,1,10);
	gui.addInt("*Screen_H",_Screen_H_,768,1,2000,1,10);
	gui.addFloat("-fps",fps_,30,0,100,100,10);
	gui.addTab();
	gui.addDummy("");
	gui.addButton("Start",Start);
	gui.addCheckbox("Enabled",Enabled);
	gui.addStringList("Model",Model,0,2,"Realsense","Azure");
	gui.addCheckbox("RGB",RGB);
	gui.addString("Path",Path,"d:\text.txt");
	gui.addInt("~hidden_value",hidden_value,0,0,10,1,10);
	gui.set_var_color("*FPS", ofColor(255,140,255));
	gui.set_var_color("*Screen_W", ofColor(255,140,255));
	gui.set_var_color("*Screen_H", ofColor(255,140,255));
	gui.set_var_editable("-fps", false);
	gui.set_var_visibility("~hidden_value", false);
	gui.set_var_editable("~hidden_value", false);
	fileName_ = fileName;
	gui_ = &gui;
	gui.loadFromFile(fileName);
	applyConstChanges();
}

//--------------------------------------------------------------
void Parameters::applyConstChanges() {
	FPS = _FPS_;
	Screen_W = _Screen_W_;
	Screen_H = _Screen_H_;
}

//--------------------------------------------------------------
void Parameters::save() {
	gui_->saveToFile(fileName_);
}

//--------------------------------------------------------------

