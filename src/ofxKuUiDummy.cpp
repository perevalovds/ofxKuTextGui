#include "ofxKuUiDummy.h"

//------------------------------------------------------------------------
void KuUiDummy::draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) {
	// Text
	if (!title().empty()) {
		//back
		ofFill();
		ofSetColor(dd.dummy_back.r, dd.dummy_back.g, dd.dummy_back.b, dd.dummy_back.a * dd.alpha_slider);
		ofDrawRectangle(dc.x0, dc.y0, dc.w, dc.h);

		const float dummyTextY = dc.y + dc.h * 0.4f;
		ofSetColor(dd.dummy_color);
		draw_string_centered(dd, dc, title(), dc.x0, dummyTextY, dc.w, fontIndex_);
	}

	// Selection
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
