#pragma once

#include "ofxKuUiComponent.h"

class KuUiDummy : public KuUiComponent {
public:
	KuUiDummy() {}
	// Introduced uniqueName in order to control conditional visibility
	KuUiDummy(const string& nameForTitle, const string& uniqueName = "") {
		type = KuUiType::VDummy;
		name_ = nameForTitle;
		buildTitle();
		if (!uniqueName.empty()) {
			name_ = uniqueName;	// Dummy will have unique name
		}
	}

	void draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) override;

};
