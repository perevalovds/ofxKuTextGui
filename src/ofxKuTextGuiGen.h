#pragma once

/*C++ code generator for ofxKuTextGui addon.
To use it, prepare GUI script, see gui-script.ini,
 and calling generateCPP() will generate .H and .CPP files, which implements this GUI in the C++ code.
 
 Also, you cam create GUI dynamically just reading from file, using createGuiFromFile()
 NOTE: dynamically creating GUI currently not supports constants (vars with names started with "*")

 Example of GUI script:
 ------
 
 # Comments starts with "#"
 # "*" - constant changed at restart
 # "-" - output value, not saved. "-fps" in C++ code is referenced as "fps_"
 " "!" - invisible variable, saved. Changed only from C++

 PAGE screen
 COLOR 255,0,0
 int *FPS=30 1:100 1,10
 int *w=1024 1:2000 1,10
 int *h=768 1:2000 1,10
 !string openedFile=default.wav
 !float scale=1.0
 
 TAB
 COLOR 64
 float -fps=30 0:100 100,10
 # reset color to default
 RESET_COLOR

 PAGE osc
 string send_host=localhost
 int send_port=12345 1:65535 1,10
 
 PAGE all
 var -fps
 dummy
 dummy Stringlist:
 stringlist list=a [a,b,c]
 button Render
 # button - it's int variable, which is set when button is pressed, needs to set to 0 after reading

 ------


Example of generator calling:
 generateCPP("../../src/gui-script.ini", "../src/params",
                        "Parameters", "params",
                         "PRMQD");
"P" is the abbreviation of the parameters, you can use them in such a way:
  sender.setup(PRMQD send_host, PRMQD send_port);
 
*/

#include "ofMain.h"
#include "ofxKuTextGui.h"


struct ofxKuTextGuiGen {
	//Create H/CPP files from GUI script file
    //generateCPP("gui-script.ini",
    //"../../src/", "gui_generated",
    //"Parameters", "params", "PRMQD");
    //extern_var_name = "" means no extern variable for parameters
    static void generateCPP(string gui_file_in = "gui-script.ini",
                            string c_path = "../../src/",
                            string c_file_out = "gui_generated",
                            string class_name = "Parameters",
                            string extern_var_name = "params",
                            string define_prefix_name = "PRMQD");

	//Create GUI from script file
	static void createGuiFromFile(ofxKuTextGui &gui, string gui_file_in = "gui-script.ini");

	//Create GUI from list of lines
	static void createGuiFromLines(ofxKuTextGui &gui, vector<string> &lines);


protected:
	//common function which generates both CPP and dynamic GUI
	static void generate_common(bool make_cpp, bool make_gui,
		vector<string> &lines,
		ofxKuTextGui *gui,
		string c_path = "../../src/",
		string c_file_out = "gui_generated",
		string class_name = "Parameters",
		string extern_var_name = "params",
		string define_prefix_name = "PRMQD");



	static vector<string> loadFile(string fileName, bool trim = true, bool skip_comments_and_empty = true);
	static void saveFile(vector<string> &lines, string fileName);

    static void put(string lin, vector<string> &lines);
    static void insert(vector<string> &lines, vector<string> &what_to_insert);
    
    struct Pair {
        string a,b;
        Pair(string v, string delim) {
            vector<string> item = ofSplitString(v, delim);
            if (item.size()>=1) a = item[0];
            if (item.size()>=2) b = item[1];
        }
    };
    struct Name {
        string code_name, screen_name;
        bool is_const = false;
        string const_name;
        bool visible = true;
        bool editable = true;
        Name(string name) {
            screen_name = name;
            string prefix = (name.size()>=1)?name.substr(0,1):"";
            code_name = name;
            string short_name = (name.size()>=2)?name.substr(1):"";  //shorten
            is_const = false;
            if (prefix == "*") {
                code_name = "_" + short_name + "_";
                const_name = short_name;
                is_const = true;
            }
            if (prefix == "-") {
                code_name = short_name + "_";
                editable = false;
            }
            if (prefix == "!") {
                visible = false;
                editable = false;
            }
        }
    };
    static vector<string> parse_stringlist_values(string v) {
        //[a,b,c]
        vector<string> list;
        if (v.size() >= 2) {
            v = v.substr(1,v.size()-2);
            list = ofSplitString(v, ",");
        }
        return list;
    }
    static string stringlist_values(vector<string> &list) {
        string s = "";
        for (int i=0; i<list.size(); i++) {
            if (i>0) s += ",";
            s += "\"" + list[i] + "\"";
        }
        return s;
    }
};

