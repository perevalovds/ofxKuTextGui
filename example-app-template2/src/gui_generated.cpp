#include "gui_generated.h"
//Auto-generated GUI file for ofxKuTextGui, 2024-12-20-21-29-10-331

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
	Item_Setup=0;
	Pos11=0;
	Pos12=0;
	Pos21=0;
	Pos22=0;
	Projection=0;
	Wall_dilate=0;
	Wall_erode=0;
	Wall_filter=0;
	Wall_raw_=0;
	Wall_X0_CM=0;
	Wall_X1_CM=0;
	Wall_Y_CM=0;
	Wall_SclY=0;
	Floor_dilate=0;
	Floor_erode=0;
	Floor_filter=0;
	Floor_raw_=0;
	Floor_X0_CM=0;
	Floor_X1_CM=0;
	Floor_Y_CM=0;
	Floor_SclY=0;
	radio_1=0;
	radio_2=0;
	radio_3=0;
	radio_value_=-1;
	Value_1=1;
	Value_2=3;
	Value_3=5;
	Values_Str_="";
}

//--------------------------------------------------------------
void Parameters::setup(ofxKuTextGui &gui, string fileName) {
	gui.addPage("Settings");
	gui.addDummy("SCREEN", "SCREEN_0");
	gui.addInt("*FPS",_FPS_,30,1,100,1,10);
	gui.addInt("*Screen_W",_Screen_W_,1024,1,2000,1,10);
	gui.addInt("*Screen_H",_Screen_H_,768,1,2000,1,10);
	gui.addFloat("-fps",fps_,30,0,100,100,10);
	gui.addTab();
	gui.addDummy("", "_1");
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
	gui.addStringList("Item_Setup",Item_Setup,0,4,"Item11","Item12","Item21","Item22");
	gui.addDummy("11", "11_2");
	gui.addInt("Pos11",Pos11,0,1,10,1,10);
	gui.addDummy("12", "12_3");
	gui.addInt("Pos12",Pos12,0,1,10,1,10);
	gui.addDummy("21", "21_4");
	gui.addInt("Pos21",Pos21,0,1,10,1,10);
	gui.addDummy("22", "22_5");
	gui.addInt("Pos22",Pos22,0,1,10,1,10);
	gui.addTab();
	gui.addStringList("Projection",Projection,0,2,"WALL","FLOOR");
	gui.addInt("Wall_dilate",Wall_dilate,0,0,16,1,10);
	gui.addInt("Wall_erode",Wall_erode,0,0,16,1,10);
	gui.addInt("Wall_filter",Wall_filter,0,0,400,1,10);
	gui.addInt("-Wall_raw",Wall_raw_,0,0,1000000,1,10);
	gui.addDummy("PRECISE", "PRECISE_6");
	gui.addInt("Wall_X0_CM",Wall_X0_CM,0,-1000,1000,1,10);
	gui.addInt("Wall_X1_CM",Wall_X1_CM,0,-1000,1000,1,10);
	gui.addInt("Wall_Y_CM",Wall_Y_CM,0,-1000,1000,1,10);
	gui.addInt("Wall_SclY",Wall_SclY,0,-1000,1000,1,10);
	gui.addInt("Floor_dilate",Floor_dilate,0,0,16,1,10);
	gui.addInt("Floor_erode",Floor_erode,0,0,16,1,10);
	gui.addInt("Floor_filter",Floor_filter,0,0,400,1,10);
	gui.addInt("-Floor_raw",Floor_raw_,0,0,1000000,1,10);
	gui.addDummy("PRECISE", "PRECISE_7");
	gui.addInt("Floor_X0_CM",Floor_X0_CM,0,-1000,1000,1,10);
	gui.addInt("Floor_X1_CM",Floor_X1_CM,0,-1000,1000,1,10);
	gui.addInt("Floor_Y_CM",Floor_Y_CM,0,-1000,1000,1,10);
	gui.addInt("Floor_SclY",Floor_SclY,0,-1000,1000,1,10);
	gui.addTab();
	gui.addDummy("Radio_Group", "Radio_Group_8");
	gui.addCheckbox("radio_1",radio_1);
	gui.addCheckbox("radio_2",radio_2);
	gui.addCheckbox("radio_3",radio_3);
	gui.addDummy("Test_Radio_Value", "Test_Radio_Value_9");
	gui.addInt("-radio_value",radio_value_,-1,-1,2,1,10);
	gui.addTab();
	gui.addDummy("Array_Test", "Array_Test_10");
	gui.addInt("Value_1",Value_1,1,1,10,1,10);
	gui.addInt("Value_2",Value_2,3,1,10,1,10);
	gui.addInt("Value_3",Value_3,5,1,10,1,10);
	ValuesArray = {&Value_1, &Value_2, &Value_3};
	gui.addString("-Values_Str",Values_Str_,"");
	gui.set_var_color("*FPS", ofColor(255,140,255));
	gui.set_var_color("*Screen_W", ofColor(255,140,255));
	gui.set_var_color("*Screen_H", ofColor(255,140,255));
	gui.set_var_editable("-fps", false);
	gui.set_var_visibility("~hidden_value", false);
	gui.set_var_editable("~hidden_value", false);
	gui.set_var_visibility_conditions("Enable_AB",{ {"option1", {"UseA","UseB"}} });
	gui.set_var_visibility_conditions("Enable_C",{ {"option1", {"UseC"}} });
	gui.set_var_visibility_conditions("11_2",{ {"Item_Setup", {"Item11"}} });
	gui.set_var_visibility_conditions("Pos11",{ {"Item_Setup", {"Item11"}} });
	gui.set_var_visibility_conditions("12_3",{ {"Item_Setup", {"Item12"}} });
	gui.set_var_visibility_conditions("Pos12",{ {"Item_Setup", {"Item12"}} });
	gui.set_var_visibility_conditions("21_4",{ {"Item_Setup", {"Item21"}} });
	gui.set_var_visibility_conditions("Pos21",{ {"Item_Setup", {"Item21"}} });
	gui.set_var_visibility_conditions("22_5",{ {"Item_Setup", {"Item22"}} });
	gui.set_var_visibility_conditions("Pos22",{ {"Item_Setup", {"Item22"}} });
	gui.set_var_visibility_conditions("Wall_dilate",{ {"Projection", {"WALL"}} });
	gui.set_var_visibility_conditions("Wall_erode",{ {"Projection", {"WALL"}} });
	gui.set_var_visibility_conditions("Wall_filter",{ {"Projection", {"WALL"}} });
	gui.set_var_visibility_conditions("-Wall_raw",{ {"Projection", {"WALL"}} });
	gui.set_var_editable("-Wall_raw", false);
	gui.set_var_visibility_conditions("PRECISE_6",{ {"Projection", {"WALL"}} });
	gui.set_var_visibility_conditions("Wall_X0_CM",{ {"Projection", {"WALL"}} });
	gui.set_var_visibility_conditions("Wall_X1_CM",{ {"Projection", {"WALL"}} });
	gui.set_var_visibility_conditions("Wall_Y_CM",{ {"Projection", {"WALL"}} });
	gui.set_var_visibility_conditions("Wall_SclY",{ {"Projection", {"WALL"}} });
	gui.set_var_visibility_conditions("Floor_dilate",{ {"Projection", {"FLOOR"}} });
	gui.set_var_visibility_conditions("Floor_erode",{ {"Projection", {"FLOOR"}} });
	gui.set_var_visibility_conditions("Floor_filter",{ {"Projection", {"FLOOR"}} });
	gui.set_var_visibility_conditions("-Floor_raw",{ {"Projection", {"FLOOR"}} });
	gui.set_var_editable("-Floor_raw", false);
	gui.set_var_visibility_conditions("PRECISE_7",{ {"Projection", {"FLOOR"}} });
	gui.set_var_visibility_conditions("Floor_X0_CM",{ {"Projection", {"FLOOR"}} });
	gui.set_var_visibility_conditions("Floor_X1_CM",{ {"Projection", {"FLOOR"}} });
	gui.set_var_visibility_conditions("Floor_Y_CM",{ {"Projection", {"FLOOR"}} });
	gui.set_var_visibility_conditions("Floor_SclY",{ {"Projection", {"FLOOR"}} });
	gui.set_var_editable("-radio_value", false);
	gui.set_var_editable("-Values_Str", false);
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

