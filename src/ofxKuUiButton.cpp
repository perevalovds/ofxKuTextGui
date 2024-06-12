#include "ofxKuUiButton.h"

//------------------------------------------------------------------------
void KuUiButton::draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) {
	bool checkbox = is_checkbox();
	bool toggled = is_toggled();


	const float buttonTopPerc = 0.25f;
	const float buttonBottomPerc = 1.8f;
	const float buttonTextShiftPerc = 0.58f;

	const float buttonRoundPixels = 10;	//button rounding of corners

	float textYTitle = dc.y + dc.h * buttonTextShiftPerc;
	const float& buttonX = dc.x0;
	float buttonY = dc.y0 + dc.h * buttonTopPerc;
	const float& buttonW = dc.w;
	float buttonH = dc.h * (buttonBottomPerc - buttonTopPerc);

	// Background
	float a;
	if (checkbox) {
		a = ofLerp(0.2f, 1, button_alpha_);	//NOTE: Parameters for checkbox's background
	}
	else {
		a = ofLerp(0.3f, 1, button_alpha_);	//NOTE: Parameters for button's background
	}
	if (a > 0) {
		ofSetColor(180 * a, dd.alpha_slider);
		ofFill();
		ofDrawRectRounded(buttonX, buttonY, buttonW, buttonH, buttonRoundPixels);
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
		ofDrawRectRounded(buttonX, buttonY, buttonW, buttonH, buttonRoundPixels);
		if (!checkbox) {
			ofSetLineWidth(1);
		}

		// Checkbox 'V' inside square
		if (checkbox) {
			float h1 = dc.h - 6;
			float x1 = buttonX + buttonW - h1 - 10;
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
		draw_string(dd, " " + title(), dc.x, textYTitle, fontIndex_);
	}
	else {
		draw_string_centered(dd, dc, title(), dc.x0, textYTitle, buttonW, fontIndex_);
	}

	// Mark
	if (marked) {
		const float MarkShiftX = 3;
		const float MarkShiftY = 1;
		const float MarkSize = 6;
		float x1 = buttonX + MarkShiftX;
		float y1 = buttonY + MarkShiftY;
		ofFill();
		ofDrawTriangle(x1, y1, x1, y1 + MarkSize, x1 + MarkSize, y1);
	}
}

//------------------------------------------------------------------------
