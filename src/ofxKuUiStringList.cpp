#include "ofxKuUiStringList.h"

//------------------------------------------------------------------------
void KuUiStringList::draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) {
	drawSlider(dd, dc, false, true);


	const float sliderTopPerc = 0.8f;

	float sliderX = dc.x0;
	float sliderY = dc.y0 + dc.h * sliderTopPerc;
	float sliderW = dc.w;
	float sliderH = dc.h;

	float textYTitle = dc.y - dc.h * 0.1f;
	float textYValue = textYTitle + dc.h * 0.95f;

	// Store for mouse response
	//setInteractiveRect(sliderX, sliderY, sliderW, sliderH);

	int n = titles.size();
	const float indY = 0.2f;
	float Y = sliderY + sliderH + indY * sliderH;

	// Background
	ofFill();
	ofSetColor(0.1f * 255, dd.alpha_slider);
	ofDrawRectangle(sliderX, Y, sliderW, n * sliderH);


/*	for (int i = 0; i < titles.size(); i++) {

		// Rectangle contour
		if (dd.drawSliderMode) {
			if (dc.selected) {
				if (dd.enabled) ofSetColor(200, 200, 0, dd.alpha_slider);
				else ofSetColor(0, 200, 200, dd.alpha_slider);
			}
			else ofSetColor(128, dd.alpha_slider);
			ofNoFill();
			ofDrawRectangle(sliderX, sliderY, sliderW, sliderH);
		}

		// Title and value text
		ofSetColor(color.r, color.g, color.b, color.a * dd.alpha_text_f);
		draw_string(dd, title(), dc.x, textYTitle, fontIndex_);
		draw_string(dd, value(), dc.x, textYValue, KuUiFontIndex::Normal);
	}*/
	
}

//------------------------------------------------------------------------
// Can return true if need to be in model mode, and do preparations - for stringlist
bool KuUiStringList::start_modal_on_click() {
	if (is_modal_) {
		return false;
	}
	is_modal_ = true;
	return true;
}

//------------------------------------------------------------------------
bool KuUiStringList::is_modal() {
	return is_modal_;
}

//------------------------------------------------------------------------
void KuUiStringList::modalKeyPressed(int key)  {

}

//------------------------------------------------------------------------
void KuUiStringList::modalMousePressed(int x, int y, int button) {

}

//------------------------------------------------------------------------
