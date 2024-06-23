#include "ofxKuTextGui.h"

#include "ofxKuUiButton.h"
#include "ofxKuUiDummy.h"
#include "ofxKuUiFloat.h"
#include "ofxKuUiInt.h"
#include "ofxKuUiString.h"
#include "ofxKuUiStringList.h"

#include "json.hpp"

//------------------------------------------------------------------------
ofxKuTextGui::ofxKuTextGui() {

	set_tab_w(180, 20);
	set_tab_h(35, 10, -18);

	//set_tab_w(140, 10, -10.5);
	//set_tab_h(20, 2, -14.5);

    
	editing_strings_ = true;
	mouse_enabled_ = true;

	drawn_x_ = -10000;
	drawn_y_ = -10000;
	mouse_step_ = 10; //15; //5;
	mouse_dragging_ = false;

	set_font(0, 0, 0);

	clear();
}

//------------------------------------------------------------------------
void ofxKuTextGui::clear() {
	selPage = 0;
	needRebuild_ = true;
	mouse_reset();

	page_.clear();
	vars_.clear();
}


//------------------------------------------------------------------------
void ofxKuTextGui::set_tab_w(float w, float indentx, float cell_dx) {
	cellW = w;
	draw_tabW = cellW + indentx;
	cellDx = cell_dx;
}

//------------------------------------------------------------------------
void ofxKuTextGui::set_tab_h(float h, float indenty, float cell_dy) {
	draw_yStep = h;
	cellH = draw_yStep - indenty;
	cellDy = cell_dy;
}

//------------------------------------------------------------------------
void ofxKuTextGui::set_var_color(const string &var_name, const ofColor &color) {	//sets variable color through all pages
	vector<KuUiComponent *> vars = findVars(var_name);
	for (int i = 0; i<vars.size(); i++) {
		vars[i]->setColor(color);
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::set_var_visibility(const string& var_name, bool visible) {
	vector<KuUiComponent*> vars = findVars(var_name);
	for (int i = 0; i < vars.size(); i++) {
		vars[i]->visible = visible;
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::set_pagelist_visibility(bool visible)
{
	set_var_visibility(PageVarName, visible);
}

//------------------------------------------------------------------------
void ofxKuTextGui::set_var_editable(const string& var_name, bool editable)
{
	vector<KuUiComponent*> vars = findVars(var_name);
	for (int i = 0; i < vars.size(); i++) {
		vars[i]->editable = editable;
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::set_var_mark(const string& var_name, bool mark) {	// marking elements, for example, used in presets
	vector<KuUiComponent*> vars = findVars(var_name);
	for (int i = 0; i < vars.size(); i++) {
		vars[i]->marked = mark;
	}
}

//------------------------------------------------------------------------
vector<KuUiComponent *> ofxKuTextGui::getVars() {
	rebuildVars();
	vector<KuUiComponent *> vars;
	for (StringVarMap::const_iterator iter = vars_.begin(); iter != vars_.end(); iter++) {
		vars.push_back( iter->second );
	}
	return vars;
}

//------------------------------------------------------------------------
vector<KuUiComponent*> ofxKuTextGui::getPageVars() {
    vector<KuUiComponent*> vars;
    KuUiPage *page = currentPagePointer();
    if (page) {
        for (int i=0; i<page->tab.size(); i++) {
            KuUiTab &tab = page->tab[i];
            for (int j=0; j<tab.var.size(); j++) {
                KuUiComponent* var = tab.var[j];
                vars.push_back(var);
            }
        }
    }
    return vars;
}

//------------------------------------------------------------------------
string ofxKuTextGui::toString() {
    string s;
    vector<KuUiComponent *> vars = getVars();
    for (int i=0; i<vars.size(); i++) {
        if (i>0) s += " ";
        s += vars[i]->name() + "=" + vars[i]->value();
    }
    return s;
}

//------------------------------------------------------------------------
void ofxKuTextGui::setFromString(const string &s) {
    vector<KuUiComponent *> vars = getVars();
    vector<string> lines = ofSplitString(s," ");
    for (int k=0; k<lines.size(); k++) {
        vector<string> item = ofSplitString(lines[k], "=", true, true);
        if (item.size() >= 2) {
            string name		= item[0];
            string value	= item[1];
            KuUiComponent *var = findVar(name);
            if (var) {
                var->setValue(value);
            }
        }
    }
}

//------------------------------------------------------------------------
void ofxKuTextGui::loadFromFile(const string &fileName) {
	if (fileName.empty()) {
		return;
	}
	//read lines
	vector<string> lines;
    ifstream f(ofToDataPath(fileName).c_str(),ios::in | ios::binary);
    string line;
    while (getline(f,line)) {
        if (line=="") continue;
        else {
            //remove '\r' for proper reading windows-files in linux
			while ( line[line.length()-1] == '\r' ) {
                line = line.substr( 0, line.length() - 1 );
            }
            lines.push_back( line );
        }
    }
	//parse lines
	vector<KuUiComponent *> vars = getVars();
	for (int k=0; k<lines.size(); k++) {
		vector<string> item = ofSplitString(lines[k], "=", true, true);
		if (item.size() >= 2) {
			string name		= item[0];
			string value	= item[1];
			KuUiComponent *var = findVar(name);
			if (var) {
				var->setValue(value);
			}
		}
	}
	storeState();
}

//------------------------------------------------------------------------
void ofxKuTextGui::saveToFile(const string &fileName) {
	vector<string> file;
	vector<KuUiComponent *> vars = getVars();
	for (int i=0; i<vars.size(); i++) {
		if (vars[i]->need_save()) {
			string line = vars[i]->name() + "=" + vars[i]->value();
			file.push_back(line);
			//cout << "SAVE PARAM " << line << endl;
		}
	}
	ofstream f(ofToDataPath(fileName).c_str(),ios::out);
    for ( size_t i=0; i<file.size(); i++ ) {
        f << file[i] << endl;
    }
    f.close();
	storeState();
}


//------------------------------------------------------------------------
void ofxKuTextGui::storeState() {		// for "wasModified"
	stateJson_ = saveToJSON();
}

//------------------------------------------------------------------------
bool ofxKuTextGui::wasModified() {		// Currently slow, because comparing JSON serializations
	string json = saveToJSON();
	return (stateJson_ != json);
}

//------------------------------------------------------------------------
/*
Saving format:
{
  "-camera"="OFF",
  "FPS"="30",
  "[page]"="Camera",
  "depth_fps"="30",
  "depth_res"="640x480",
  "rgb_fps"="30"
}
*/
string ofxKuTextGui::saveToJSON() {
	vector<string> file;
	file.push_back("{");
	vector<KuUiComponent *> vars = getVars();
	for (int i = 0; i < vars.size(); i++) {
		if (vars[i]->need_save()) {
			string name = vars[i]->name();
			if (!name.empty()) {
				string line = "  \"" + vars[i]->name() + "\"" + ": " + "\"" + vars[i]->value() + "\"";
				file.push_back(line);
			}
		}
	}
	file.push_back("}");
	for (int i = 1; i < int(file.size()) - 2; i++) {
		file[i] += ",";
	}
	return ofJoinString(file, "\n");
}

//------------------------------------------------------------------------
//JSON using documentation: https://github.com/nlohmann/json#examples

bool ofxKuTextGui::loadFromJSON(const string &s) {
	nlohmann::json j = nlohmann::json::parse(s);

	vector<KuUiComponent *> vars = getVars();

	for (auto it = j.begin(); it != j.end(); ++it) {
		//std::cout << it.key() << " : " << it.value() << "\n";
		KuUiComponent *var = findVar(it.key());
		if (var) {
			var->setValue(it.value());
		}
	}

	storeState();

	return true;
}

//------------------------------------------------------------------------
//Generate names and values separated by separator: x;10;y;20...
string ofxKuTextGui::saveToLine(const string &separator) {
	string line;
	vector<KuUiComponent *> vars = getVars();
	for (int i = 0; i < vars.size(); i++) {
		if (vars[i]->need_save()) {
			string name = vars[i]->name();
			if (!name.empty()) {
				if (!line.empty()) line += separator;
				line += vars[i]->name() + separator + vars[i]->value();
			}
		}
	}
	return line;
}

//------------------------------------------------------------------------
bool ofxKuTextGui::loadFromLine(const string &s, const string &separator) {
	vector<KuUiComponent *> vars = getVars();

	vector<string> items = ofSplitString(s, separator);
	int n = items.size();
	for (int i = 0; i < n; i+=2) {
		if (i + 1 < n) {
			KuUiComponent *var = findVar(items[i]);
			if (var) {
				var->setValue(items[i + 1]);
			}
		}
	}

	storeState();

	return true;
}

//------------------------------------------------------------------------
int ofxKuTextGui::addPage(const string &pageName) {
	KuUiPage page;
	page.setup(pageName);
	page_.push_back(page);
	addTab();
	needRebuild_ = true;

    //add var "[page]" at the top of the page
    if (page_.size() == 1) {
        addStringList(PageVarName, selPage, selPage, 0);
    }
    else {
        addVar(PageVarName);
    }
    vector<string> titles = pageTitles();
    vector<KuUiComponent *> var = findVars(PageVarName);
    for (int i=0; i<var.size(); i++) {
        var[i]->setTitles(titles);
		if (var[i]->type != KuUiType::VStringList) {
			KuUiExitWithMessage("ofxKuTextGui: expected it's stringlist: " + var[i]->name());
		}
        ((KuUiStringList*)var[i])->maxV = int(titles.size())-1;
    }
    selPage = 0;
    return int(page_.size())-1;
    
}

//------------------------------------------------------------------------
void ofxKuTextGui::addTab(int n) {
	for (int i = 0; i < n; i++) {
		if (page_.empty()) addPage("");
		KuUiTab tab;
		page_[page_.size() - 1].addTab();
		needRebuild_ = true;
	}
}

//------------------------------------------------------------------------
KuUiComponent *ofxKuTextGui::addVar(KuUiComponent* var) {
	KuUiComponent *var_ = page_[page_.size()-1].addVar(var);
	needRebuild_ = true;
    return var_;
}

//------------------------------------------------------------------------
bool ofxKuTextGui::setValue(const string &name, const string &value) {
	KuUiComponent *var = findVar(name);
	if (var) {
		var->setValue(value);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------
void ofxKuTextGui::rebuildVars() {
	if (needRebuild_) {
		needRebuild_ = false;
		vars_.clear();
		hash_vars_.clear();
		for (int p = 0; p<page_.size(); p++) {
			KuUiPage &page = page_[p];
			for (int t=0; t<page.tab.size(); t++) {
				KuUiTab &tab = page.tab[t];
				for (int i=0; i<tab.var.size(); i++) {
					KuUiComponent* var_ = tab.var[i];
					vars_[var_->name()] = var_;
					hash_vars_[var_->name()].push_back(var_);
				}
			}
		}
	}
}

//------------------------------------------------------------------------
//adding to current page
KuUiComponent *ofxKuTextGui::addFloat(string name, float &var, float defV, float minV, float maxV,
	int numSteps1, int numSteps2) 
{
	if (page_.empty()) addPage("");
	// TODO need "delete"
	KuUiComponent* var_ = new KuUiFloat(name, var, defV, minV, maxV, numSteps1, numSteps2);
	return addVar(var_);
}

//------------------------------------------------------------------------
KuUiComponent *ofxKuTextGui::addInt(string name, int &var, int defV, int minV, int maxV,
		int step1, int step2) 
{
	if (page_.empty()) addPage("");
	// TODO need "delete"
	KuUiComponent* var_ = new KuUiInt(name, var, defV, minV, maxV, step1, step2);
	return addVar(var_);
}

//------------------------------------------------------------------------
KuUiComponent *ofxKuTextGui::addButton(string name, int &var) {
	if (page_.empty()) addPage("");
	KuUiButton* var_ = new KuUiButton(name, var, 0);
	return addVar(var_);
}

//------------------------------------------------------------------------
KuUiComponent* ofxKuTextGui::addCheckbox(string name, int& var)
{
	if (page_.empty()) addPage("");
	// TODO need "delete"
	KuUiButton* var_ = new KuUiButton(name, var, 0);
	var_->setCheckbox();
	return addVar(var_);
}

//------------------------------------------------------------------------
KuUiComponent *ofxKuTextGui::addString(string name, string &var, const string &defV) {
	if (page_.empty()) addPage("");
	// TODO need "delete"
	KuUiComponent* var_ = new KuUiString(name, var, defV);
	return addVar(var_);
}

//------------------------------------------------------------------------
KuUiComponent *ofxKuTextGui::addStringList(string name, int &var, int defV, const vector<string> &title) {
    if (page_.empty()) addPage("");
	// TODO need "delete"
    KuUiComponent* var_ = new KuUiStringList(name, var, defV, 0, int(title.size())-1, 1, 10, title);
    return addVar(var_);
}

//------------------------------------------------------------------------
KuUiComponent *ofxKuTextGui::addStringList(string name, int &var, int defV, int count...) {
    vector<string> title(count);
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; ++i) {
        title[i] = va_arg(args, char*);
    }
    va_end(args);
    return addStringList(name,var,defV,title);
}

//------------------------------------------------------------------------
void ofxKuTextGui::addDummy(string title, int n) {
	for (int i = 0; i < n; i++) {
		if (page_.empty()) addPage("");
		// TODO delete somethere
		KuUiDummy* var_ = new KuUiDummy(title);
		addVar(var_);
	}
}


//------------------------------------------------------------------------
KuUiComponent *ofxKuTextGui::addVar(string name) {	//adding existing var
	KuUiComponent *var = findVar(name);
	if (var) {
		addVar(var);
	}
	else {
		cout << "ERROR ofxKuTextGui::addVar, no variable " << name << endl;
	}
    return var;
}


//------------------------------------------------------------------------
void ofxKuTextGui::setPage( int index ) {
    if (index >= 0 && index < page_.size()) {
        selPage = index;
		mouse_reset();
    }
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoPrevPage() {
    setPage(selPage-1);
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoNextPage() {
    setPage(selPage+1);
}

//------------------------------------------------------------------------
void ofxKuTextGui::setTab(int index) {
    if (validPage()) {
        KuUiPage &page = page_[selPage];
        page.selTab = index;
        if (page.selTab >= page.tab.size()) {
            page.selTab = 0;
        }
    }
    
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoPrevTab() {
	if (validPage()){
		KuUiPage &page = page_[selPage];
		page.selTab--;
		if (page.selTab < 0) {
			page.selTab = int(page.tab.size())-1;
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoNextTab() {
	if (validPage()){
		KuUiPage &page = page_[selPage];
		page.selTab++;
		if (page.selTab >= page.tab.size()) {
			page.selTab = 0;
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::setValue(int index) {
    if (validPage()){
        KuUiPage &page = page_[selPage];
        if (page.validTab()) {
            KuUiTab &tab = page.tab[page.selTab];
            tab.selVar = index;
            if (tab.selVar<0) tab.selVar = int(tab.var.size())-1;
        }
    }
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoPrevValue() {
	if (validPage()){
		KuUiPage &page = page_[selPage];
		if (page.validTab()) {
			KuUiTab &tab = page.tab[page.selTab];
			tab.selVar--;
			if (tab.selVar<0) tab.selVar = int(tab.var.size())-1;
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoNextValue() {
	if (validPage()){
		KuUiPage &page = page_[selPage];
		if (page.validTab()) {
			KuUiTab &tab = page.tab[page.selTab];
			tab.selVar++;
			if (tab.selVar>=tab.var.size()) tab.selVar = 0;
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::decreaseValue(int speed, int value) {	//0-slow,1-fast
	if (validPage()){
		KuUiPage &page = page_[selPage];
		if (page.validTab()) {
			KuUiTab &tab = page.tab[page.selTab];
			if (tab.validVar()) {
				bool is_string = tab.var[tab.selVar]->is_string();
				if (!is_string || (is_string && editing_strings_)) {
					tab.var[tab.selVar]->inc(-value, speed);
				}
			}
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::increaseValue(int speed, int value) {	//0-slow,1-fast
	if (validPage()){
		KuUiPage &page = page_[selPage];
		if (page.validTab()) {
			KuUiTab &tab = page.tab[page.selTab];
			if (tab.validVar()) {
				bool is_string = tab.var[tab.selVar]->is_string();
				if (!is_string || (is_string && editing_strings_)) {
					tab.var[tab.selVar]->inc(+value, speed);
				}
			}
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::editStringValue() {
	if (validPage() && editing_strings_) {
		KuUiPage &page = page_[selPage];
		if (page.validTab()) {
			KuUiTab &tab = page.tab[page.selTab];
			if (tab.validVar()) tab.var[tab.selVar]->editStringValue();
		}
	}
}


//------------------------------------------------------------------------
//Enable(default) or disable editing string values from keyboard
//This disabling feature t may be useful, because currently editing strings blocks main app loop in oF,
//and for remote controlled GUI it's undesirable to be blocked
void ofxKuTextGui::set_editing_strings(bool v) {	
	editing_strings_ = v;
}

//------------------------------------------------------------------------
void ofxKuTextGui::setPage( const string &name ) {
	for (int i=0; i<page_.size(); i++) {
		if (name == page_[i].name) {
			selPage = i;
			break;
		}
	}
}

//------------------------------------------------------------------------
int ofxKuTextGui::pageIndex() {
    return selPage;
}

//------------------------------------------------------------------------
int ofxKuTextGui::tabIndex() {
    if (validPage()){
        KuUiPage &page = page_[selPage];
        if (page.validTab()) {
            return page.selTab;
        }
    }
    return -1;
}

//------------------------------------------------------------------------
int ofxKuTextGui::varIndex() {
    if (validPage()){
        KuUiPage &page = page_[selPage];
        if (page.validTab()) {
            KuUiTab &tab = page.tab[page.selTab];
            if (tab.validVar()) return tab.selVar;
        }
    }
    return -1;
}

//------------------------------------------------------------------------
string ofxKuTextGui::pageTitle() {
    if (validPage()) return page_[selPage].name;
    else return "";
}

//------------------------------------------------------------------------
bool ofxKuTextGui::validPage() {
	return (selPage >= 0 && selPage < page_.size());
}

//------------------------------------------------------------------------
KuUiPage *ofxKuTextGui::currentPagePointer() {
    if (validPage()) { return &page_[selPage]; }
    return 0;
}

//------------------------------------------------------------------------
vector<string> ofxKuTextGui::pageTitles() {
    int n = page_.size();
    vector<string> titles(n);
    for (int i=0; i<n; i++) {
        titles[i] = page_[i].title;
    }
    return titles;
}

//------------------------------------------------------------------------
ofRectangle ofxKuTextGui::drawn_cell_rect(int tab, int i) {
	float x = drawn_x_ + cellDx + draw_tabW * tab;
    float y = drawn_y_ + cellDy + draw_yStep * i;
	return ofRectangle(x,y,cellW,cellH);                
}

//------------------------------------------------------------------------
void ofxKuTextGui::get_cell_by_coords(float x, float y, int &tab, int &i) {
	tab = (x-drawn_x_-cellDx) / draw_tabW;
	i = (y-drawn_y_-cellDy) / draw_yStep;
}

//------------------------------------------------------------------------
void ofxKuTextGui::update() {					//for buttons processing
	for (int i = 0; i<page_.size(); i++) {
		KuUiPage &page = page_[i];
		for (int j = 0; j<page.tab.size(); j++) {
			KuUiTab &tab = page.tab[j];
			for (int k = 0; k<tab.var.size(); k++) {
				tab.var[k]->update_button();				//TODO optimize and check only button variables
			}
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::draw(float X, float Y, bool enabled, int alpha_text, int alpha_slider) {	//generic draw
	if (validPage()) {
		dd.enabled = enabled;
		dd.alpha_text = alpha_text;
		dd.alpha_text_f = float(alpha_text) / 255.0;
		dd.alpha_slider = alpha_slider;

		ofEnableAlphaBlending();
        
		drawn_x_ = X;
		drawn_y_ = Y;

		KuUiDrawComponentData dc;
		KuUiDrawComponentData modalDc;
		dc.w = cellW;
		dc.h = cellH;
        
		KuUiPage &page = page_[selPage];
		for (int t=0; t<page.tab.size(); t++) {
			KuUiTab &tab = page.tab[t];
			dc.x = X + draw_tabW * t;
			dc.y = Y;

			{
				// Background for the whole tab
				// Find number of visible components
				int n = 0;
				for (int i = 0; i < tab.var.size(); i++) {
					KuUiComponent* var = tab.var[i];
					if (var->visible) {
						n++;
					}
				}
				ofSetColor(dd.background_color);
				ofFill();
				float borderX = (draw_tabW - cellW) / 2;
				ofDrawRectangle(dc.x + cellDx - borderX, dc.y + cellDy - dc.h * 0.1f, dc.w + 2 * borderX, n * (draw_yStep * KuUiComponent::cellHeight()));
			}

			// Tab
			for (int i = 0; i < tab.var.size(); i++) {
				KuUiComponent* var = tab.var[i];
				if (!var->visible) {
					continue;
				}
				dc.selected = (/*enabled &&*/ page.selTab == t && tab.selVar == i);
                
				dc.x0 = dc.x + cellDx;
				dc.y0 = dc.y + cellDy;

				// Modal component will be drawn after all
				if (var == modalComponent_) {
					modalDc = dc;
				}
				else {
					var->draw(dd, dc);
				}
				dc.y += draw_yStep * KuUiComponent::cellHeight();	// Allocate variable space for a components
			}
		}

		// Draw modal component
		if (modalComponent_) {
			modalComponent_->draw(dd, modalDc);
		}

	}
}

//------------------------------------------------------------------------
string ofxKuTextGui::drawToString() {  //keeps current page, tabs, selected value
    string s = "";
    if (validPage()) {
        KuUiPage &page = page_[selPage];
        for (int t=0; t<page.tab.size(); t++) {
            if (t>0) s += ";";
            KuUiTab &tab = page.tab[t];
            for (int i=0; i<tab.var.size(); i++) {
                KuUiComponent* var = tab.var[i];
                bool selected = (page.selTab==t && tab.selVar==i);
                string name = var->name();
                if ( selected ) name = ">" + name;
                else name = " " + name;
                
                if (i>0) s+= ",";
                s += name + "=" + var->value() + "=" + ofToString(var->valueNormalized());
            }
        }
    }
    return s;
}

//------------------------------------------------------------------------
//using custom font - if not, using ofDrawBitmapString
void ofxKuTextGui::set_font(ofTrueTypeFont *font, float shift_x, float shift_y) {
	set_fonts(font, font, font, shift_x, shift_y);
}

//------------------------------------------------------------------------
void ofxKuTextGui::set_fonts(ofTrueTypeFont* font, ofTrueTypeFont* fontBold, ofTrueTypeFont* fontItalics, float shift_x, float shift_y)
{
	dd.fonts[int(KuUiFontIndex::Normal)] = font;
	dd.fonts[int(KuUiFontIndex::Bold)] = fontBold;
	dd.fonts[int(KuUiFontIndex::Italics)] = fontItalics;
	dd.font_shift_x = shift_x;
	dd.font_shift_y = shift_y;
}

//------------------------------------------------------------------------
void ofxKuTextGui::drawFromString(const string &s, float X, float Y) {
    vector<string> tabs = ofSplitString(s, ";");
    float w = cellW;
    float h = cellH;
        
    for (int t=0; t<tabs.size(); t++) {
        vector<string> tab = ofSplitString(tabs[t], ",");
        for (int i=0; i<tab.size(); i++) {
            vector<string> var = ofSplitString(tab[i],"=");
            if (var.size()>=3) {
                string name = var[0];
                string value = var[1];
                float valueNormalized = ofToFloat(var[2]);
                bool selected = (!name.empty() && name[0] == '>');
                float x = X + draw_tabW * t;
                float y = Y + draw_yStep * i;
                
                ofFill();
                ofSetColor(0);
                ofDrawRectangle(x+cellDx,y+cellDy,w,h);
                if (dd.drawSliderMode) {
                    if (selected) ofSetColor(200,200,0);
                    else ofSetColor(128);
                    ofNoFill();
                    ofDrawRectangle(x+cellDx,y+cellDy,w,h);
                }
                ofSetColor(255);
				KuUiComponent::draw_string(dd, name + " " + value, x, y, KuUiFontIndex::Normal);
                if (dd.drawSliderMode) {
                    ofFill();
                    ofSetColor(255,60);
                    ofDrawRectangle(x+cellDx,y+cellDy,w*valueNormalized,h);
                    if (selected) ofSetColor(255,255,0);
                    else ofSetColor(200);
                    ofNoFill();
                    ofDrawRectangle(x+cellDx,y+cellDy,w*valueNormalized,h);
                }
            }
        }
    }
}

//------------------------------------------------------------------------
bool ofxKuTextGui::keyPressed(int key) {       //generic keyPressed handler
	if (modalComponent_) {
		modalComponent_->modalKeyPressed(key);
		if (!modalComponent_->is_modal()) {
			stopModalMode();
		}
		return true;
	}
    if (key == '1')             { gotoPrevPage(); return true; }
    if (key == '2')             { gotoNextPage(); return true; }
	if (key == '!')				{ setPage(0); return true; }
	if (key == '@')				{ setPage(int(page_.size())-1); return true; }
    if (key == OF_KEY_LEFT)     { gotoPrevTab(); return true; }
    if (key == OF_KEY_RIGHT)    { gotoNextTab(); return true; }
    if (key == OF_KEY_UP)       { gotoPrevValue(); return true; }
    if (key == OF_KEY_DOWN)     { gotoNextValue(); return true; }
    if (key == '[')             { decreaseValue(0); return true; }
    if (key == ']')             { increaseValue(0); return true; }
    if (key == '{')             { decreaseValue(1); return true; }
    if (key == '}')             { increaseValue(1); return true; }

    return false;
}

//------------------------------------------------------------------------
KuUiComponent *ofxKuTextGui::findVar(const string &name) {
	rebuildVars();
	StringVarHash::iterator p = hash_vars_.find( name );
    return ( p != hash_vars_.end() )? hash_vars_[name][0] : nullptr;
}

//------------------------------------------------------------------------
KuUiComponent *ofxKuTextGui::findVarChecking(const string &name) {   //one var, exits if no found
	KuUiComponent *v = findVar(name);
	if (!v) {	//Note - it shouldn't put checking of v to "KuUiExitWithMessage" to avoid constructing strings each calling
		KuUiExitWithMessage("ofxKuTextGui error in findVarChecking, no var '" + name + "'");
	}
	return v;
}

//------------------------------------------------------------------------
float *ofxKuTextGui::findVarFloat(const string &name) {
	KuUiComponent* v = findVarChecking(name);
	if (v->type != KuUiType::VFloat) {
		KuUiExitWithMessage("ofxKuTextGui: No float " + name);
	}
    return ((KuUiFloat*)v)->var;
}

//------------------------------------------------------------------------
int *ofxKuTextGui::findVarInt(const string &name) {
	KuUiComponent* v = findVarChecking(name);
	if (v->type != KuUiType::VInt) {
		KuUiExitWithMessage("ofxKuTextGui: No int " + name);
	}
	return ((KuUiInt*)v)->var;
}

//------------------------------------------------------------------------
int *ofxKuTextGui::findVarStringList(const string &name) {
	KuUiComponent* v = findVarChecking(name);
	if (v->type != KuUiType::VStringList) {
		KuUiExitWithMessage("ofxKuTextGui: No stringlist " + name);
	}
	return ((KuUiStringList*)v)->var;
}

//------------------------------------------------------------------------
string *ofxKuTextGui::findVarString(const string &name) {
	KuUiComponent* v = findVarChecking(name);
	if (v->type != KuUiType::VString) {
		KuUiExitWithMessage("ofxKuTextGui: No string " + name);
	}
	return ((KuUiString*)v)->var;
}

//------------------------------------------------------------------------
int *ofxKuTextGui::findVarButton(const string &name) {
	KuUiComponent* v = findVarChecking(name);
	if (v->type != KuUiType::VInt || !v->is_button()) {
		KuUiExitWithMessage("ofxKuTextGui: No button " + name);
	}
	return ((KuUiInt*)v)->var;
}

//------------------------------------------------------------------------
void ofxKuTextGui::setToggled(const string& name, int t) {		// Need call this, because there can be multiple duplicated buttons 
	StringVarHash::iterator p = hash_vars_.find(name);
	if (p == hash_vars_.end()) {
		cout << "ofxKuTextGui::setToggled: can't find " << name << endl;
	} 
	
	for (auto b : hash_vars_[name]) {
		b->setToggled(t);
	}
}

//------------------------------------------------------------------------
float &ofxKuTextGui::float_(const string &name) {
	return *findVarFloat(name);
}

//------------------------------------------------------------------------
int &ofxKuTextGui::int_(const string &name) {
	return *findVarInt(name);
}

//------------------------------------------------------------------------
int &ofxKuTextGui::stringlist_(const string &name) {
	return *findVarStringList(name);
}

//------------------------------------------------------------------------
string &ofxKuTextGui::string_(const string &name) {
	return *findVarString(name);
}

//------------------------------------------------------------------------
int &ofxKuTextGui::button_(const string &name) {
	return *findVarButton(name);
}

//------------------------------------------------------------------------
//applicable to float and int vars, automatically enables drawing smoothed value
float ofxKuTextGui::updateSmoothedValue(const string name, float dt, float time_whole_change) {
	auto *var = findVarChecking(name);
	var->setDrawSmoothed(true);
	var->updateSmoothedValue(dt, time_whole_change);
	return var->getSmoothedValue();
}

//------------------------------------------------------------------------
float ofxKuTextGui::getSmoothedValue(const string name) {
	return findVarChecking(name)->getSmoothedValue();
}

//------------------------------------------------------------------------
vector<KuUiComponent *> ofxKuTextGui::findVars(const string &name) {   //all instances
	//rebuildVars();
    vector<KuUiComponent *> vars;
    for (int i=0; i<page_.size(); i++) {
        KuUiPage &page = page_[i];
        for (int j=0; j<page.tab.size(); j++) {
            KuUiTab &tab = page.tab[j];
            for (int k=0; k<tab.var.size(); k++) {
                if (tab.var[k]->name() == name) {
                    vars.push_back(tab.var[k]);
                }
            }
        }
    }
    return vars;
    
}

//------------------------------------------------------------------------
void ofxKuTextGui::setDrawSliderMode(bool value) { //should we draw slider
    dd.drawSliderMode = value;
}

//------------------------------------------------------------------------
void ofxKuTextGui::set_mouse_enabled(bool v) {
	mouse_enabled_  = v;
}

//------------------------------------------------------------------------
void ofxKuTextGui::set_mouse_step(int step) {
	mouse_step_ = step;
}

//------------------------------------------------------------------------
bool ofxKuTextGui::mousePressed(int x, int y, int button) {
	if (mouse_enabled_) {
		if (modalComponent_) {
			modalComponent_->modalMousePressed(x, y, button);
			if (!modalComponent_->is_modal()) {
				stopModalMode();
			}
			return true;
		}

		if (validPage()) {
			KuUiPage *page = currentPagePointer();		
			if (page) {
				int t, iNotUse;
				get_cell_by_coords(x, 0, t, iNotUse);	// get only tab 
				if (t >= 0 && t < page->tab.size()) {
					glm::vec2 pos(x, y);
					KuUiTab &tab = page->tab[t];
					for (int i = 0; i < tab.var.size(); i++) {
						auto& var = tab.var[i];
						if (var->is_editable() && var->mouseInside(pos)) {
							page->selTab = t;
							tab.selVar = i;

							//check button
							if (tab.validVar() && var->acceptMouseClick()) {
								var->mouseClick(x, y, button);
							}
							else if (tab.validVar() && var->start_modal_on_click()) {
								startModalMode(var);
								return true;
							}
							else {
								mouse_dragging_ = true;
								mouse_x_ = x;
								mouse_y_ = y;
							}
							return true;
						}
					}
				}
			}
		}
	}
    return false;
}

//------------------------------------------------------------------------
void ofxKuTextGui::mouseDragged(int x, int y, int button) {
	if (mouse_enabled_) {
		if (mouse_dragging_) {
			int speed = (button == 2)?1:0;

			int delta = x - mouse_x_;
			int a = abs(delta);
			if (a >= mouse_step_) {
				int shift = (a / mouse_step_) ;				
				if (delta>0) increaseValue(speed, shift);
				else decreaseValue(speed, shift);
				mouse_x_ += shift * mouse_step_ * ((delta>0)?1:-1);
			}
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::mouseReleased()
{
	mouse_reset();
}

//------------------------------------------------------------------------
void ofxKuTextGui::mouse_reset() {
	mouse_dragging_ = false;
}

//------------------------------------------------------------------------
void ofxKuTextGui::startModalMode(KuUiComponent* var) {
	modalComponent_ = var;
}

//------------------------------------------------------------------------
void ofxKuTextGui::stopModalMode() {
	modalComponent_ = nullptr;
}

//------------------------------------------------------------------------
