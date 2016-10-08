#include "ofxKuTextGui.h"


//------------------------------------------------------------------------
ofxKuTextGui::ofxKuTextGui() {
	selPage = 0;

	draw_tabW = 150;
	draw_yStep = 20;

	needRebuild_ = true;

}


//------------------------------------------------------------------------
vector<ofxKuTextGui::Var *> ofxKuTextGui::getVars() {
	rebuildVars();
	vector<Var *> vars;
	for (StringVarMap::const_iterator iter = vars_.begin(); iter != vars_.end(); iter++) {
		vars.push_back( iter->second );
	}
	return vars;
}

//------------------------------------------------------------------------
void ofxKuTextGui::loadFromFile(const string &fileName) {
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
	vector<Var *> vars = getVars();
	for (int k=0; k<lines.size(); k++) {
		vector<string> item = ofSplitString(lines[k], "=", true, true);
		if (item.size() >= 2) {
			string name		= item[0];
			string value	= item[1];
			Var *var = findVar(name);
			if (var) {
				var->setValue(value);
			}
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::saveToFile(const string &fileName) {
	vector<string> file;
	vector<Var *> vars = getVars();
	for (int i=0; i<vars.size(); i++) {
		string line = vars[i]->name() + "=" + vars[i]->value();
		file.push_back(line);
		cout << line << endl;
	}
	ofstream f(ofToDataPath(fileName).c_str(),ios::out);
    for ( size_t i=0; i<file.size(); i++ ) {
        f << file[i] << endl;
    }
    f.close();
}

//------------------------------------------------------------------------
void ofxKuTextGui::addPage(const string &pageName) {
	Page page;
	page.name = pageName;
	page_.push_back(page);
	addTab();
	needRebuild_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGui::addTab() {
	if (page_.empty()) addPage("");
	Tab tab;
	page_[page_.size()-1].addTab();	
	needRebuild_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGui::addVar(Var &var) {
	page_[page_.size()-1].addVar(var);
	needRebuild_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGui::rebuildVars() {
	if (needRebuild_) {
		needRebuild_ = false;
		vars_.clear();
		for (int p = 0; p<page_.size(); p++) {
			Page &page = page_[p];
			for (int t=0; t<page.tab.size(); t++) {
				Tab &tab = page.tab[t];
				for (int i=0; i<tab.var.size(); i++) {
					Var &var_ = tab.var[i];
					vars_[var_.name()] = &var_;
				}
			}
		}
	}
}

//------------------------------------------------------------------------
//adding to current page
void ofxKuTextGui::addFloat(string name, float &var, float defV, float minV, float maxV, 
	int numSteps1, int numSteps2) 
{
	if (page_.empty()) addPage("");
	Var var_;
	var_.index=0;
	var_.vfloat = VarFloat(name, var, defV, minV, maxV, numSteps1, numSteps2);
	addVar(var_);

}

//------------------------------------------------------------------------
void ofxKuTextGui::addInt(string name, int &var, int defV, int minV, int maxV,
		int step1, int step2) 
{
	if (page_.empty()) addPage("");
	Var var_;
	var_.index=1;
	var_.vint = VarInt(name, var, defV, minV, maxV, step1, step2);
	addVar(var_);
}

//------------------------------------------------------------------------
void ofxKuTextGui::addString(string name, string &var, const string &defV) {
	if (page_.empty()) addPage("");
	Var var_;
	var_.index=2;
	var_.vstring = VarString(name, var, defV);
	addVar(var_);
}

//------------------------------------------------------------------------
void ofxKuTextGui::addVar(string name) {	//adding existing var
	Var *var = findVar(name);
	if (var) {
		addVar(*var);
	}
	else {
		cout << "ERROR ofxKuTextGui::addVar, no variable " << name << endl;
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoPrevTab() {
	if (validPage()){
		Page &page = page_[selPage];
		page.selTab--;
		if (page.selTab < 0) {
			page.selTab = int(page.tab.size())-1;
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoNextTab() {
	if (validPage()){
		Page &page = page_[selPage];
		page.selTab++;
		if (page.selTab >= page.tab.size()) {
			page.selTab = 0;
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoPrevValue() {
	if (validPage()){
		Page &page = page_[selPage];
		if (page.validTab()) {
			Tab &tab = page.tab[page.selTab];
			tab.selVar--;
			if (tab.selVar<0) tab.selVar = int(tab.var.size())-1;
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::gotoNextValue() {
	if (validPage()){
		Page &page = page_[selPage];
		if (page.validTab()) {
			Tab &tab = page.tab[page.selTab];
			tab.selVar++;
			if (tab.selVar>=tab.var.size()) tab.selVar = 0;
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::decreaseValue(int speed) {	//0-slow,1-fast
	if (validPage()){
		Page &page = page_[selPage];
		if (page.validTab()) {
			Tab &tab = page.tab[page.selTab];
			if (tab.validVar()) tab.var[tab.selVar].inc(-1,speed);
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::increaseValue(int speed) {	//0-slow,1-fast
	if (validPage()){
		Page &page = page_[selPage];
		if (page.validTab()) {
			Tab &tab = page.tab[page.selTab];
			if (tab.validVar()) tab.var[tab.selVar].inc(+1,speed);
		}
	}
}

//------------------------------------------------------------------------
void ofxKuTextGui::setActive( bool active ) {

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
bool ofxKuTextGui::validPage() {
	return (selPage >= 0 && selPage < page_.size());
}

//------------------------------------------------------------------------
void ofxKuTextGui::draw(float X, float Y) {	//generic draw
	if (validPage()) {
		Page &page = page_[selPage];
		for (int t=0; t<page.tab.size(); t++) {
			Tab &tab = page.tab[t];
			for (int i=0; i<tab.var.size(); i++) {
				Var &var = tab.var[i];
				bool selected = (page.selTab==t && tab.selVar==i);
				string name = var.name();
				if ( selected ) name = ">" + name;
				else name = " " + name;
				ofDrawBitmapStringHighlight(name+" "+var.value(),
					X + draw_tabW * t, Y + draw_yStep * i);
			}
		}
	}
}

//------------------------------------------------------------------------
ofxKuTextGui::Var *ofxKuTextGui::findVar(const string &name) {
	rebuildVars();
	StringVarMap::iterator p = vars_.find( name );
    return ( p != vars_.end() )?vars_[name]:0;
}

//------------------------------------------------------------------------
