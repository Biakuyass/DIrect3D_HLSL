
#include<d3d9.h>
#include<d3dx9.h>
#pragma once;

#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTsettingsdlg.h"


//控件序号部分
#define IDC_BUTTON1       1
#define IDC_BUTTON2       2
#define IDC_STATICTEXT    3
#define IDC_EDITBOX       4


class UI
{
	static CDXUTDialog           m_SampleUI;
	static CD3DSettingsDlg       m_SettingDlg;
	static CDXUTDialogResourceManager m_DialogResourceManager;

public:
	static void InitApp();
	static HRESULT OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText);
	static HRESULT OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText);
	static void OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
	static void OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
	static LRESULT  MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,bool* pbNoFurtherProcessing, void* pUserContext );
	static void  CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
	static void  OnD3D9LostDevice( void* pUserContext );
	static void OnD3D9DestroyDevice( void* pUserContext );
	//用来显示不用渲染场景时的UI，直接跳出
	static bool RenderOut(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext);
};


