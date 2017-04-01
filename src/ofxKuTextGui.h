#pragma once

//Textual GUI
#include "ofMain.h"
#include <cstdarg>


//Checking oF version
#ifndef ofDrawLine
#define OFXKUTEXTGUI_074
#define ofDrawRectangle ofRect
#define getArgAsInt getArgAsInt32
#endif


struct ofxKuTextGui {
	ofxKuTextGui();

	void loadFromFile(const string &fileName);
	void saveToFile(const string &fileName);
    bool keyPressed(int key);       //generic keyPressed handler
    void draw(float X, float Y, bool enabled=true, int alpha_text=255, int alpha_slider=255);	//generic draw
    float draw_tabW;	//Distance between tabs
    float draw_yStep;	//Distance between lines
    
    void mousePressed(int x, int y, int button);
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    string toString();
    void setFromString(const string &s);
    
    string drawToString();  //keeps current page, tabs, selected value
    void drawFromString(const string &s, float x, float y); //gui can be empty, just use cellW, cellH params

	void addPage(const string &pageName);
	void addTab();
    
    void setDrawSliderMode(bool value); //should we draw slider

	//adding to current page/tab
	void addFloat(string name, float &var, float defV, float minV, float maxV, 
		int numStepsSlow, int numStepsFast);
	void addInt(string name, int &var, int defV, int minV, int maxV,
		int stepSlow, int stepFast);
	void addString(string name, string &var, const string &defV);
    void addStringList(string name, int &var, int defV, const vector<string> &title);
    void addStringList(string name, int &var, int defV, int count...);
    
    
    struct Var;
    
	void addVar(string name);	//adding existing var
    Var *findVar(const string &name);   //one var
    Var *findVarChecking(const string &name);   //one var, exits if no found
    vector<ofxKuTextGui::Var *> findVars(const string &name);   //all instances

	bool setValue(const string &name, const string &value);

	//edit
    void gotoPrevPage();
    void gotoNextPage();
	void gotoPrevTab();
	void gotoNextTab();
	void gotoPrevValue();
	void gotoNextValue();
	void decreaseValue(int speed);	//0-slow,1-fast
	void increaseValue(int speed);	//0-slow,1-fast
	void editStringValue();

    void setTab(int index);
    void setValue(int index);
    
	void setPage( const string &name ); //switch to page
    void setPage( int index ); //switch to page
    int pageIndex();
    string pageTitle();
    bool validPage();   //is current page number valid
    struct Page;
    Page *currentPagePointer();
	
	//-------------------------------------------------------------------
	struct VarFloat {
		string name;
		float *var;
		float minV, maxV;
		float step[2];
		float def;
		VarFloat() { var = 0; step[0]=step[1]=0; }
		VarFloat(string name0, float &var0, float defV, float minV0, float maxV0,
		int numSteps1, int numSteps2) {
			name = name0;
			var = &var0;
			step[0] = (maxV0-minV0)/numSteps1;
			step[1] = (maxV0-minV0)/numSteps2;
			minV = minV0;
			maxV = maxV0;
			def = defV;
			setValue( defV );
		}
		void setValue(float v) {
			*var = min(max(v,minV),maxV);
		}
		void inc(int dir, int speed) { //speed=0,1
			speed = min(max(speed,0),1);
			setValue( *var + dir*step[speed] );
		}		
		void reset() {
			setValue(def);
		}
	};
	
	struct VarInt {
		string name;
		int *var;
		int minV, maxV;
		int step[2];
		int def;
		VarInt() { var = 0; step[0]=step[1]=0; }
		VarInt(string name0, int &var0, int defV, int minV0, int maxV0,
		int step1, int step2) {
			name = name0;
			var = &var0;
			step[0] = step1;
			step[1] = step2;
			minV = minV0;
			maxV = maxV0;
			setValue( defV );
			def = defV;
		}
		void setValue(int v) {
			*var = min(max(v,minV),maxV);
		}
		void inc(int dir, int speed) { //speed=0,1
			speed = min(max(speed,0),1);
			setValue( *var + dir*step[speed] );
		}		
		void reset() {
			setValue(def);
		}
	};
	struct VarString {
		string name;
		string *var;
		string def;
		VarString() {}
		VarString(const string &name0, string &var0, const string &defV) {
			name = name0;
			var = &var0;
			def = defV;
			setValue( defV );
		}
		void setValue(string v) {
			*var = v;
		}
		void inc(int dir, int speed) { //speed=0,1
		}		
		void reset() {
			setValue(def);
		}
		void editStringValue() {
			*var = ofSystemTextBoxDialog(name, *var);
		}
	};
    struct VarStringList {
        string name;
        int *var;
        int minV, maxV;
        int step[2];
        int def;
        vector<string> titles;
        VarStringList() { var = 0; step[0]=step[1]=0; }
        VarStringList(string name0, int &var0, int defV, int minV0, int maxV0,
               int step1, int step2, vector<string> titles0) {
            name = name0;
            var = &var0;
            step[0] = step1;
            step[1] = step2;
            minV = minV0;
            maxV = maxV0;
            setValue( defV );
            def = defV;
            titles = titles0;
        }
        void setValueString(const string &s) {
            if (!s.empty()) {
                if (s[0]>='0' && s[0]<='9') {  //TODO it's so easy check for is-number
                    setValue(ofToInt(s));
                    return;
                }
                for (int i=0; i<titles.size(); i++) {
                    if (titles[i] == s) {
                        setValue(i);
                        return;
                    }
                }
            }
        }
        void setValue(int v) {
            *var = min(max(v,minV),maxV);
        }
        void inc(int dir, int speed) { //speed=0,1
            speed = min(max(speed,0),1);
            setValue( *var + dir*step[speed] );
        }
        void reset() {
            setValue(def);
        }
        string getValue() {
            if (*var >= 0 && *var <= int(titles.size())-1) return titles[*var];
            return "";
        }
    };
	
	struct Var {
		VarFloat vfloat;
		VarInt vint;
		VarString vstring;
        VarStringList vstringlist;
		int index;	//0 - vfloat, 1-vint, 2-string, 3-stringlist
		void setValue( const string &v ) {
			if (index == 0) vfloat.setValue(ofToFloat(v));
			if (index == 1) vint.setValue(ofToInt(v));
			if (index == 2) vstring.setValue(v);
            if (index == 3) vstringlist.setValueString(v);
		}
		void inc(int dir, int speed) {
			if (index == 0) vfloat.inc(dir,speed);
			if (index == 1) vint.inc(dir,speed);
			if (index == 2) vstring.inc(dir,speed);
            if (index == 3) vstringlist.inc(dir,speed);
		}
		void editStringValue() {
			if (index == 2) vstring.editStringValue();
		}
		string value() {
			if (index == 0) return ofToString(*vfloat.var);
			if (index == 1) return ofToString(*vint.var);
			if (index == 2) return *vstring.var;
            if (index == 3) return vstringlist.getValue();
			return "";
		}
        float valueNormalized() {
            if (index == 0) return ofMap(*vfloat.var, vfloat.minV, vfloat.maxV,0,1);
            if (index == 1) return ofMap(*vint.var, vint.minV, vint.maxV,0,1);
            if (index == 3) return ofMap(*vstringlist.var, vstringlist.minV, vstringlist.maxV,0,1);
            
            return 0;
            
        }
        int intValue() {
            if (index == 0) return 0;//int(*vfloat.var);
            if (index == 1) return *vint.var;
            if (index == 2) return 0;
            if (index == 3) return *vstringlist.var;
            return 0;
        }
		string name() {
			if (index == 0) return vfloat.name;
			if (index == 1) return vint.name;
			if (index == 2) return vstring.name;
            if (index == 3) return vstringlist.name;
			return "???";
		}
		void reset() {
			if (index == 0) vfloat.reset();
			if (index == 1) vint.reset();
			if (index == 2) vstring.reset();
            if (index == 3) vstringlist.reset();
		}
        void setTitles(vector<string> &titles) {
            if (index == 3) vstringlist.titles = titles;
        }
	};

	struct Tab {
		vector<Var> var;
		int selVar;
		Tab() {
			selVar = 0;
		}
		bool validVar() { return selVar >= 0 && selVar < var.size(); }
	};

	struct Page {
		string name;
		vector<Tab> tab;
		int selTab;

		Page() {
			selTab = 0;
		}
		void addTab() {
			tab.push_back(Tab());
		}
		Var *addVar( Var &var ) {
			if ( tab.empty() ) addTab();
			vector<Var> &vars = tab[tab.size()-1].var;
			vars.push_back(var);
			return &vars[vars.size()-1];
		}
		bool validTab() { return selTab >= 0 && selTab < tab.size(); }
	};

	vector<Var *> getVars();    //without repetitions
    vector<Var *> getPageVars();
    
    //-------------------------------------------------------------------


protected:
	vector<Page> page_;
	int selPage;	//selected page
    vector<string> pageTitles();

	typedef map<string, Var *> StringVarMap;
	StringVarMap vars_;	//index of vars

	void addVar(Var &var);

	void rebuildVars();
	bool needRebuild_;
    
    bool drawSliderMode_;
    
    bool mouseDrag_;
    int mouseIndex_;
    float cellW, cellH, cellDx, cellDy;
    
};

