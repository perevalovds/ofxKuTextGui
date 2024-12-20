#include "ofxKuTextGuiGen.h"

#include <unordered_map>

//------------------------------------------------------------------------
void ofxKuTextGuiGen::put(string line, vector<string> &lines) {
    lines.push_back(line);
}

//------------------------------------------------------------------------
void ofxKuTextGuiGen::saveFile(vector<string> &lines, string fileName) {
    ofstream f(ofToDataPath(fileName).c_str(),ios::out);
    for ( size_t i=0; i<lines.size(); i++ ) {
        f << lines[i] << endl;
    }
    f.close();
}

//------------------------------------------------------------------------
vector<string> ofxKuTextGuiGen::loadFile(string fileName, bool trim, bool skip_comments_and_empty) {
	vector<string> lines;
	ifstream F(ofToDataPath(fileName).c_str(), ios::in | ios::binary); //TODO binary???
	string line;
	while (getline(F, line)) {
		if (trim) ofTrim(line);
		if (line == "") continue;         //skip empty line
		if (line[0] == '#') continue;   //comment
		//remove '\r' for proper reading windows-files in linux
		while (!line.empty() && line[line.length() - 1] == '\r') {
			line = line.substr(0, line.length() - 1);
		}
		lines.push_back(line);
	}
	return lines;
}

//------------------------------------------------------------------------
void ofxKuTextGuiGen::insert(vector<string>& lines, vector<string>& what_to_insert) {
	for (int i = 0; i < what_to_insert.size(); i++) {
		lines.push_back(what_to_insert[i]);
	}
}

//------------------------------------------------------------------------
//Create H/CPP files from GUI script file
void ofxKuTextGuiGen::generateCPP(string gui_file_in, string c_path, string c_file_out,
	string class_name, string extern_var_name, string define_prefix_name) {

	vector<string> lines = loadFile(gui_file_in);

	generate_common(true, false, lines, 0, c_path, c_file_out,
		class_name, extern_var_name, define_prefix_name);
}

//------------------------------------------------------------------------
//Create GUI from GUI script file
void ofxKuTextGuiGen::createGuiFromFile(ofxKuTextGui& gui, string gui_file_in) {
	vector<string> lines = loadFile(gui_file_in);

	createGuiFromLines(gui, lines);
}

//------------------------------------------------------------------------
//Create GUI from list of lines
void ofxKuTextGuiGen::createGuiFromLines(ofxKuTextGui& gui, vector<string>& lines) {
	generate_common(false, true, lines, &gui);
}

//------------------------------------------------------------------------
struct ofxKuTextGuiGenTemplate {
	string name;
	vector<string> params;
	vector<string> lines;
};

//------------------------------------------------------------------------
void ofxKuTextGuiGen::processTemplates(const vector<string>& lines0, vector<string>& lines)
{
	/*
	TEMPLATE ITEM X Y
	dummy {X}{Y}
	int Pos{X}{Y}=0 1:10 1,10
	ENDTEMPLATE

	ITEM 1 1
	ITEM 1 2
	ITEM 2 1
	*/

	vector<ofxKuTextGuiGenTemplate> templates;
	unordered_map<string, int> nameToIndex;
	ofxKuTextGuiGenTemplate* templ = nullptr;

	lines.clear();
	for (auto& line : lines0) {
		vector<string> list = ofSplitString(line, " ", true);
		string name = (list.empty()) ? "" : list[0];
		if (!templ) {
			if (name == "ENDTEMPLATE") {
				KuUiExitWithMessage("ENDTEMPLATE without TEMPLATE");
			}
			if (name == "TEMPLATE") {
				if (list.size() < 2) {
					KuUiExitWithMessage("TEMPLATE must have name");
				}
				templates.push_back(ofxKuTextGuiGenTemplate());
				templ = &templates.back();	// Important! this pointer is temporary - after adding next template it can be invalidated dur vector<> resize
				templ->name = list[1];
				nameToIndex[templ->name] = int(templates.size()) - 1;

				for (int i = 2; i < list.size(); i++) {
					templ->params.push_back("{" + list[i] + "}");
				}
			}
			else {
				// Check if we need to insert template
				if (nameToIndex.count(name) > 0) {
					int index = nameToIndex[name];
					auto& templ = templates[index];
					if (templ.params.size() != list.size() - 1) {
						KuUiExitWithMessage("Template " + name + " expects " + ofToString(templ.params.size())
							+ " but got " + ofToString(list.size() - 1) + ", at " + line);
					}
					for (string L : templ.lines) {
						for (int i = 0; i < templ.params.size(); i++) {
							ofStringReplace(L, templ.params[i], list[i + 1]);
						}
						lines.push_back(L);
					}
				}
				else {
					// Just copy original line
					lines.push_back(line);
				}
			}
		}
		else {
			if (name == "TEMPLATE") {
				KuUiExitWithMessage("TEMPLATE can't be inside other TEMPLATE");
			}
			if (name == "ENDTEMPLATE") {
				templ = nullptr;
			}
			else {
				templ->lines.push_back(line);
			}
		}
	}

}

//------------------------------------------------------------------------
void ofxKuTextGuiGen::generate_common(bool make_cpp, bool make_gui,
	vector<string> &lines0,
	ofxKuTextGui *gui,
		string c_path,
		string c_file_out,
		string class_name,
		string extern_var_name,
		string define_prefix_name) {


	vector<string> lines;
	processTemplates(lines0, lines);

    vector<string> Decl;  //struct declaration
	vector<string> PageEnum;

    vector<string> Constr;  //constructor
    vector<string> Setup;  //setup_gui
    vector<string> ApplyConstChanges;  //applyConstChanges
	vector<string> ColorLines;	//setting colors
	string current_color;	//if not empty - the set to this color
    
	// Stack of conditions
	vector<KuUiVisibilityConditionStr> visibilityConditions;

    for (int i=0; i<lines.size(); i++) {
        string line = lines[i];
        ofStringReplace(line, "\t", " ");
        ofStringReplace(line, "  ", " ");
        line = ofTrimFront(line);
        if (line.empty()) continue;
        vector<string> item = ofSplitString(line, " ");
        int n = item.size();
        string type_s = item[0];
        string name_s = (n>=2) ? item[1] : "";
        string range_s = (n>=3) ? item[2] : "";
        string step_s = (n>=4) ? item[3] : "";
        
        Pair name_pair(name_s,"=");
        Pair range_pair(range_s, ":");
        Pair step_pair(step_s, ",");
        Name name_code(name_pair.a);

		// Conditional visibility
		if (type_s == "IF") {
			if (name_s.empty()) {
				KuUiExitWithMessage("Bad IF, no argument, but must have arguments i.e. Value==1,2");
			}
			auto list = ofSplitString(name_s, "==");
			if (list.size() != 2) {
				KuUiExitWithMessage("Bad " + line + ", expected arguments i.e.Value==1,2");
			}
			KuUiVisibilityConditionStr cond;
			cond.valueName = list[0];
			cond.values = ofSplitString(list[1], ",");
			visibilityConditions.push_back(cond);
		}

		if (type_s == "ENDIF") {
			if (visibilityConditions.empty()) {
				KuUiExitWithMessage("Bad ENDIF");
			}
			visibilityConditions.pop_back();
		}

        if (name_code.is_const) {
			if (make_cpp) put("\t" + name_code.const_name + " = " + name_code.code_name + ";", ApplyConstChanges);
			if (make_gui) { //TODO support of constants
			}
        }

		if (type_s == "PAGE" && n>=2) {
			if (make_cpp) {
				put("\tgui.addPage(\"" + name_s + "\");", Setup);

				// Cleanup page name
				string enumName = name_s;
				ofStringReplace(enumName, " ", "_");
				ofStringReplace(enumName, "\t", "_");
				ofStringReplace(enumName, "-", "_");
				ofStringReplace(enumName, ".", "_");
				ofStringReplace(enumName, "&", "_");
				if (!enumName.empty() && enumName[0] >= '0' && enumName[0] <= '9') {
					enumName = "_" + enumName;
				}
				string line = "        " + enumName + " = " + ofToString(PageEnum.size());
				if (!PageEnum.empty()) {
					PageEnum.back() += ",";
				}
				PageEnum.push_back(line);
			}
			if (make_gui) gui->addPage(name_s);
		}
        if (type_s == "TAB") {
			if (make_cpp) put("\tgui.addTab();", Setup);
			if (make_gui) gui->addTab();
        }

		if (type_s == "COLOR") {
			current_color = name_s;	//second argument
		}
		if (type_s == "RESET_COLOR") {
			current_color = "";
		}
        
		// var
		if (type_s == "var") {
			if (make_cpp) put("\tgui.addVar(\"" + name_code.screen_name + "\");", Setup);
			if (make_gui) gui->addVar(name_code.screen_name);
		}

		bool is_dummy = false;
		string uniqueName = "";	// If will be filled by dummy, use it

		// dummy
		if (type_s == "dummy") {
			vector<string> title0 = item;
			title0.erase(title0.begin());
			string title = ofJoinString(title0, " ");
			static int DummyID = 0;	// Unique ID to separate dummies
			uniqueName = title + "_" + ofToString(DummyID++);

			if (make_cpp) put("\tgui.addDummy(\"" + title + "\", \"" + uniqueName + "\");", Setup);
			if (make_gui) gui->addDummy(title);
			is_dummy = true;
		}
        
		bool is_var = false;

		// int, float
		if (type_s == "int" || type_s == "float") {
            if (name_code.is_const) {
				if (make_cpp) put("\t" + type_s + " " + name_code.const_name + ";", Decl);
				if (make_gui) {
					//TODO CONST
				}
            }
			if (make_cpp) put("\t" + type_s + " " + name_code.code_name + ";" + name_code.comment, Decl);
			if (make_cpp) put("\t" + name_code.code_name + "=" + name_pair.b + ";", Constr);
            if (type_s == "int") {
                //int *FPS=30 1:100 1,10
				if (make_cpp) put("\tgui.addInt(\"" + name_code.screen_name + "\","
                    + name_code.code_name + "," + name_pair.b + ","
                    + range_pair.a + "," + range_pair.b + ","
                    + step_pair.a + "," + step_pair.b + ");",
                    Setup);
				if (make_gui) {
					int *null = 0;	//means that var must create autovariable itself
					gui->addInt(name_code.screen_name, *null, ofToInt(name_pair.b), ofToInt(range_pair.a), ofToInt(range_pair.b),
						ofToInt(step_pair.a), ofToInt(step_pair.b));
				}
            }
            if (type_s == "float") {
                //float -fps=30 0:100 100,10
				if (make_cpp) put("\tgui.addFloat(\"" + name_code.screen_name + "\","
                    + name_code.code_name + "," + name_pair.b + ","
                    + range_pair.a + "," + range_pair.b + ","
                    + step_pair.a + "," + step_pair.b + ");",
                    Setup);
				if (make_gui) {
					float *null = 0;
					gui->addFloat(name_code.screen_name, *null, ofToFloat(name_pair.b), 
						ofToFloat(range_pair.a), ofToFloat(range_pair.b),
						ofToFloat(step_pair.a), ofToFloat(step_pair.b));
				}
            }
			is_var = true;
        }

		// string
        if (type_s == "string") {
            if (name_code.is_const) {
				if (make_cpp) put("\t" + type_s + " " + name_code.const_name + ";", Decl);
            }
            //string send_host=localhost
			if (make_cpp) put("\t" + type_s + " " + name_code.code_name + ";" + name_code.comment, Decl);
			if (make_cpp) put("\t" + name_code.code_name + "=\"" + name_pair.b + "\";", Constr);
			if (make_cpp) put("\tgui.addString(\"" + name_code.screen_name + "\","
                + name_code.code_name + ",\"" + name_pair.b + "\");",
                Setup);

			if (make_gui) {
				string *null = 0;
				gui->addString(name_code.screen_name, *null, name_pair.b);
			}

			is_var = true;
        }

		// stringlist
        if (type_s == "stringlist") {
            if (name_code.is_const) {
				if (make_cpp) put("\tint " + name_code.const_name + ";", Decl);
            }

            //stringlist list=a [a,b,c]
			if (make_cpp) put("\tint " + name_code.code_name + ";" + name_code.comment, Decl);
            vector<string> list = parse_stringlist_values(range_s);
            string def_index = "0";
            for (int i=0; i<list.size(); i++) {
                if (list[i] == name_pair.b) {
                    def_index = ofToString(i);
                    break;
                }
            }
			if (make_cpp) put("\t" + name_code.code_name + "=" + def_index + ";", Constr);
            string line = "\tgui.addStringList(\"" + name_code.screen_name + "\","
            + name_code.code_name + "," + def_index + ","
            + ofToString(list.size());
            if (list.size()>0) {
                line += "," + stringlist_values(list) + ");";
            }
			if (make_cpp) put(line,Setup);

			if (make_gui) {
				int *null = 0;
				gui->addStringList(name_code.screen_name, *null, ofToInt(name_pair.b), list);
			}

			is_var = true;
        }

		// button
		if (type_s == "button") {
			if (name_code.is_const) {			//...but of course for buttons "const" value is meaningless :)
				if (make_cpp) put("\tint " + name_code.const_name + ";", Decl);
			}
			if (make_cpp) {
				put("\tint " + name_code.code_name + ";" + name_code.comment, Decl);
				put("\t" + name_code.code_name + "=0;", Constr);
				put("\tgui.addButton(\"" + name_code.screen_name + "\"," + name_code.code_name + ");", Setup);
			}

			if (make_gui) {
				int *null = 0;
				gui->addButton(name_code.screen_name, *null);
			}

			is_var = true;
		}

		// checkbox
		if (type_s == "checkbox") {
			if (name_code.is_const) {			
				if (make_cpp) put("\tint " + name_code.const_name + ";", Decl);
			}
			if (make_cpp) {
				put("\tint " + name_code.code_name + ";" + name_code.comment, Decl);
				put("\t" + name_code.code_name + "=0;", Constr);
				put("\tgui.addCheckbox(\"" + name_code.screen_name + "\"," + name_code.code_name + ");", Setup);
			}

			if (make_gui) {
				int* null = 0;
				gui->addButton(name_code.screen_name, *null);
			}

			is_var = true;
		}

		// array
		if (type_s == "array") {
			if (make_cpp) {
				put("\tvector<int *> " + name_code.code_name + ";  // array", Decl);

				vector<string> list = ofSplitString(name_pair.b, ",", true, true);
				string s;
				for (auto& var : list) {
					if (!s.empty()) s += ", ";
					s += "&" + var;
				}
				put("\t" + name_code.code_name + " = {" + s + "};", Setup);
			}
			
		}

		// Setting color, visibility, editing
		if (is_dummy || is_var) {			
			string name = (uniqueName.empty()) ? name_code.screen_name : uniqueName;

			if (!current_color.empty()) {
				if (make_cpp) put("\tgui.set_var_color(\"" + name + "\", ofColor(" + current_color + "));", ColorLines);
				if (make_gui) {
					vector<string> items_s = ofSplitString(current_color, ",");
					vector<int> items;
					for (auto& s : items_s) {
						items.push_back(ofToInt(s));
					}
					ofColor color;
					if (items.size() == 1) color = ofColor(items[0]);
					if (items.size() == 2) color = ofColor(items[0], items[1]);
					if (items.size() == 3) color = ofColor(items[0], items[1], items[2]);
					if (items.size() == 4) color = ofColor(items[0], items[1], items[2], items[3]);
					gui->set_var_color(name, color);
				}
			}
			if (!name_code.visible) {
				if (make_cpp) {
					put("\tgui.set_var_visibility(\"" + name + "\", false);", ColorLines);
				}
				if (make_gui) {
					gui->set_var_visibility(name, false);
				}
			}
			if (!visibilityConditions.empty()) {
				if (make_cpp) {
					put("\tgui.set_var_visibility_conditions(\"" + name + "\","
						+ KuUiVisibilityConditionStr::toStr(visibilityConditions) + ");", ColorLines);
				}
				if (make_gui) {
					// TODO
				}
			}

			if (!name_code.editable) {
				if (make_cpp) {
					put("\tgui.set_var_editable(\"" + name + "\", false);", ColorLines);
				}
				if (make_gui) {
					gui->set_var_editable(name, false);
				}
			}
		}
    }
    
	// Check that stack is empty
	if (!visibilityConditions.empty()) {
		KuUiExitWithMessage("Number of IFs is greater than number of ENDIFs");
	}

	//Save files
	if (make_cpp) {
		//.H file
		vector<string> f_h;
		put("#pragma once", f_h);
		put("//Auto-generated GUI file for ofxKuTextGui, " + ofGetTimestampString(), f_h);
		put("", f_h);
		put("#include \"ofMain.h\"", f_h);
		put("#include \"ofxKuTextGui.h\"", f_h);

		put("", f_h);
		put("struct " + class_name + " {", f_h);

		// Page enum
		put("    enum class PageEnum: int {", f_h);
		insert(f_h, PageEnum);
		put("    };", f_h);

		// Getting pageenum
		put("    PageEnum pageEnum() { return PageEnum(gui_->pageIndex()); } ", f_h);

		insert(f_h, Decl);
		put("    " + class_name + "();", f_h);
		put("    void setup(ofxKuTextGui &gui, string fileName);", f_h);
		put("    void applyConstChanges();", f_h);
		put("    void save();", f_h);
		put("    string fileName_;", f_h);
		put("    ofxKuTextGui *gui_;", f_h);
		put("};", f_h);
		put("", f_h);

		if (!extern_var_name.empty()) {
			put("extern " + class_name + " " + extern_var_name + ";", f_h);
			if (!define_prefix_name.empty()) {
				put("#define " + define_prefix_name + " " + extern_var_name + ".", f_h);
			}
		}

		saveFile(f_h, c_path + c_file_out + ".h");

		//.CPP file
		vector<string> f_cpp;

		put("#include \"" + c_file_out + ".h\"", f_cpp);
		put("//Auto-generated GUI file for ofxKuTextGui, " + ofGetTimestampString(), f_cpp);
		put("", f_cpp);
		if (!extern_var_name.empty()) {
			put(class_name + " " + extern_var_name + ";", f_cpp);
		}
		put("//--------------------------------------------------------------", f_cpp);
		put(class_name + "::" + class_name + "() {", f_cpp);
		insert(f_cpp, Constr);
		put("}", f_cpp);
		put("", f_cpp);
		put("//--------------------------------------------------------------", f_cpp);
		put("void " + class_name + "::setup(ofxKuTextGui &gui, string fileName) {", f_cpp);
		insert(f_cpp, Setup);
		insert(f_cpp, ColorLines);
		put("\tfileName_ = fileName;", f_cpp);
		put("\tgui_ = &gui;", f_cpp);
		put("\tgui.loadFromFile(fileName);", f_cpp);
		put("\tapplyConstChanges();", f_cpp);
		put("}", f_cpp);
		put("", f_cpp);
		put("//--------------------------------------------------------------", f_cpp);
		put("void " + class_name + "::applyConstChanges() {", f_cpp);
		insert(f_cpp, ApplyConstChanges);
		put("}", f_cpp);
		put("", f_cpp);
		put("//--------------------------------------------------------------", f_cpp);
		put("void " + class_name + "::save() {", f_cpp);
		put("\tgui_->saveToFile(fileName_);", f_cpp);
		put("}", f_cpp);
		put("", f_cpp);
		put("//--------------------------------------------------------------", f_cpp);
		put("", f_cpp);
		saveFile(f_cpp, c_path + c_file_out + ".cpp");


		cout << "H/CPP GUI files " << c_file_out << " are generated" << endl;
	}
}

//------------------------------------------------------------------------
