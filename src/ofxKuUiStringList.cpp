#include "ofxKuUiStringList.h"

//------------------------------------------------------------------------
void KuUiStringList::draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) {
	drawSlider(dd, dc, false, true);

	// Modal
	if (is_modal_) {

		const float sliderTopPerc = 0.8f;

		float sliderX = dc.x0;
		float sliderY = dc.y0 + dc.h * sliderTopPerc;
		float sliderW = dc.w;
		float sliderH = dc.h;

		// Store for mouse response
		//setInteractiveRect(sliderX, sliderY, sliderW, sliderH);

		int n = titles.size();
		const float indY = 0.2f;
		float Y = sliderY + sliderH + indY * sliderH;
		float H = n * sliderH;
		float border = dc.h * 0.5;

		float textYTitle = dc.y - dc.h * 0.1f + sliderH + indY * sliderH;
		float textYValue = textYTitle + dc.h * 0.95f;

		// Background
		ofFill();
		ofSetColor(0.1f * 255, dd.alpha_slider);
		ofDrawRectangle(sliderX - border, Y, sliderW + 2 * border, H + border);
		ofNoFill();
		ofSetColor(255, dd.alpha_slider);
		ofDrawRectangle(sliderX, Y, sliderW, H);


		// Values text
		ofSetColor(255, dd.alpha_slider);
		for (int i = 0; i < titles.size(); i++) {
			draw_string(dd, titles[i], dc.x, textYValue + i * sliderH, KuUiFontIndex::Normal);
		}
	}
	
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
