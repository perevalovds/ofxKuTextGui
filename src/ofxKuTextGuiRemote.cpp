#include "ofxKuTextGuiRemote.h"


//------------------------------------------------------------------------
void ofxKuTextGuiRemoteServer::setup(int in_port, ofBaseApp *app, ofxKuTextGui *gui) {
	cout << "Starting OSC (ofxKuTextGuiRemoteServer) at port " << in_port << endl;
	receiver_.setup(in_port);
	setup(app, gui);
}

//------------------------------------------------------------------------
//Setup without starting OSC - call processMessage for processing messages
void ofxKuTextGuiRemoteServer::setup(ofBaseApp *app, ofxKuTextGui *gui) {
	app_ = app;
	gui_ = gui;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteServer::update() {
	unparsed_messages_.clear();

    while (receiver_.hasWaitingMessages()) {
        ofxOscMessage m;
#ifdef OFXKUTEXTGUI_074
		receiver_.getNextMessage(&m);
#else
		receiver_.getNextMessage(m);
#endif
        bool parsed = processMessage(m);
		if (!parsed) {
			unparsed_messages_.push_back(m);
		}
    }
    
}

//------------------------------------------------------------------------
vector<ofxOscMessage> &ofxKuTextGuiRemoteServer::unparsed_messages() {
	return unparsed_messages_;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteServer::exit() {

}

//------------------------------------------------------------------------
bool ofxKuTextGuiRemoteServer::processMessage(ofxOscMessage &m) {
    //guiRequest(int) - argument is port for returning the result
    //keyPressed(int)
    //keyReleased(int)
    //mousePressed(float,float,int)
    //mouseDragged(float,float,int)
    //mouseReleased(float,float,int)

    string addr = m.getAddress();
    if (addr == "/guiRequest") {
        int port = m.getArgAsInt(0);
        answerGuiRequest( m.getRemoteIp(), port );
		return true;
		//gui_->set_editing_strings(false);	//disable string editing in order not to loose the focus
    }
    if (app_) {
        if (addr == "/keyPressed") {
            app_->keyPressed(m.getArgAsInt(0));
			return true;
        }
        if (addr == "/keyReleased") {
            app_->keyReleased(m.getArgAsInt(0));
			return true;
        }
        if (addr == "/mousePressed") {
            app_->mousePressed(m.getArgAsFloat(0) * ofGetWidth(),
                               m.getArgAsFloat(1) * ofGetHeight(),
                               m.getArgAsInt(0));
			return true;
        }
        if (addr == "/mouseDragged") {
            app_->mouseDragged(m.getArgAsFloat(0) * ofGetWidth(),
                               m.getArgAsFloat(1) * ofGetHeight(),
                               m.getArgAsInt(0));
			return true;
        }
        if (addr == "/mouseReleased") {
            app_->mouseReleased(m.getArgAsFloat(0) * ofGetWidth(),
                               m.getArgAsFloat(1) * ofGetHeight(),
                               m.getArgAsInt(0));
			return true;
        }
    }
	return false;
}


//------------------------------------------------------------------------
void ofxKuTextGuiRemoteServer::answerGuiRequest(string ip, int port) {
    ofxOscSender sender;
    sender.setup(ip, port);
    ofxOscMessage m;
    m.setAddress("/guiRequestReturn");
    if (gui_) {
        m.addStringArg(gui_->drawToString());
    }
    sender.sendMessage(m);
    
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Connection to Gui, draws it and allows to send mouse and keyboard transmitting
void ofxKuTextGuiRemoteClient::setup(string out_host, int out_port, int in_port) {
    in_port_ = in_port;
    time0_ = ofGetElapsedTimef();
    sender_.setup(out_host, out_port);
    receiver_.setup(in_port);
    
    fast_time = 0.1;
    slow_time = 0.5;
    need_fast_update_ = true;

	gui_string_ = "CONNECTING=0=0";
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::update() {
    float time = ofGetElapsedTimef();
    float delta = time-time0_;
    if ( (need_fast_update_ && delta >= fast_time)
        || delta >= slow_time ) {
        askGuiRequest();
        time0_ = time;
        need_fast_update_ = false;
    }
    while (receiver_.hasWaitingMessages()) {
        ofxOscMessage m;
#ifdef OFXKUTEXTGUI_074
		receiver_.getNextMessage(&m);
#else
		receiver_.getNextMessage(m);
#endif
        processMessage(m);
    }
}

//------------------------------------------------------------------------
string ofxKuTextGuiRemoteClient::get_var_value(string var_name) {	//returns var value if it's visible
	var_name = " " + var_name;
	vector<string> tabs = ofSplitString(gui_string_, ";");
	for (int t = 0; t < tabs.size(); t++) {
		vector<string> tab = ofSplitString(tabs[t], ",");
		for (int i = 0; i < tab.size(); i++) {
			vector<string> var = ofSplitString(tab[i], "=");
			if (var.size() >= 3) {
				string name = var[0];
				string &value = var[1];
				//float valueNormalized = ofToFloat(var[2]);
				bool selected = (!name.empty() && name[0] == '>');
				if (selected) name[0] = ' ';
				if (name == var_name) {
					return value;
				}
			}
		}
	}
	return "";
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::exit() {
    
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::draw(float x, float y) {
    gui_.drawFromString(gui_string_, x, y);
}


//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::keyPressed(int key) {
    ofxOscMessage m;
    m.setAddress("/keyPressed");
    m.addIntArg(key);
    sender_.sendMessage(m);
    need_fast_update_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::keyReleased(int key) {
    ofxOscMessage m;
    m.setAddress("/keyReleased");
    m.addIntArg(key);
    sender_.sendMessage(m);
    need_fast_update_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::mousePressed(int x, int y, int button) {
    ofxOscMessage m;
    m.setAddress("/mousePressed");
    m.addFloatArg(float(x) / ofGetWidth());
    m.addFloatArg(float(y) / ofGetHeight());
    m.addIntArg(button);
    sender_.sendMessage(m);
    need_fast_update_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::mouseDragged(int x, int y, int button) {
    ofxOscMessage m;
    m.setAddress("/mouseDragged");
    m.addFloatArg(float(x) / ofGetWidth());
    m.addFloatArg(float(y) / ofGetHeight());
    m.addIntArg(button);
    sender_.sendMessage(m);
    need_fast_update_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::mouseReleased(int x, int y, int button) {
    ofxOscMessage m;
    m.setAddress("/mouseReleased");
    m.addFloatArg(float(x) / ofGetWidth());
    m.addFloatArg(float(y) / ofGetHeight());
    m.addIntArg(button);
    sender_.sendMessage(m);
    need_fast_update_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::askGuiRequest() {
    ofxOscMessage m;
    m.setAddress("/guiRequest");
    m.addIntArg(in_port_);
    sender_.sendMessage(m);
    need_fast_update_ = true;
}

//------------------------------------------------------------------------
void ofxKuTextGuiRemoteClient::processMessage(ofxOscMessage &m) {
    if (m.getAddress() == "/guiRequestReturn") {
        gui_string_ = m.getArgAsString(0);
    }
}

//------------------------------------------------------------------------







