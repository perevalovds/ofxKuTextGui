#pragma once

// Textual GUI
#include "ofMain.h"
#include <cstdarg>
#include "ofxKuUiComponent.h"

struct ofxKuTextGui {
	ofxKuTextGui();

	void clear();
	void loadFromFile(const string &fileName);  //if fileName=="", then do nothing
	void saveToFile(const string &fileName);

	bool loadFromJSON(const string &s);
	string saveToJSON();

	bool wasModified();		// Currently slow, because comparing JSON serializations

	//Generate names and values separated by separator: x;10;y;20...
	string saveToLine(const string &separator);
	bool loadFromLine(const string &s, const string &separator);


    bool keyPressed(int key);       //generic keyPressed handler
	void update();					//for buttons processing
    void draw(float X, float Y, bool enabled=true, int alpha_text=255, int alpha_slider=255);	//generic draw
    float draw_tabW;	//Distance between tabs
    float draw_yStep;	//Distance between lines
	void set_tab_w(float w, float indentx = 10, float cell_dx = -10.5);
	void set_tab_h(float h, float indenty = 2, float cell_dy = -14.5);

	void set_dummy_color(const ofColor &color) { dd.dummy_color = color; }
	void set_background_color(const ofColor &color) { dd.background_color = color; }

	//using custom font - if not, using ofDrawBitmapString
	void set_font(ofTrueTypeFont *font, float shift_x, float shift_y);
	void set_fonts(ofTrueTypeFont* font, ofTrueTypeFont* fontBold, ofTrueTypeFont* fontItalics, float shift_x, float shift_y);


	//Enable(default) or disable editing string values from keyboard
	//This disabling feature t may be useful, because currently editing strings blocks main app loop in oF,
	//and for remote controlled GUI it's undesirable to be blocked
	void set_editing_strings(bool v);	//true by default

	void set_mouse_enabled(bool v);	//enable/disable mouse control, true by default
	void set_mouse_step(int step);	//step for changing values

    bool mousePressed(int x, int y, int button);
    void mouseDragged(int x, int y, int button);
	bool is_mouse_dragging() { return mouse_dragging_; }
	void mouseReleased();

    string toString();
    void setFromString(const string &s);
    
    string drawToString();  //keeps current page, tabs, selected value
    void drawFromString(const string &s, float x, float y); //gui can be empty, just use cellW, cellH params

	int addPage(const string &pageName);    //returns page index
	void addTab(int n = 1);
    
    void setDrawSliderMode(bool value); //should we draw slider
	void set_var_color(const string &var_name, const ofColor &color);	//sets variable color through all pages
	void set_var_visibility(const string& var_name, bool visible);
	void set_var_editable(const string& var_name, bool editable);
	void set_var_mark(const string& var_name, bool mark);	// marking elements, for example, used in presets
	void set_pagelist_visibility(bool visible);	// allows to hide pages list
    
	//adding to current page/tab
	//if &var = NULL, then GUI parameter is linked to its own value - useful for dynamic GUI creation
	KuUiComponent *addFloat(string name, float &var, float defV, float minV, float maxV,
		int numStepsSlow, int numStepsFast);
	KuUiComponent *addInt(string name, int &var, int defV, int minV, int maxV,
		int stepSlow, int stepFast);
	KuUiComponent *addString(string name, string &var, const string &defV);
    KuUiComponent *addStringList(string name, int &var, int defV, const vector<string> &title);
    KuUiComponent *addStringList(string name, int &var, int defV, int count...);
	KuUiComponent *addButton(string name, int &var);
	KuUiComponent* addCheckbox(string name, int& var);
	void addDummy(string title = "", int n=1);
    

    
	KuUiComponent *addVar(string name);	//adding existing var
    KuUiComponent *findVar(const string &name);   //one var
    KuUiComponent *findVarChecking(const string &name);   //one var, exits if no found
    float *findVarFloat(const string &name);
    int *findVarInt(const string &name);
    int *findVarStringList(const string &name);
	string *findVarString(const string &name);
	int *findVarButton(const string &name);
	void setToggled(const string& name, int t);		// Call this to toggle buttons (with duplicates)

	//short access to vars
	float &float_(const string &name);
	int &int_(const string &name);
	int &stringlist_(const string &name);
	string &string_(const string &name);
	int &button_(const string &name);

	//Smoothing vars values
	//Applicable to float and int vars, automatically enables drawing smoothed value
	//Currently it's not in an development stage (not matured), so access by smoothed values is by their string names
	float updateSmoothedValue(const string name, float dt, float time_whole_change);
	float getSmoothedValue(const string name);

    vector<KuUiComponent*> findVars(const string &name);   //all instances

	bool setValue(const string &name, const string &value);

	//edit
    void gotoPrevPage();
    void gotoNextPage();
	void gotoPrevTab();
	void gotoNextTab();
	void gotoPrevValue();
	void gotoNextValue();
	void decreaseValue(int speed, int value=1);	//0-slow,1-fast
	void increaseValue(int speed, int value=1);	//0-slow,1-fast
	void editStringValue();

    void setTab(int index);     //select tab
    void setValue(int index);   //select value
    
	void setPage( const string &name ); //switch to page
    void setPage( int index ); //switch to page
    int pageIndex();
    int tabIndex();
    int varIndex();
    string pageTitle();
    bool validPage();   //is current page number valid
    KuUiPage *currentPagePointer();    

	vector<KuUiComponent *> getVars();    //without repetitions
    vector<KuUiComponent *> getPageVars();
    
    //-------------------------------------------------------------------


protected:
	void storeState();		// for "wasModified"
	string stateJson_;

	const string PageVarName = "Page";

	vector<KuUiPage> page_;
	int selPage;	//selected page
    vector<string> pageTitles();

	typedef map<string, KuUiComponent *> StringVarMap;
	StringVarMap vars_;	//index of vars - sorted, for saving to file

	typedef unordered_map<string, vector<KuUiComponent *> > StringVarHash;
	StringVarHash hash_vars_;	//index of vars - unsorted, multiple, for search and toggle buttons

	KuUiComponent *addVar(KuUiComponent* var);

	void rebuildVars();
	bool needRebuild_;
    float cellW, cellH, cellDx, cellDy;
    
	KuUiDrawData dd;

	bool mouse_enabled_;    
	bool editing_strings_;

	float drawn_x_;
	float drawn_y_;
	ofRectangle drawn_cell_rect(int tab, int i);
	void get_cell_by_coords(float x, float y, int &tab, int &i);


    bool mouse_dragging_;
    int mouse_x_,mouse_y_;
	int mouse_step_;
	void mouse_reset();

	// Modal mode for stringlist
	KuUiComponent* modalComponent_ = nullptr;
	void startModalMode(KuUiComponent* var);
	void stopModalMode();

};

