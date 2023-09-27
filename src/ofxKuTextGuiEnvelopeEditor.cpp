#include "ofxKuTextGuiEnvelopeEditor.h"

//--------------------------------------------------------------
ofxKuTextGuiEnvelopeEditor::ofxKuTextGuiEnvelopeEditor() {
	clean();
}

//--------------------------------------------------------------
void ofxKuTextGuiEnvelopeEditor::clean() {
	dragging = false;
	index = -1;
	points.clear();
}

//--------------------------------------------------------------
void ofxKuTextGuiEnvelopeEditor::add_point(float *x, float *y, int *X, int *Y, int *W, int *H, float range0, float range1) {
	points.push_back(ofxKuTextGuiEnvelopeEditor_MousePoint(x,y,X,Y,W,H,range0,range1));
}

//--------------------------------------------------------------
void ofxKuTextGuiEnvelopeEditor::add_env(ofxKuTextGuiEnvelope &env, int *X, int *Y, int *W, int *H) {
	for (int i=0; i<env.D.size(); i++) {
		add_point(env.D[i], env.V[i], X, Y, W, H, env.range0_, env.range1_);
	}

}

//--------------------------------------------------------------
void ofxKuTextGuiEnvelopeEditor::mousePressed(int x, int y, int button) {
	float dist = 7;
	ofPoint p(x,y);
	for (int i=0; i<points.size(); i++) {
		if (points[i].is_near(p, dist)) {
			dragging = true;
			index = i;
			delta = points[i].to_scr() - p;
			//cout << "drag " << i << endl;
			return;
		}
	}
}

//--------------------------------------------------------------
void ofxKuTextGuiEnvelopeEditor::mouseDragged(int x, int y, int button) {
	if (dragging && index >= 0 && index < points.size()) {
		ofPoint p(x,y);
		points[index].set_from_scr(p+delta);
	}

}

//--------------------------------------------------------------
void ofxKuTextGuiEnvelopeEditor::mouseReleased(int x, int y, int button) {
	mouseDragged(x,y,button);
	dragging = false;
};

//--------------------------------------------------------------
