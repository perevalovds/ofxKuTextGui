#include "gui_generated.h"
//Auto-generated GUI file for ofxKuTextGui, 2024-06-27-20-45-55-687

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
	option1=0;
	Enable_AB=0;
	Enable_C=0;
	Pos11=0;
	Pos12=0;
	Pos21=0;
	Pos22=0;
}

//--------------------------------------------------------------
void Parameters::setup(ofxKuTextGui &gui, string fileName) {
	gui.addPage("Settings");
	gui.addDummy("SCREEN");
	gui.addInt("*FPS",_FPS_,30,1,100,1,10);
	gui.addInt("*Screen_W",_Screen_W_,1024,1,2000,1,10);
	gui.addInt("*Screen_H",_Screen_H_,768,1,2000,1,10);
	gui.addFloat("-fps",fps_,30,0,100,100,10);
	gui.addTab();
	gui.addDummy("");
	gui.addButton("Start",Start);
	gui.addCheckbox("Enabled",Enabled);
	gui.addStringList("Model",Model,0,3,"Realsense","KinectOne","Azure");
	gui.addCheckbox("RGB",RGB);
	gui.addString("Path",Path,"d:\text.txt");
	gui.addInt("~hidden_value",hidden_value,0,0,10,1,10);
	gui.addPage("Conditions");
	gui.addStringList("option1",option1,0,3,"UseA","UseB","UseC");
	gui.addCheckbox("Enable_AB",Enable_AB);
	gui.addCheckbox("Enable_C",Enable_C);
	gui.addPage("Templates");
	gui.addDummy("11");
	gui.addInt("Pos11",Pos11,0,1,10,1,10);
	gui.addDummy("12");
	gui.addInt("Pos12",Pos12,0,1,10,1,10);
	gui.addDummy("21");
	gui.addInt("Pos21",Pos21,0,1,10,1,10);
	gui.addDummy("22");
	gui.addInt("Pos22",Pos22,0,1,10,1,10);
	gui.set_var_color("*FPS", ofColor(255,140,255));
	gui.set_var_color("*Screen_W", ofColor(255,140,255));
	gui.set_var_color("*Screen_H", ofColor(255,140,255));
	gui.set_var_editable("-fps", false);
	gui.set_var_visibility("~hidden_value", false);
	gui.set_var_editable("~hidden_value", false);
	gui.set_var_visibility_conditions("Enable_AB",{ {"option1", {"UseA","UseB"}} });
	gui.set_var_visibility_conditions("Enable_C",{ {"option1", {"UseC"}} });
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

