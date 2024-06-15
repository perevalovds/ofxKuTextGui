#include "ofxKuUiStringList.h"

//------------------------------------------------------------------------
void KuUiStringList::draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) {
	drawSlider(dd, dc, false, true);

	// Modal
	if (is_modal_) {
		setupModal(dd, dc);

		// Background
		ofFill();
		ofSetColor(0.1f * 255, dd.alpha_slider);
		ofDrawRectangle(modalRectBackground_);

		// Current item
		{
			ofFill();
			ofSetColor(40, dd.alpha_slider);
			ofDrawRectangle(modalRect_.x, modalRect_.y + *var * modalItemHeight_, modalRect_.width, modalItemHeight_);
		}

		// Selected item
		int selected = getModalSelectedIndex(mousePosition());
		if (selected >= 0 && selected < titles.size()) {
			ofFill();
			ofSetColor(100, dd.alpha_slider);
			ofDrawRectangle(modalRect_.x, modalRect_.y + selected * modalItemHeight_, modalRect_.width, modalItemHeight_);
		}

		// Values text
		ofSetColor(255, dd.alpha_slider);
		for (int i = 0; i < titles.size(); i++) {
			draw_string(dd, titles[i], dc.x, modalTextYValue_ + i * modalItemHeight_, KuUiFontIndex::Normal);
		}


		// Final contour rect
		ofNoFill();
		ofSetColor(255, dd.alpha_slider);
		ofDrawRectangle(modalRect_);

	}
}

//------------------------------------------------------------------------
int KuUiStringList::getModalSelectedIndex(const glm::vec2& mousePos) {
	if (titles.empty()) {
		return -1;
	}
	if (modalRect_.inside(mousePos)) {
		return (mousePos.y - modalRect_.y) / modalItemHeight_;
	}
	return -1;
}

//------------------------------------------------------------------------
void KuUiStringList::setupModal(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) {
	if (!modal_inited_) {
		modal_inited_ = true;
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

		modalTextYValue_ = dc.y - dc.h * 0.1f + sliderH + indY * sliderH + dc.h * 0.95f;

		modalRect_ = ofRectangle(sliderX, Y, sliderW, H);
		modalRectBackground_ = ofRectangle(sliderX - border, Y, sliderW + 2 * border, H + border);
		modalItemHeight_ = sliderH;
	}
}

//------------------------------------------------------------------------
// Can return true if need to be in model mode, and do preparations - for stringlist
bool KuUiStringList::start_modal_on_click() {
	if (is_modal_) {
		return false;
	}
	is_modal_ = true;
	modal_inited_ = false;

	return true;
}

//------------------------------------------------------------------------
void KuUiStringList::stopModal() {
	is_modal_ = false;
}

//------------------------------------------------------------------------
bool KuUiStringList::is_modal() {
	return is_modal_;
}

//------------------------------------------------------------------------
void KuUiStringList::modalKeyPressed(int key)  {
	stopModal();
}

//------------------------------------------------------------------------
void KuUiStringList::modalMousePressed(int x, int y, int button) {
	int selected = getModalSelectedIndex(mousePosition());
	if (selected >= 0 && selected < titles.size()) {
		setValueInt(selected);
	}
	stopModal();
}

//------------------------------------------------------------------------
