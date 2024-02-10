#include "gui_generated.h"
//Auto-generated GUI file for ofxKuTextGui, 2024-02-10-10-18-57-816

Parameters params;
//--------------------------------------------------------------
Parameters::Parameters() {
	_FPS_=30;
	hidden_value=0;
	_w_=1024;
	_h_=768;
	backR=0;
	backG=0;
	backB=0;
	fps_=30;
	send_host="localhost";
	send_port=12345;
	list=0;
	render=0;
}

//--------------------------------------------------------------
void Parameters::setup(ofxKuTextGui &gui, string fileName) {
	gui.addPage("screen");
	gui.addDummy("Constants names starts with *:");
	gui.addInt("*FPS",_FPS_,30,1,100,1,10);
	gui.addDummy("");
	gui.addInt("~hidden_value",hidden_value,0,0,10,1,10);
	gui.addDummy("Screen size:");
	gui.addInt("*w",_w_,1024,1,2000,1,10);
	gui.addInt("*h",_h_,768,1,2000,1,10);
	gui.addDummy("Background color:");
	gui.addInt("backR",backR,0,0,255,1,10);
	gui.addInt("backG",backG,0,0,255,1,10);
	gui.addInt("backB",backB,0,0,255,1,10);
	gui.addTab();
	gui.addTab();
	gui.addDummy("");
	gui.addDummy("Output values names starts with -:");
	gui.addFloat("-fps",fps_,30,0,100,100,10);
	gui.addPage("osc");
	gui.addString("send_host",send_host,"localhost");
	gui.addInt("send_port",send_port,12345,1,65535,1,10);
	gui.addPage("test_list");
	gui.addVar("-fps");
	gui.addDummy("");
	gui.addDummy("Stringlist:");
	gui.addStringList("list",list,0,3,"a","b","c");
	gui.addDummy("");
	gui.addDummy("Button:");
	gui.addButton("render",render);
	gui.set_var_color("*FPS", ofColor(140,140,255));
	gui.set_var_color("~hidden_value", ofColor(140,140,255));
	gui.set_var_visibility("~hidden_value", false);
	gui.set_var_editable("~hidden_value", false);
	gui.set_var_color("*w", ofColor(140,140,255));
	gui.set_var_color("*h", ofColor(140,140,255));
	gui.set_var_editable("-fps", false);
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

