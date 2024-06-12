#include "ofxKuUiInt.h"

//------------------------------------------------------------------------
void KuUiInt::draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) {
	if (is_button() || is_checkbox()) {
		drawButtonLike(dd, dc);
	}
	else {
		drawSlider(dd, dc);
	}
}

//------------------------------------------------------------------------
