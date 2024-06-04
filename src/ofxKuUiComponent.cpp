#include "ofxKuUiComponent.h"

#include "json.hpp"

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
//------------------------------------------------------------------------
