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
	//__debugbreak();
	ofSleepMillis(2000);
	OF_EXIT_APP(0);
}

//------------------------------------------------------------------------
void KuUiComponent::buildTitle() {
	title_ = name_;
	ofStringReplace(title_, "_", " ");

	// Change font
	/*
	if (!title_.empty()) {
		if (title_[0] == '*') {	// Const value
			fontIndex_ = KuUiFontIndex::Bold;
			title_ = title_.substr(1);
		}
		else if (title_[0] == '-') {	// Output value
			fontIndex_ = KuUiFontIndex::Italics;
			title_ = title_.substr(1);
		}
	}*/
}

//------------------------------------------------------------------------
void KuUiComponent::setInteractiveRect(float x, float y, float w, float h) {
	interactiveRect_ = ofRectangle(x, y, w, h);
}

//------------------------------------------------------------------------
bool KuUiComponent::mouseInside(const glm::vec2& pos) {
	return interactiveRect_.inside(pos);
}

//------------------------------------------------------------------------
void KuUiComponent::setupSmoothValue() {
	smoothed_value_normalized_ = valueNormalized();
	smoothed_value_ = floatValue();
}

//------------------------------------------------------------------------
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

//------------------------------------------------------------------------
float KuUiComponent::getSmoothedValue() {
	return smoothed_value_;
}

//------------------------------------------------------------------------
void KuUiComponent::setDrawSmoothed(bool v) {
	draw_smoothed_value_ = v;
}

//------------------------------------------------------------------------
void KuUiComponent::draw_string(
	const KuUiDrawData& dd, const string& s, float x, float y,
	KuUiFontIndex fontIndex) {
	ofTrueTypeFont* font = dd.fonts[int(fontIndex)];
	if (font) {
		font->drawString(s, x + dd.font_shift_x, y + dd.font_shift_y);
	}
	else {
		ofDrawBitmapString(s, x, y);
	}
}

//------------------------------------------------------------------------
void KuUiComponent::draw_string_centered(
	const KuUiDrawData& dd, const KuUiDrawComponentData& dc,
	const string& s, float x, float y, float w,
	KuUiFontIndex fontIndex) {
	ofTrueTypeFont* font = dd.fonts[int(fontIndex)];
	if (font) {
		float shiftX = (w - font->getStringBoundingBox(s, 0, 0).getWidth()) / 2.f;
		font->drawString(s, x + shiftX, y + dd.font_shift_y);
	}
	else {
		float shiftX = (w - s.length() * 8) / 2.f;	// As I see, system font char width is 8 pixels
		ofDrawBitmapString(s, x + shiftX, y);
	}
}

//------------------------------------------------------------------------
glm::vec2 KuUiComponent::mousePosition() {
	return glm::vec2(ofGetMouseX(), ofGetMouseY());
}

//------------------------------------------------------------------------
void KuUiComponent::drawSlider(const KuUiDrawData& dd, const KuUiDrawComponentData& dc,
	bool drawValuePosition, bool drawStringlistTriangle)
{
	const float sliderTopPerc = 0.8f;

	float sliderX = dc.x0;
	float sliderY = dc.y0 + dc.h * sliderTopPerc;
	float sliderW = dc.w;
	float sliderH = dc.h;

	float textYTitle = dc.y - dc.h * 0.1f;
	float textYValue = textYTitle + dc.h * 0.95f;

	// Store for mouse response
	setInteractiveRect(sliderX, sliderY, sliderW, sliderH);

	// Background
	ofFill();
	ofSetColor(0.1f * 255, dd.alpha_slider);
	ofDrawRectangle(sliderX, sliderY, sliderW, sliderH);

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

	// Mark
	if (marked) {
		const float MarkSize = 6;
		ofFill();
		ofDrawTriangle(sliderX, sliderY, sliderX, sliderY + MarkSize, sliderX + MarkSize, sliderY);
	}

	if (drawValuePosition) {
		//Slider and smoothed value
		if (dd.drawSliderMode) {
			ofFill();
			ofSetColor(255, 60.0 / 255.0 * dd.alpha_slider);
			ofDrawRectangle(sliderX, sliderY, sliderW * valueNormalized(), dc.h);
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
			float bottomY = sliderY + sliderH - 0.5;
			// Bottom line
			ofDrawLine(sliderX, bottomY, sliderX + val_pix, bottomY);
			// Right line
			ofDrawLine(sliderX + val_pix, sliderY, sliderX + val_pix, bottomY);

			ofSetLineWidth(1);

			//Almost Bottom line - currently only for smoothed values
			if (draw_smoothed_value_) {	//comment to draw bottom line always
				float bottom_val_pix = (draw_smoothed_value_) ? sliderW * smoothed_value_normalized_ : val_pix;
				ofFill();
				const int h1 = 3; //PARAM
				ofDrawRectangle(sliderX, bottomY - h1, bottom_val_pix, h1);
			}
		}
	}

	if (drawStringlistTriangle) {
		float w = sliderH - 2;
		float h = sliderH;
		float x = sliderX + sliderW - w;
		float triX = x + w / 2;
		float triY = sliderY + h / 2;
		float triRx = 4;
		float triRy1 = 3;
		float triRy2 = 4;

		ofSetColor(128, dd.alpha_slider);
		ofDrawLine(x, sliderY, x, sliderY + sliderH);
		ofFill();
		ofDrawTriangle(triX, triY + triRy2,
			triX - triRx, triY - triRy1,
			triX + triRx, triY - triRy1);
	}
}

//------------------------------------------------------------------------
