#pragma once

#include "ofxKuUiComponent.h"

class KuUiDummy : public KuUiComponent {
public:
	KuUiDummy() {}
	KuUiDummy(const string& name0) {
		type = KuUiType::VDummy;
		name_ = name0;
		title_ = name0;
		ofStringReplace(title_, "_", " ");
	}

	void draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) override;

};
