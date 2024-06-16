#include "ofxKuUiDummy.h"

//------------------------------------------------------------------------
void KuUiDummy::draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) {
	const float dummyTopPerc = 0.25f;
	const float dummyBottomPerc = 1.8f;
	const float dummyTextShiftPerc = 0.58f;

	// Text
	if (!title().empty()) {
		float textYTitle = dc.y + dc.h * dummyTextShiftPerc;
		ofSetColor(dd.dummy_color);
		draw_string_centered(dd, dc, title(), dc.x0, textYTitle, dc.w, fontIndex_);
	}

	// Selection
	if (dd.drawSliderMode) {
		if (dc.selected) {
			float dummyY = dc.y0 + dc.h * dummyTopPerc;
			float dummyH = dc.h * (dummyBottomPerc - dummyTopPerc);

			if (dd.enabled) ofSetColor(150, 150, 0, dd.alpha_slider);
			else ofSetColor(0, 120, 120, dd.alpha_slider);

			ofNoFill();
			ofDrawRectangle(dc.x0, dummyY, 1, dummyH);
		}
	}
}

//------------------------------------------------------------------------
