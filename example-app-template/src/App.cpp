#include "App.h"

#include "gui_generated.h"


extern ofxKuTextGui gui;	//access to GUI object
	
//--------------------------------------------------------------
void App::setup(){

	time0_ = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void App::update(){	
	float time = ofGetElapsedTimef();
	float dt = time - time0_;
	time0_ = time;

	//Smoothing values
	float time_smooth = PRM time_smoothing;
	float smoothed1 = gui.updateSmoothedValue("smooth_float", dt, time_smooth);
	float smoothed2 = gui.updateSmoothedValue("smooth_int", dt, time_smooth);

	//Access to a smoothed value
	cout << "smooth_float smoothed = " << gui.getSmoothedValue("smooth_float") << endl;

}

//--------------------------------------------------------------
void App::draw() {

}

//--------------------------------------------------------------
