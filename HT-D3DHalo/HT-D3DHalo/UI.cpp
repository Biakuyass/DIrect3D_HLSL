#include"UI.h"
void UI::InitApp()
{
	m_SampleUI.Init(&m_DialogResourceManager);
	m_SettingDlg.Init(&m_DialogResourceManager);


	m_SampleUI.SetFont(1,L"宋体",20,20);


	int Y=20;
	//设置响应函数
	m_SampleUI.SetCallback(OnGUIEvent);
	/*m_SampleUI.AddButton(IDC_BUTTON1,L"Test Button 1",20,Y,80,30);
	m_SampleUI.AddButton(IDC_BUTTON2,L"Test Button 2",20,Y+=30,80,60);

	m_SampleUI.AddStatic(IDC_STATICTEXT,L"STATIC TEXT",20,Y+=50,400,100);
	m_SampleUI.GetStatic(IDC_STATICTEXT)->SetTextColor(D3DCOLOR_ARGB(100,255,0,0,0));
	m_SampleUI.GetControl(IDC_STATICTEXT)->GetElement(0)->dwTextFormat=DT_LEFT|DT_WORDBREAK|DT_TOP;
	m_SampleUI.GetControl(IDC_STATICTEXT)->GetElement(0)->SetFont(1);


	m_SampleUI.AddEditBox(IDC_EDITBOX,L"Not changed",0,0,200,100);
	m_SampleUI.GetControl(IDC_EDITBOX)->GetElement(0)->SetFont(1);*/
}

HRESULT  UI::OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText)
{
	HRESULT hr;
	V_RETURN(m_DialogResourceManager.OnD3D9CreateDevice(pd3dDevice));
	V_RETURN(m_SettingDlg.OnD3D9CreateDevice(pd3dDevice));

}

HRESULT  UI::OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText)
{
	HRESULT hr;

	V_RETURN( m_DialogResourceManager.OnD3D9ResetDevice() );
	V_RETURN(m_SettingDlg.OnD3D9ResetDevice());

	m_SampleUI.SetLocation(0,0);
	m_SampleUI.SetSize(pBackBufferSurfacfeDesc->Width,pBackBufferSurfacfeDesc->Height);
	/*m_SampleUI.GetControl(IDC_BUTTON1)->SetLocation(20,pBackBufferSurfacfeDesc->Height-200);
	m_SampleUI.GetControl(IDC_BUTTON2)->SetLocation(20,pBackBufferSurfacfeDesc->Height-280);
	m_SampleUI.GetControl(IDC_STATICTEXT)->SetLocation(50,pBackBufferSurfacfeDesc->Height-100);
	m_SampleUI.GetControl(IDC_EDITBOX)->SetLocation(50,pBackBufferSurfacfeDesc->Height-400);*/
}

void  UI::OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{

}

void  UI::OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext)
{
	HRESULT hr;
    V(m_SampleUI.OnRender(fElapsedTime));
}

LRESULT UI::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext)
{
	*pbNoFurtherProcessing = m_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;

	*pbNoFurtherProcessing=m_SampleUI.MsgProc(hWnd,uMsg,wParam,lParam);
	if(*pbNoFurtherProcessing)
		return 0;

	if(m_SettingDlg.IsActive())
	{
		m_SettingDlg.MsgProc(hWnd,uMsg,wParam,lParam);
		return 0;
	}
	return 0;
}

void CALLBACK UI::OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{

	WCHAR wtext[500];

	switch (nControlID)
	{
/*	case IDC_BUTTON1:
		m_SampleUI.GetStatic(IDC_STATICTEXT)->SetText(L"Click One!");
		break;
	case IDC_BUTTON2:
		m_SettingDlg.SetActive( !m_SettingDlg.IsActive() );
		break;
	case IDC_EDITBOX:
		switch (nEvent)
		{
			//输入回车
		case EVENT_EDITBOX_STRING:
			m_SampleUI.GetStatic(IDC_STATICTEXT)->SetText(L"FDS");
			break;
		case EVENT_EDITBOX_CHANGE:
			swprintf_s(wtext,500,L"Have changed:%s",((CDXUTEditBox*)pControl)->GetText());
			m_SampleUI.GetStatic(IDC_STATICTEXT)->SetText(wtext);
			break;
		default:
			break;
		}
		break;
	default:
		break;*/
	}

}

void  UI::OnD3D9LostDevice(void* pUserContext)
{
	m_DialogResourceManager.OnD3D9LostDevice();
	m_SettingDlg.OnD3D9LostDevice();
}

void UI::OnD3D9DestroyDevice(void* pUserContext)
{
	m_DialogResourceManager.OnD3D9DestroyDevice();
	m_SettingDlg.OnD3D9DestroyDevice();
}

bool UI::RenderOut(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext)
{
	if(m_SettingDlg.IsActive())
	{

		m_SettingDlg.OnRender(fElapsedTime);
		return true;

	}
	return false;
}

CDXUTDialogResourceManager UI::m_DialogResourceManager;

CD3DSettingsDlg UI::m_SettingDlg;

CDXUTDialog UI::m_SampleUI;




