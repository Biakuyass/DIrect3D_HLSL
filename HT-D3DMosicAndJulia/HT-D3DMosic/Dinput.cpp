#include"Dinput.h"

#define SAFE_RELEASE(p) {if(p){(p)->Release(); (p)=NULL;}}

DInput::DInput():m_pDirectInput(NULL),m_pKeyBoardinput(NULL),m_pMouseinput(NULL)
{
	ZeroMemory(m_KeyBoardBuffer,sizeof(char)*256);
	ZeroMemory(&m_MouseState,sizeof(m_MouseState));
}

DInput::~DInput()
{
	if(m_pKeyBoardinput!=NULL)
		m_pKeyBoardinput->Unacquire();
	if(m_pMouseinput!=NULL)
		m_pMouseinput->Unacquire();
	SAFE_RELEASE(m_pDirectInput);
	SAFE_RELEASE(m_pKeyBoardinput);
	SAFE_RELEASE(m_pMouseinput);

}

HRESULT DInput::Init(HWND hwnd,HINSTANCE hInstance,DWORD keyboardCoopFlags,DWORD mouseCoopFlags)
{
	if(FAILED(DirectInput8Create(hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&m_pDirectInput,NULL)))
		return E_FAIL;

	m_pDirectInput->CreateDevice(GUID_SysKeyboard,&m_pKeyBoardinput,NULL);
	m_pDirectInput->CreateDevice(GUID_SysMouse,&m_pMouseinput,NULL);

	//设置格式和协作级别
	m_pKeyBoardinput->SetDataFormat(&c_dfDIKeyboard);
	m_pMouseinput->SetDataFormat(&c_dfDIMouse);


	m_pKeyBoardinput->SetCooperativeLevel(hwnd,keyboardCoopFlags);
	m_pMouseinput->SetCooperativeLevel(hwnd,mouseCoopFlags);

//	m_pKeyBoardinput->Poll();
	m_pKeyBoardinput->Acquire();

//	m_pMouseinput->Poll();
	m_pMouseinput->Acquire();
}

void DInput::GetInput()
{
	ZeroMemory(m_KeyBoardBuffer,sizeof(char)*256);
	ZeroMemory(&m_MouseState,sizeof(m_MouseState));

	m_pKeyBoardinput->Acquire();
	m_pKeyBoardinput->GetDeviceState(sizeof(m_KeyBoardBuffer),(LPVOID)m_KeyBoardBuffer);


	m_pMouseinput->Acquire();
	m_pMouseinput->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&m_MouseState);
}

bool DInput::getKeyDown(int iKey)
{
	if(m_KeyBoardBuffer[iKey] & 0x80)
	{
		return true;
	}
	else return false;
}

bool DInput::getMouseDown(int button)
{
	if(m_MouseState.rgbButtons[button] & 0x80)
	{
		return true;
	}
	else return false;
	
}

float DInput::MouseDX()
{
	return (float)m_MouseState.lX;
}
float DInput::MouseDY()
{
	return (float)m_MouseState.lY;
}

float DInput::MouseDZ()
{
	return (float)m_MouseState.lZ;
}


