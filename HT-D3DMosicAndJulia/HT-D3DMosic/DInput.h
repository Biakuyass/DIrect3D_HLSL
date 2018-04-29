#pragma once


#include<windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include<time.h>
#include<tchar.h> //用于swprintf
#include<dinput.h>

class DInput
{
private:
	LPDIRECTINPUT8 m_pDirectInput;
	LPDIRECTINPUTDEVICE8 m_pKeyBoardinput;
	LPDIRECTINPUTDEVICE8 m_pMouseinput;

	char m_KeyBoardBuffer[256];
	DIMOUSESTATE m_MouseState;

public:
	DInput();
	~DInput();

	HRESULT Init(HWND hwnd,HINSTANCE hInstance,DWORD keyboardCoopFlags,DWORD mouseCoopFlags);
	bool getKeyDown(int iKey);
	bool getMouseDown(int button);
	float MouseDX();
	float MouseDY();
	float MouseDZ();

	void GetInput();//用于刷新输入信息的函数


};