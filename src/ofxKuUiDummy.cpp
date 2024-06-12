#include "ofxKuUiDummy.h"

//------------------------------------------------------------------------
void KuUiDummy::draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) {
	//dummy's text
	if (!title().empty()) {
		//back
		ofFill();
		ofSetColor(dd.dummy_back);
		//ofSetColor(0, alpha_slider);
		ofDrawRectangle(dc.x0, dc.y0, dc.w, dc.h);

		ofSetColor(dd.dummy_color);
		draw_string(dd, title(), dc.x, dc.y);
	}

	if (dd.drawSliderMode) {
		if (dc.selected) {
			if (dd.enabled) ofSetColor(150, 150, 0, dd.alpha_slider);
			else ofSetColor(0, 120, 120, dd.alpha_slider);
			//}
			//else ofSetColor(100, alpha_slider);
			ofNoFill();
			ofDrawRectangle(dc.x0, dc.y0, dc.w, dc.h);
		}
	}
}

//------------------------------------------------------------------------
