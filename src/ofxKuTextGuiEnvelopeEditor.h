#pragma once

//Visual editor of envelopes
#include "ofMain.h"
#include "ofxKuTextGuiEnvelope.h"


struct ofxKuTextGuiEnvelopeEditor_MousePoint {
    float *x;
    float *y;
    int *X, *Y, *W, *H;
	float range0, range1;
	ofxKuTextGuiEnvelopeEditor_MousePoint(
                                          float *x0, float *y0, int *X0, int *Y0, int *W0, int *H0, float range00=0, float range10=1) {
		x = x0;
		y = y0;
		X = X0;
		Y = Y0;
		W = W0;
		H = H0;
		range0 = range00;
		range1 = range10;
	}
	ofPoint to_scr() { return ofPoint(ofMap(*x,0,1,*X,*X+*W), ofMap(*y,range1,range0,*Y,*Y+*H)); }
	bool is_near(ofPoint p, float rad) {
		p = p-to_scr();
		//cout << p.x << " " << p.y << endl;
		return (ofDist(0,0,p.x,p.y)<=rad);
	}
	ofPoint to_coord(ofPoint p) {
		return ofPoint(ofMap(p.x,*X,*X+*W,0,1),ofMap(p.y,*Y,*Y+*H,range1,range0));
	}
	void set_from_scr(ofPoint p) {
		p = to_coord(p);
		*x = ofClamp(p.x,0,1);
		*y = ofClamp(p.y,range0,range1);
	}
};

struct ofxKuTextGuiEnvelopeEditor {
	ofxKuTextGuiEnvelopeEditor();

	void clean();
	void add_point(float *x, float *y, int *X, int *Y, int *W, int *H, float range0=0, float range1=1);
	void add_env(ofxKuTextGuiEnvelope &env, int *X, int *Y, int *W, int *H);

	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	vector<ofxKuTextGuiEnvelopeEditor_MousePoint> points;

	bool dragging;
	int index;
	ofPoint delta;
};