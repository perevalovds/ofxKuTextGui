#include "ofxKuUiComponent.h"

#include "ofxKuUiDummy.h"
#include "ofxKuUiFloat.h"
#include "ofxKuUiInt.h"
#include "ofxKuUiString.h"
#include "ofxKuUiStringList.h"

//------------------------------------------------------------------------
void KuUiExitWithMessage(const string& message) {
	cout << "ofxKuTextGui causes exiting now," << endl;
	cout << "    Reason: " << message << endl;
	ofSetFullscreen(false);
	ofSystemAlertDialog("ofxKuTextGui causes exiting now, reason:\n" + message);
	__debugbreak();
	ofSleepMillis(2000);
	OF_EXIT_APP(0);
}

//------------------------------------------------------------------------
void KuUiComponent::setupSmoothValue() {
	smoothed_value_normalized_ = valueNormalized();
	smoothed_value_ = floatValue();
}

void KuUiComponent::updateSmoothedValue(float dt, float time_whole_change) {
	float v = valueNormalized();
	if (time_whole_change <= 0.00001) {
		//momentary change
		smoothed_value_normalized_ = v;
	}
	else {
		//smooth change
		float limit = dt / time_whole_change;
		smoothed_value_normalized_ += ofClamp(v - smoothed_value_normalized_, -limit, limit);
	}
	smoothed_value_ = normalizedToValue(smoothed_value_normalized_);
}

float KuUiComponent::getSmoothedValue() {
	return smoothed_value_;
}

void KuUiComponent::setDrawSmoothed(bool v) {
	draw_smoothed_value_ = v;
}

void KuUiComponent::draw_string(const KuUiDrawData& dd, const string& s, float x, float y) {
	if (dd.custom_font) {
		dd.custom_font->drawString(s, x + dd.font_shift_x, y + dd.font_shift_y);
	}
	else {
		ofDrawBitmapString(s, x, y);
	}
}

//------------------------------------------------------------------------
void KuUiComponent::drawSlider(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) {
	//non-button

	ofFill();
	ofSetColor(0, dd.alpha_slider);
	ofDrawRectangle(dc.x0, dc.y0, dc.w, dc.h);

	if (dd.drawSliderMode) {
		if (dc.selected) {
			if (dd.enabled) ofSetColor(200, 200, 0, dd.alpha_slider);
			else ofSetColor(0, 200, 200, dd.alpha_slider);
		}
		else ofSetColor(128, dd.alpha_slider);
		ofNoFill();
		ofDrawRectangle(dc.x0, dc.y0, dc.w, dc.h);
	}


	// Name and value text
	ofSetColor(color.r, color.g, color.b, color.a * dd.alpha_text_f);
	draw_string(dd, " " + title() + " " + value(), dc.x, dc.y);

	// Mark
	if (marked) {
		const float MarkSize = 6;
		ofFill();
		ofDrawTriangle(dc.x0, dc.y0, dc.x0, dc.y0 + MarkSize, dc.x0 + MarkSize, dc.y0);
	}

	//Slider and smoothed value
	if (dd.drawSliderMode) {
		ofFill();
		ofSetColor(255, 60.0 / 255.0 * dd.alpha_slider);
		ofDrawRectangle(dc.x0, dc.y0, dc.w * valueNormalized(), dc.h);
		if (dc.selected) {
			if (dd.enabled) ofSetColor(255, 255, 0, dd.alpha_slider);
			else ofSetColor(0, 200, 200, dd.alpha_slider);
		}
		else ofSetColor(200, dd.alpha_slider);
		//Slider
		ofNoFill();
		float val_pix = dc.w * valueNormalized();
		// ofDrawRectangle(x0, y0, val_pix, h);

		ofSetLineWidth(3);
		float bottomY = dc.y0 + dc.h - 0.5;
		// Bottom line
		ofDrawLine(dc.x0, bottomY, dc.x0 + val_pix, bottomY);
		// Right line
		ofDrawLine(dc.x0 + val_pix, dc.y0, dc.x0 + val_pix, bottomY);

		ofSetLineWidth(1);

		//Almost Bottom line - currently only for smoothed values
		if (draw_smoothed_value_) {	//comment to draw bottom line always
			float bottom_val_pix = (draw_smoothed_value_) ? dc.w * smoothed_value_normalized_ : val_pix;
			ofFill();
			const int h1 = 3; //PARAM
			ofDrawRectangle(dc.x0, bottomY - h1, bottom_val_pix, h1);
		}
	}
}

//------------------------------------------------------------------------
void KuUiComponent::drawButtonLike(const KuUiDrawData& dd, const KuUiDrawComponentData& dc)
{
	const int button_ind = 8;
	const float button_round = 10;	//button rounding of corners

	// Background
	bool toggled = is_toggled();
	if (type != KuUiType::VInt) {
		KuUiExitWithMessage("ofxKuTextGui: it's expected to be int: " + title());
	}
	float a = ofLerp(toggled ? 0.5 : 0.3, 1, ((KuUiInt*)this)->button_alpha_);	//NOTE: Parameters for button's background
	if (a > 0) {
		ofSetColor(180 * a, dd.alpha_slider);
		ofFill();
		ofDrawRectRounded(dc.x0 + button_ind, dc.y0, dc.w - 2 * button_ind, dc.h, button_round);
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
		ofDrawRectRounded(dc.x0 + button_ind, dc.y0, dc.w - 2 * button_ind, dc.h, button_round);

		// Checkbox mark and square
		if (is_checkbox()) {
			float h1 = dc.h - 6;
			float x1 = dc.x0 + dc.w - 2 * button_ind - h1 - 1;
			float y1 = dc.y0 + 3;

			if (intValue()) {
				// Mark
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
	draw_string(dd, " " + title(), dc.x, dc.y);

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
