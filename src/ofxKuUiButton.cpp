#include "ofxKuUiButton.h"

//------------------------------------------------------------------------
void KuUiButton::draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) {
	const int button_ind = 8;
	const float button_round = 10;	//button rounding of corners

	float textYTitle = dc.y + dc.h * 0.5f;
	float buttonY = dc.y0 + dc.h * 0.25f;
	float buttonW = dc.w - 2 * button_ind;
	float buttonH = dc.h * 1.4f;

	bool toggled = is_toggled();
	bool checkbox = is_checkbox();
	// Background
	if (!checkbox) {
		float a = ofLerp(toggled ? 0.5 : 0.3, 1, button_alpha_);	//NOTE: Parameters for button's background
		if (a > 0) {
			ofSetColor(180 * a, dd.alpha_slider);
			ofFill();
			ofDrawRectRounded(dc.x0 + button_ind, buttonY, dc.w - 2 * button_ind, buttonH, button_round);
		}
	}

	// Contour
	if (dd.drawSliderMode) {
		if (dc.selected) {
			int c = toggled ? 255 : 220;
			//if (enabled) ofSetColor(c, c, c, alpha_slider);
		//	else 
			ofSetColor(c, c, 0, dd.alpha_slider);
		}
		else {
			int c = toggled ? 255 : 160;
			ofSetColor(c, dd.alpha_slider);
		}
		ofNoFill();
		if (!checkbox) {
			ofSetLineWidth(toggled ? 3 : 2);
		}
		ofDrawRectRounded(dc.x0 + button_ind, buttonY, buttonW, buttonH, button_round);
		if (!checkbox) {
			ofSetLineWidth(1);
		}

		// Checkbox 'V' inside square
		if (checkbox) {
			float h1 = dc.h - 6;
			float x1 = dc.x0 + dc.w - 2 * button_ind - h1 - 1;
			float y1 = buttonY + (buttonH - dc.h) / 2 + 3;

			if (intValue()) {
				// 'V'
				ofSetColor(color.r, color.g, color.b, color.a * dd.alpha_text_f);
				ofDrawLine(x1 + 4, y1 + 6, x1 + h1 / 2, y1 + h1 - 3);
				ofDrawLine(x1 + h1 / 2, y1 + h1 - 3, x1 + h1 - 3, y1 + 3);
			}

			ofDrawRectangle(x1, y1, h1, h1);
		}


	}

	// Text
	ofSetColor(color.r, color.g, color.b, color.a * dd.alpha_text_f);
	if (checkbox) {
		draw_string(dd, " " + title(), dc.x, textYTitle);
	}
	else {
		draw_string_centered(dd, dc, title(), dc.x, textYTitle, buttonW);
	}

	// Mark
	if (marked) {
		const float MarkShiftX = 11;
		const float MarkShiftY = 1;
		const float MarkSize = 6;
		float x1 = dc.x0 + MarkShiftX;
		float y1 = buttonY + MarkShiftY;
		ofFill();
		ofDrawTriangle(x1, y1, x1, y1 + MarkSize, x1 + MarkSize, y1);
	}
}

//------------------------------------------------------------------------
