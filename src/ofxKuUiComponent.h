#pragma once

// Base UI Component Class
#include "ofMain.h"
#include <cstdarg>

using namespace std;

void KuUiExitWithMessage(const string& message);

enum class KuUiType : int {
	VUndefined = -1,
	VFloat = 0,
	VInt = 1,
	VStringList = 2,
	VString = 3,
	VButton = 4,
	VDummy = 5
};


// KuUiVisibilityConditionStr is a string-valued version of KuUiVisibilityCondition,
// used for intermediate generation
struct KuUiVisibilityConditionStr {
	string valueName;
	vector<string> values;

	// Serialize to C++ string:
	// { {valueName, {value1, value2, ...}}, {valueName, {value1, value2,...}} }
	// "" if empty
	static string KuUiVisibilityConditionStr::toStr(const vector<KuUiVisibilityConditionStr>& conds) {
		if (conds.empty()) {
			return "";
		}
		stringstream ss;
		ss << "{ ";
		for (int i = 0; i < conds.size(); i++) {
			if (i > 0) {
				ss << ", ";
			}
			ss << "{\"" << conds[i].valueName
				<< "\", {\"" << ofJoinString(conds[i].values, "\",\"") << "\"}}";
		}
		ss << " }";
		return ss.str();
	}
};

// Single item of component's visibility
struct KuUiVisibilityCondition {
	int* valuePtr = nullptr;
	vector<int> values;
	KuUiVisibilityConditionStr str;	// Stringed version is used for debugging purposes
};


enum class KuUiFontIndex : int {
	Normal = 0,
	Bold = 1,
	Italics = 2
};

// General draw parameters
struct KuUiDrawData {
	bool drawSliderMode = true;

	bool enabled = true;
	int alpha_text = 255;
	int alpha_slider = 255;

	float alpha_text_f = 0;

	ofColor background_color = ofColor(0, 200);
	ofColor dummy_color = ofColor(127);

	vector<ofTrueTypeFont*> fonts = { nullptr, nullptr, nullptr };
	float font_shift_x = 0;
	float font_shift_y = 0;
};

// Parameters for drawing particular element
struct KuUiDrawComponentData {
	bool selected = false;

	float w = 0;
	float h = 0;

	float x = 0;	// fot font draw
	float y = 0;
	float x0 = 0;	// for rectangle draw
	float y0 = 0;
};

class KuUiComponent {
public:
	// Static members
	static void draw_string(const KuUiDrawData& dd, const string& s, float x, float y, KuUiFontIndex fontIndex);
	static void draw_string_centered(const KuUiDrawData& dd, const KuUiDrawComponentData& dc,
		const string& s, float x, float y, float w, KuUiFontIndex fontIndex);
	static glm::vec2 mousePosition();	// used for stringlist hover

	static float cellHeight() { return 1.6f; }
public:

	string name_;
	string title_;

	KuUiType type = KuUiType::VUndefined;
	bool isVisible() const;	// Can be time consuming. Is computed from 'visible_' and 'visibilityConditions_'
	void setVisibility(bool v);
	void setVisibilityConditions(const vector<KuUiVisibilityCondition>& conds);

	bool editable = true;	// Can user edit it
	bool marked = false;	// Special mark, for example, to show the variable controlled from presets


	virtual void draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) {}
	bool mouseInside(const glm::vec2& pos);

	bool is_editable() const { return isVisible() && editable; }

	//Note: dummy's title stored in vstring.title !!!!
	//So, when we will optimize Vars - check this

	bool need_save() { //we save only not-dummy, not-button, not-output vars (name doesn't start with "-")
		return (type != KuUiType::VDummy && !is_button() && (title().empty() || title()[0] != '-'));
	}

	virtual bool is_string() { return false; }
	virtual bool is_button() { return false; }
	virtual bool is_checkbox() { return false; }
	virtual bool is_toggled() { return false; }


	// Mouse click for button and string
	virtual bool acceptMouseClick() { return false;  }
	virtual void mouseClick(int x, int y, int button) {}
	
	// Modal mode for stringlist
	// Can return true if need to be in model mode, and do preparations
	virtual bool start_modal_on_click() { return false; }
	virtual bool is_modal() { return false; }
	virtual void modalKeyPressed(int key) {}
	virtual void modalMousePressed(int x, int y, int button) {}


	ofColor color;
	void setColor(const ofColor& color0) {
		color = color0;
	}

	virtual void update_button() {}

	void setTitle(const string& title) { title_ = title; }
	virtual void setToggled(int t) {}

	virtual float* floatVarPtr() { return nullptr; }
	virtual int* intVarPtr() { return nullptr; }
	virtual string* stringVarPtr() { return nullptr; }
	virtual float floatDef() { return 0.f; }
	virtual int intDef() { return 0; }
	virtual const string& stringDef() { return ""; }


	virtual void setValueInt(int v) {}
	virtual void setValueFloat(float v) {}
	virtual void setValue(const string& v) {}
	virtual string value() { return ""; }
	virtual int intValue() { return 0; }
	virtual float floatValue() { return 0.f; }
	virtual float valueNormalized() { return 0.f; }
	virtual float normalizedToValue(float normalizedValue) { return 0.f; }

	virtual void inc(int dir, int speed) {}
	virtual void editStringValue() {}
	string name() { return name_; }
	string title() { return title_; }
	virtual void reset() {}

	virtual void setTitles(vector<string>& titles) {};

	//Smoothing values 
	//applicable to float and int vars
	float smoothed_value_normalized_ = 0;
	float smoothed_value_ = 0;
	bool draw_smoothed_value_ = false;

	void setupSmoothValue();
	//smoothing function
	void updateSmoothedValue(float dt, float time_whole_change);
	//get smoothed value
	float getSmoothedValue();
	//should we draw smoothed value
	void setDrawSmoothed(bool v);

protected:
	bool visible_ = true;	
	vector<KuUiVisibilityCondition> visibilityConditions_;

	void buildTitle();
	KuUiFontIndex fontIndex_ = KuUiFontIndex::Normal;
	void drawSlider(const KuUiDrawData& dd, const KuUiDrawComponentData& dc, 
		bool drawValuePosition, bool drawStringlistTriangle);
	
	// Setup area for mouse - after a draw
	void setInteractiveRect(float x, float y, float w, float h);
	ofRectangle interactiveRect_ = ofRectangle(-1, -1, -1, -1);
};

//------------------------------------------------------------------------
struct KuUiTab {
	vector<KuUiComponent*> var;
	int selVar;
	KuUiTab() {
		selVar = 0;
	}
	bool validVar() { return selVar >= 0 && selVar < var.size(); }
};

struct KuUiPage {
	string name;
	string title;
	vector<KuUiTab> tab;
	int selTab;

	KuUiPage() {
		selTab = 0;
	}
	void setup(const string& name0) {
		name = name0;
		buildTitle();
	}

	void addTab() {
		tab.push_back(KuUiTab());
	}
	KuUiComponent* addVar(KuUiComponent* var) {
		if (tab.empty()) addTab();
		vector<KuUiComponent*>& vars = tab[tab.size() - 1].var;
		vars.push_back(var);
		return vars[vars.size() - 1];
	}
	bool validTab() { return selTab >= 0 && selTab < tab.size(); }

private:
	void buildTitle() {
		title = name;
		ofStringReplace(title, "_", " ");
	}

};
