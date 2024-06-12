#pragma once

#include "ofxKuUiComponent.h"


class KuUiString: public KuUiComponent {
public:
	shared_ptr<string> autovar;
	string* var = nullptr;
	string def;

	KuUiString() {}
	KuUiString(const string& name0, string& var0, const string& defV) {
		type = KuUiType::VString;
		name_ = name0;
		title_ = name0;
		ofStringReplace(title_, "_", " ");

		if (&var0) var = &var0;
		else {
			autovar = shared_ptr<string>(new string);
			var = autovar.get();
		}

		def = defV;
		setValue(defV);
	}

	float cellHeight() override { return 1.75f; }
	void draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) override;

	void setValue(const string& v) override {
		*var = v;
	}

	string value() override {
		return *var;
	}

	void inc(int dir, int speed) override { //speed=0,1
		editStringValue();
	}
	void reset() override {
		setValue(def);
	}

	void editStringValue() {
		*var = ofSystemTextBoxDialog(name_, *var);
	}
	bool is_string() override {
		return true;
	}
};

