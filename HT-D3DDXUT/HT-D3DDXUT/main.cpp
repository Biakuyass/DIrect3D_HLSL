#include "DXUT.h"
#include "DXUTgui.h"


#pragma once

//win32
#pragma comment(lib,"comctl32.lib")
//d3d
#pragma comment(lib,"dxerr.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxut.lib")
#pragma comment(lib,"DXUTOpt.lib")
#pragma comment(lib,"d3dx10d.lib")


//全局变量声明
ID3DXFont * g_pFont=NULL;
ID3DXSprite *g_pTextSprite=NULL;
CDXUTDialog g_SampleUI;


//函数
HRESULT CALLBACK OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText)
{

	HRESULT hr;
	V_RETURN(D3DXCreateFont(pd3dDevice,20,0,FW_THIN,1,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,L"宋体",&g_pFont));



	return S_OK;
}

HRESULT CALLBACK OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText)
{
	HRESULT hr;
	if(g_pFont)
		V_RETURN(g_pFont->OnResetDevice());
	V_RETURN(D3DXCreateSprite(pd3dDevice,&g_pTextSprite));

	return S_OK;

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

void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
}


void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	HRESULT hr;

	// Clear the render target and the zbuffer 
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );

	// Render the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		V( pd3dDevice->EndScene() );
	}
}


LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
						 bool* pbNoFurtherProcessing, void* pUserContext )
{
	return 0;
}

void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
}

void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pTextSprite);

}

void InitApp()
{

}


INT WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{


	DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
	DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
	DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
	DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
	DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
	DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
	DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackFrameMove( OnFrameMove );


	InitApp();

	DXUTInit( true, true ); 
	DXUTSetCursorSettings( true, true );
	DXUTCreateWindow( L"EmptyProject" );
	DXUTCreateDevice( true, 640, 480 );

	DXUTMainLoop();


	return DXUTGetExitCode();
}





