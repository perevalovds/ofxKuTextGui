#pragma once

//Envelope
#include "ofMain.h"
#include "ofxKuTextGui.h"

class ofxKuTextGuiEnvelope {
public:
	ofxKuTextGuiEnvelope();
    void add_point(float *d, float *val, float range0=0, float range1=1);
    void add_point(ofxKuTextGui &gui, string name_d, string name_val, float range0=0, float range1=1);
	void add_points(ofxKuTextGui &gui, string name, int n, float range0=0, float range1=1);
    //name + "_d1,...", name + "_v1,..."
    
    float v(float d);	//range0..range1
	float v_u(float d);	//0..1
	int size();
    
    float d(int i, float low=0, float high=1); //could use <=-1 and >=size+1 - then returns low и high
    float vd(int i);    //envelope point with index i
    
    vector<float *> D;
    vector<float *> V;
	float range0_, range1_;

	void drawGrid(ofRectangle rect, ofColor color, string title="");
	void draw(ofRectangle rect, ofColor color);

private:
	

};
