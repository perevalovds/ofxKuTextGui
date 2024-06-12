#include "ofxKuUiButton.h"

//------------------------------------------------------------------------
void KuUiButton::draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) {
	const int button_ind = 8;
	const float button_round = 10;	//button rounding of corners

	float textYTitle = dc.y + dc.h * 0.5f;
	float buttonY = dc.y0 + dc.h * 0.25f;
	float buttonH = dc.h * 1.4f;

	// Background
	bool toggled = is_toggled();
	if (type != KuUiType::VInt) {
		KuUiExitWithMessage("ofxKuTextGui: it's expected to be int: " + title());
	}
	float a = ofLerp(toggled ? 0.5 : 0.3, 1, button_alpha_);	//NOTE: Parameters for button's background
	if (a > 0) {
		ofSetColor(180 * a, dd.alpha_slider);
		ofFill();
		ofDrawRectRounded(dc.x0 + button_ind, buttonY, dc.w - 2 * button_ind, buttonH, button_round);
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
		ofSetLineWidth(toggled ? 3 : 2);
		ofDrawRectRounded(dc.x0 + button_ind, buttonY, dc.w - 2 * button_ind, buttonH, button_round);

		// Checkbox 'V' inside square
		if (is_checkbox()) {
			float h1 = dc.h - 6;
			float x1 = dc.x0 + dc.w - 2 * button_ind - h1 - 1;
			float y1 = dc.y0 + 3;

			if (intValue()) {
				// 'V'
				ofSetColor(color.r, color.g, color.b, color.a * dd.alpha_text_f);
				ofDrawLine(x1 + 4, y1 + 6, x1 + h1 / 2, y1 + h1 - 3);
				ofDrawLine(x1 + h1 / 2, y1 + h1 - 3, x1 + h1 - 3, y1 + 3);
			}

			ofSetLineWidth(1);
			ofDrawRectangle(x1, y1, h1, h1);
		}

		ofSetLineWidth(1);
	}

	// Text
	ofSetColor(color.r, color.g, color.b, color.a * dd.alpha_text_f);
	draw_string(dd, " " + title(), dc.x, textYTitle);

	// Mark
	if (marked) {
		const float MarkShift = 12;
		const float MarkSize = 6;
		float x1 = dc.x0 + MarkShift;
		ofFill();
		ofDrawTriangle(x1, dc.y0, x1, dc.y0 + MarkSize, x1 + MarkSize, dc.y0);
	}
}

//------------------------------------------------------------------------
