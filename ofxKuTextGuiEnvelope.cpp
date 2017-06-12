#include "ofxKuTextGuiEnvelope.h"


//--------------------------------------------------------------
ofxKuTextGuiEnvelope::ofxKuTextGuiEnvelope() {
	range0_ = 0;
	range1_ = 1;
}

//--------------------------------------------------------------
void ofxKuTextGuiEnvelope::add_point(float *d, float *val, float range0, float range1) {
    D.push_back(d);
    V.push_back(val);
	range0_ = range0;
	range1_ = range1;
}

//--------------------------------------------------------------
void ofxKuTextGuiEnvelope::add_point(ofxKuTextGui &gui, string name_d, string name_val, float range0, float range1) {
    float *d = gui.findVarFloat(name_d);
    float *val = gui.findVarFloat(name_val);
    add_point(d, val, range0, range1);
}

//--------------------------------------------------------------
void ofxKuTextGuiEnvelope::add_points(ofxKuTextGui &gui, string name, int n, float range0, float range1) {
	for (int i=0; i<n; i++) {
		string idname = ofToString(i+1);
		add_point(gui, name + "_d" + idname, name + "_v" + idname, range0, range1);
	}
}

//--------------------------------------------------------------
int ofxKuTextGuiEnvelope::size() {
	return D.size();
}

//--------------------------------------------------------------
float ofxKuTextGuiEnvelope::d(int i, float low, float high) {
    if (i < 0) return low;
    if (i >= D.size()) return high;
    return *D[i];
}

//--------------------------------------------------------------
float ofxKuTextGuiEnvelope::vd(int i) {
    return v_u(d(i));
}

//--------------------------------------------------------------
float ofxKuTextGuiEnvelope::v_u(float d) {    //0..1
    int EnvN = D.size();
    if (EnvN == 0) return 0;
    //search for the segment
    int k = EnvN-1;
    for (int i=0; i<EnvN-1; i++) {
        k = i;
        if (d <= *D[i+1]) {
            break;
        }
    }
    int k1 = min(k+1,EnvN-1);
    float v = ofMap(d,*D[k],*D[k1],*V[k],*V[k1],true);
	v = ofMap(v, range0_, range1_, 0, 1);
    return v;
}

//--------------------------------------------------------------
float ofxKuTextGuiEnvelope::v(float d) {	//range0..range1
	return ofLerp(range0_, range1_, v_u(d));
}

//--------------------------------------------------------------
void ofxKuTextGuiEnvelope::drawGrid(ofRectangle rect, ofColor color, string title) {
	ofPushMatrix();
    ofTranslate(rect.x+0.5, rect.y+0.5);
	ofScale(rect.width, rect.height);
    ofSetColor(color);
    ofNoFill();
    ofDrawRectangle(0,0,1,1);
    ofSetColor(90);
    ofLine(0,0.5,1,0.5);
    ofLine(0.25,0,0.25,1);
    ofLine(0.5,0,0.5,1);
    ofLine(0.75,0,0.75,1);
    
    ofPopMatrix();
	if (title != "") {
		ofDrawBitmapStringHighlight(title, rect.x, rect.y-10);
	}
}

//--------------------------------------------------------------
void ofxKuTextGuiEnvelope::draw(ofRectangle rect, ofColor color) {
    ofPushMatrix();
    ofTranslate(rect.x+0.5, rect.y + rect.height+ 0.5);
	ofScale(rect.width, -rect.height);
    
    //Graph
    ofSetColor(color);
    for (int i=-1; i<size(); i++) {
        ofLine(d(i),vd(i),d(i+1),vd(i+1));
    }

	//Squares
    ofSetColor(color);
    ofNoFill();
    float w = 3.0/rect.width;
	float h = 3.0/rect.height;
    for (int i=0; i<size(); i++) {
        ofRect(d(i)-w,vd(i)-h,2*w,2*h);
    }
   
    ofPopMatrix();
}
 