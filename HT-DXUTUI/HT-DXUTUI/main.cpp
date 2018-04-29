#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTsettingsdlg.h"



#pragma once

//win32
#pragma comment(lib,"comctl32.lib")
//d3d
#pragma comment(lib,"dxerr.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxut.lib")
#pragma comment(lib,"DXUTOpt.lib")
#pragma comment(lib,"d3dx10d.lib")


#define IDC_BUTTON1       1
#define IDC_BUTTON2       2
#define IDC_STATICTEXT    3
#define IDC_EDITBOX       4

//全局变量声明
ID3DXFont * g_pFont=NULL;
ID3DXSprite *g_pTextSprite=NULL;
CDXUTDialog g_SampleUI;
CD3DSettingsDlg g_SettingDlg;
CDXUTDialogResourceManager g_DialogResourceManager;


//函数
void InitApp();
HRESULT CALLBACK OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText);
HRESULT CALLBACK OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText);
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
						 bool* pbNoFurtherProcessing, void* pUserContext );
void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
void CALLBACK OnD3D9LostDevice( void* pUserContext );
void CALLBACK OnD3D9DestroyDevice( void* pUserContext );
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
									 bool bWindowed, void* pUserContext );
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );


INT WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{


	DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
	DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
	DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
    DXUTSetCallbackFrameMove( OnFrameMove );
	DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
	DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
	DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
	DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
	DXUTSetCallbackMsgProc( MsgProc );



	InitApp();

	DXUTInit( true, true ); 
	DXUTSetCursorSettings( true, true );
	DXUTCreateWindow( L"EmptyProject");
	DXUTCreateDevice( true, 640, 480 );

	DXUTMainLoop();


	return DXUTGetExitCode();
}

void InitApp()
{

	g_SampleUI.Init(&g_DialogResourceManager);
	g_SettingDlg.Init(&g_DialogResourceManager);


	g_SampleUI.SetFont(1,L"宋体",20,20);


	int Y=20;
	//设置响应函数
	g_SampleUI.SetCallback(OnGUIEvent);
	g_SampleUI.AddButton(IDC_BUTTON1,L"Test Button 1",20,Y,80,30);
	g_SampleUI.AddButton(IDC_BUTTON2,L"Test Button 2",20,Y+=30,80,60);

	g_SampleUI.AddStatic(IDC_STATICTEXT,L"STATIC TEXT",20,Y+=50,200,100);
	g_SampleUI.GetStatic(IDC_STATICTEXT)->SetTextColor(D3DCOLOR_ARGB(100,255,0,0,0));
	g_SampleUI.GetControl(IDC_STATICTEXT)->GetElement(0)->dwTextFormat=DT_LEFT|DT_WORDBREAK|DT_TOP;
	g_SampleUI.GetControl(IDC_STATICTEXT)->GetElement(0)->SetFont(1);


	g_SampleUI.AddEditBox(IDC_EDITBOX,L"Not changed",0,0,200,100);
	g_SampleUI.GetControl(IDC_EDITBOX)->GetElement(0)->SetFont(1);





}

HRESULT CALLBACK OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText)
{

	HRESULT hr;
	V_RETURN(D3DXCreateFont(pd3dDevice,20,0,FW_THIN,1,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,L"宋体",&g_pFont));

	V_RETURN(g_DialogResourceManager.OnD3D9CreateDevice(pd3dDevice));
	V_RETURN(g_SettingDlg.OnD3D9CreateDevice(pd3dDevice));

	return S_OK;
}

HRESULT CALLBACK OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText)
{
	HRESULT hr;
	if(g_pFont)
		V_RETURN(g_pFont->OnResetDevice());
	V_RETURN(D3DXCreateSprite(pd3dDevice,&g_pTextSprite));


	V_RETURN( g_DialogResourceManager.OnD3D9ResetDevice() );
	V_RETURN(g_SettingDlg.OnD3D9ResetDevice());

	g_SampleUI.SetLocation(0,0);
	g_SampleUI.SetSize(pBackBufferSurfacfeDesc->Width,pBackBufferSurfacfeDesc->Height);
	g_SampleUI.GetControl(IDC_BUTTON1)->SetLocation(20,pBackBufferSurfacfeDesc->Height-200);
	g_SampleUI.GetControl(IDC_BUTTON2)->SetLocation(20,pBackBufferSurfacfeDesc->Height-280);
	g_SampleUI.GetControl(IDC_STATICTEXT)->SetLocation(50,pBackBufferSurfacfeDesc->Height-100);
	g_SampleUI.GetControl(IDC_EDITBOX)->SetLocation(50,pBackBufferSurfacfeDesc->Height-400);




	return S_OK;

}

void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
}


void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	HRESULT hr;
	if(g_SettingDlg.IsActive())
	{
	
		g_SettingDlg.OnRender(fElapsedTime);
			return;

	}
	// Clear the render target and the zbuffer 
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );

	// Render the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{


		V(g_SampleUI.OnRender(fElapsedTime));

		V( pd3dDevice->EndScene() );
	}


}


LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
						 bool* pbNoFurtherProcessing, void* pUserContext )
{

	*pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;

	*pbNoFurtherProcessing=g_SampleUI.MsgProc(hWnd,uMsg,wParam,lParam);
	if(*pbNoFurtherProcessing)
		return 0;

	if(g_SettingDlg.IsActive())
	{
		g_SettingDlg.MsgProc(hWnd,uMsg,wParam,lParam);
		return 0;
	}
	return 0;

}

void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{

	WCHAR wtext[500];

	switch (nControlID)
	{
	case IDC_BUTTON1:
		g_SampleUI.GetStatic(IDC_STATICTEXT)->SetText(L"Click One!");
		break;
	case IDC_BUTTON2:
	  g_SettingDlg.SetActive( !g_SettingDlg.IsActive() );
		break;
	case IDC_EDITBOX:
		switch (nEvent)
		{
			//输入回车
		case EVENT_EDITBOX_STRING:
					g_SampleUI.GetStatic(IDC_STATICTEXT)->SetText(L"FDS");
					break;
		case EVENT_EDITBOX_CHANGE:
			swprintf_s(wtext,500,L"Have changed:%s",((CDXUTEditBox*)pControl)->GetText());
			g_SampleUI.GetStatic(IDC_STATICTEXT)->SetText(wtext);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}


}

void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
	g_DialogResourceManager.OnD3D9LostDevice();
	g_SettingDlg.OnD3D9LostDevice();

	if( g_pFont )
		g_pFont->OnLostDevice();
}

void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	g_DialogResourceManager.OnD3D9DestroyDevice();
	g_SettingDlg.OnD3D9DestroyDevice();
	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pTextSprite);

}


bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
									 bool bWindowed, void* pUserContext )
{
	// Typically want to skip back buffer formats that don't support alpha blending
	IDirect3D9* pD3D = DXUTGetD3D9Object();
	if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
		AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
		D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
		return false;

	return true;
}


bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	return true;
}







