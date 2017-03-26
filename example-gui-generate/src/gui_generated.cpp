#include "gui_generated.h"
//Auto-generated GUI file for ofxKuTextGui, 2017-03-26-12-20-35-170

Parameters params;
//--------------------------------------------------------------
Parameters::Parameters() {
	_FPS_=30;
	_w_=1024;
	_h_=768;
	fps_=30;
	send_host="localhost";
	send_port=12345;
	list=0;
}

//--------------------------------------------------------------
void Parameters::setup(ofxKuTextGui &gui, string fileName) {
	gui.addPage("screen");
	gui.addInt("*FPS",_FPS_,30,1,100,1,10);
	gui.addInt("*w",_w_,1024,1,2000,1,10);
	gui.addInt("*h",_h_,768,1,2000,1,10);
	gui.addTab();
	gui.addFloat("-fps",fps_,30,0,100,100,10);
	gui.addPage("osc");
	gui.addString("send_host",send_host,"localhost");
	gui.addInt("send_port",send_port,12345,1,65535,1,10);
	gui.addPage("test_list");
	gui.addVar("-fps");
	gui.addStringList("list",list,0,3,"a","b","c");
	fileName_ = fileName;
	gui_ = &gui;
	gui.loadFromFile(fileName);
	FPS = _FPS_;
	w = _w_;
	h = _h_;
}

//--------------------------------------------------------------
void Parameters::save() {
	gui_->saveToFile(fileName_);
}

//--------------------------------------------------------------

