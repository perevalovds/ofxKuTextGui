#include "ofxKuUiString.h"

// For _T
#include <tchar.h>


//------------------------------------------------------------------------
void KuUiString::draw(const KuUiDrawData& dd, const KuUiDrawComponentData& dc) {
	drawSlider(dd, dc, false, false);
}

//------------------------------------------------------------------------
#ifdef TARGET_WIN32
LRESULT CALLBACK KuUiString_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//switch(msg)
	//{
	//    case WM_CLOSE:
	//        DestroyWindow(hwnd);
	//    break;
	//    case WM_DESTROY:
	//        PostQuitMessage(0);
	//    break;
	//    default:
	return DefWindowProc(hwnd, msg, wParam, lParam);
	//}
}
#endif

// Stuff for implementing KuUiString::textBoxDialog
// depending on UNICODE settings in Windows
#ifdef UNICODE

inline std::wstring KuUiString_stringToWstringWindows(const std::string& str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
	return wstr;
}

typedef std::wstring KuUiString_String_Type;
#define KuUiString_WIDEN_STRING(str) (KuUiString_stringToWstringWindows(str))
#else
typedef std::string KuUiString_String_Type;
#define KuUiString_WIDEN_STRING(str) (str)
#endif


string KuUiString::textBoxDialog(string question, string text) {
// Improved ofSystemTextBoxDialog for Windows:
#ifndef TARGET_WIN32
	return ofSystemTextBoxDialog(question, text);
#else
		// we need to convert error message to a wide char message.
		// first, figure out the length and allocate a wchar_t at that length + 1 (the +1 is for a terminating character)

	WNDCLASSEX wc;
	MSG Msg;

	HWND hw = WindowFromDC(wglGetCurrentDC());

	// If UNICODE is defined, WIDEN_STRING(myString) returns a wchar_t*,
	// otherwise it returns a const char*.
	KuUiString_String_Type classNameStr = KuUiString_WIDEN_STRING("KuUiString_TextBoxDialog_WindowClass");
	KuUiString_String_Type questionStr = KuUiString_WIDEN_STRING(question);
	KuUiString_String_Type textStr = KuUiString_WIDEN_STRING(text);

	//Step 1: Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = KuUiString_WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(0);
	wc.lpszClassName = classNameStr.c_str();
	wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
	wc.lpszMenuName = nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	if (!RegisterClassEx(&wc)) {
		DWORD err = GetLastError();
		if ((err == ERROR_CLASS_ALREADY_EXISTS)) {
			; // we are ok
			// http://stackoverflow.com/questions/5791996/re-registering-user-defined-window-class-c
		}
		else {
			MessageBox(nullptr, _T("Window Registration Failed!"), _T("Error!"),
				MB_ICONEXCLAMATION | MB_OK);
			return text;
		}
	}

	int w = 300;
	int h = 140;

	int x = ofGetMouseX();
	int y = ofGetMouseY();
	



	HWND dialog = CreateWindowEx(WS_EX_DLGMODALFRAME,
		classNameStr.c_str(),
		questionStr.c_str(),
		WS_POPUP | WS_CAPTION | DS_MODALFRAME | WS_SYSMENU,
		x, y, 300, 140,
		hw, nullptr, GetModuleHandle(0), nullptr);

	if (dialog == nullptr)
	{

		MessageBox(nullptr, _T("Window Creation Failed!"), _T("Error!"),
			MB_ICONEXCLAMATION | MB_OK);
		return text;

	}

	//EnableWindow(WindowFromDC(wglGetCurrentDC()), FALSE);
	HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), textStr.c_str(),
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		10, 10, 210, 40, dialog, (HMENU)101, GetModuleHandle(nullptr), nullptr);


	HWND okButton = CreateWindowEx(WS_EX_CLIENTEDGE, _T("BUTTON"), _T("OK"),
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		10, 60, 60, 30, dialog, (HMENU)IDOK, GetModuleHandle(nullptr), nullptr);

	HWND cancelButton = CreateWindowEx(WS_EX_CLIENTEDGE, _T("BUTTON"), _T("Cancel"),
		WS_CHILD | WS_VISIBLE,
		80, 60, 60, 30, dialog, (HMENU)IDCANCEL, GetModuleHandle(nullptr), nullptr);

	SetFocus(hEdit);

	ShowWindow(dialog, SW_SHOWNORMAL);
	bool bFirstEmpty = true;
	while (true) {
		if (!PeekMessageW(&Msg, 0, 0, 0, PM_REMOVE)) {
			if (bFirstEmpty) {
				// ShowWindow the first time the queue goes empty
				ShowWindow(dialog, SW_SHOWNORMAL);
				bFirstEmpty = FALSE;
			}
			if (!(GetWindowLongW(dialog, GWL_STYLE) & DS_NOIDLEMSG)) {
				// No message present -> send ENTERIDLE and wait
				SendMessageW(WindowFromDC(wglGetCurrentDC()), WM_ENTERIDLE, MSGF_DIALOGBOX, (LPARAM)dialog);
			}
			GetMessageW(&Msg, 0, 0, 0);
		}

		if (Msg.message == WM_QUIT) {
			PostQuitMessage(Msg.wParam);
			if (!IsWindow(dialog)) {
				SetFocus(hw);
				return text;
			}
			break;
		}

		if (!IsWindow(dialog)) {
			SetFocus(hw);
			return text;
		}

		TranslateMessage(&Msg);
		DispatchMessageW(&Msg);

		if ((Msg.hwnd == okButton && Msg.message == WM_LBUTTONUP) || (Msg.message == WM_KEYUP && Msg.wParam == 13)) {
			break;
		}
		else if ((Msg.hwnd == cancelButton && Msg.message == WM_LBUTTONUP) || (Msg.message == WM_KEYUP && Msg.wParam == 27)) {
			DestroyWindow(dialog);
			SetFocus(hw);
			return text;
		}

		if (!IsWindow(dialog)) {
			SetFocus(hw);
			return text;
		}

		if (bFirstEmpty && Msg.message == WM_TIMER) {
			SetFocus(hw);
			bFirstEmpty = FALSE;
		}
	}

	char buf[16384];
	GetDlgItemTextA(dialog, 101, buf, 16384);
	text = buf;

	DestroyWindow(dialog);

	SetFocus(hw);

	return text;
#endif
}

#undef KuUiString_WIDEN_STRING

//------------------------------------------------------------------------
