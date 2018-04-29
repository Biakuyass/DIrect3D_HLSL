
#include<d3d9.h>
#include<d3dx9.h>
#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTsettingsdlg.h"
#include "DXUTcamera.h"
#include"CRenderObject.h"
#include"UI.h"
#include"CBoard.h"
#include"DInput.h"

#pragma once

//win32
#pragma comment(lib,"comctl32.lib")
//d3d
#pragma comment(lib,"dxerr.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxut.lib")
#pragma comment(lib,"DXUTOpt.lib")
#pragma comment(lib,"d3dx10d.lib")
#pragma comment(lib,"dinput8.lib")



//全局变量声明
ID3DXFont*            g_pFont=NULL;
ID3DXSprite*          g_pTextSprite=NULL;
CModelViewerCamera     g_Camera;

CRenderObject          g_renderObject;
CBoard                 g_Cboard;
D3DXMATRIX             g_matWorld;

DInput                 g_input;


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


INT WINAPI WinMain(HINSTANCE hinstance,HINSTANCE,LPSTR,int)
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


	g_input.Init(NULL,hinstance,0,0);
	DXUTMainLoop();


	return DXUTGetExitCode();
}

void InitApp()
{
	UI::InitApp();
	g_Cboard.InitApp();


}

HRESULT CALLBACK OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText)
{

	HRESULT hr;
	V_RETURN(D3DXCreateFont(pd3dDevice,20,0,FW_THIN,1,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,L"宋体",&g_pFont));
	//UI
	UI::OnD3D9CreateDevice(pd3dDevice,pBackBufferSurfacfeDesc,pUserText);
	//物体创建设备
	g_renderObject.OnD3D9CreateDevice(pd3dDevice,L"Model.X",L"Test.fx",L"ColorMap.bmp");



	D3DXMatrixTranslation(&g_matWorld,0,0,0);
	D3DXMatrixTranslation(&g_matWorld,g_renderObject.GetCenter().x,g_renderObject.GetCenter().y,g_renderObject.GetCenter().z);

	//摄像机的设置
	D3DXVECTOR3 vecEye( 0.0f, 10.0f, -15.0f );
	D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );
	g_Camera.SetViewParams(&vecEye,&vecAt);
	//最大推移半径
//	g_Camera.SetRadius(g_renderObject.GetRadius()*2,g_renderObject.GetRadius()*0.5f,g_renderObject.GetRadius()*8);

	//检验用
	//wchar_t temp_Text[500];
//	swprintf_s(temp_Text,500,L"X:%f  Y:%f   Z:%f",g_renderObject.GetCenter().x,g_renderObject.GetCenter().y,g_renderObject.GetCenter().z);
	//	g_SampleUI.GetStatic(IDC_STATICTEXT)->SetText(temp_Text);
	g_Cboard.OnD3D9CreateDevice(pd3dDevice,pBackBufferSurfacfeDesc,pUserText);


	return S_OK;
}

HRESULT CALLBACK OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfaceDesc,void *pUserText)
{
	HRESULT hr;
	if(g_pFont)
		V_RETURN(g_pFont->OnResetDevice());
	V_RETURN(D3DXCreateSprite(pd3dDevice,&g_pTextSprite));


	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
	g_Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 2.0f, 4000.0f );
	g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

	//UI
	UI::OnD3D9ResetDevice(pd3dDevice,pBackBufferSurfaceDesc,pUserText);

	g_Cboard.OnD3D9ResetDevice(pd3dDevice,pBackBufferSurfaceDesc,pUserText);
	g_renderObject.OnD3D9ResetDevice(pd3dDevice,pBackBufferSurfaceDesc,pUserText);




	return S_OK;

}

void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	UI::OnFrameMove(fTime,fElapsedTime,pUserContext);
	g_Camera.FrameMove( fElapsedTime );

	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;


//	D3DXMatrixRotationY(&g_matWorld,fTime/10.0f);

	mWorld=g_matWorld * (*g_Camera.GetWorldMatrix());
	mView=*g_Camera.GetViewMatrix();
	mProj=*g_Camera.GetProjMatrix();

	D3DXMATRIX matWorldViewProj=mWorld*mView*mProj;

	g_renderObject.OnFrameMove(fTime,fElapsedTime,&mWorld,&matWorldViewProj);

	g_Cboard.OnFrameMove(fTime, fElapsedTime,pUserContext );

	g_input.GetInput();
	static float MoveX=0;
	static float MoveY=0;
	static float fScale=1.0f;
	if(g_input.getKeyDown(DIK_W))
	{
		MoveY+=0.01*fScale;
	}
	if(g_input.getKeyDown(DIK_S))
	{
		MoveY-=0.01*fScale;
	}
	if(g_input.getKeyDown(DIK_A))
	{
		MoveX-=0.01*fScale;
	}
	if(g_input.getKeyDown(DIK_D))
	{
		MoveX+=0.01*fScale;
	}

	if(g_input.getMouseDown(0))
	{
		fScale*=1.1;
	}
	if(g_input.getMouseDown(1))
	{
		fScale/=1.1;
	}
	g_Cboard.SetFloatForJulia(fScale);
	g_Cboard.SetMoveX(MoveX);
	g_Cboard.SetMoveY(MoveY);
	UI::temp_change(fScale);
}


void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	HRESULT hr;

	g_renderObject.RenderToTexture(pd3dDevice,fTime,fElapsedTime,pUserContext);



	if(UI::RenderOut(pd3dDevice,fTime,fElapsedTime,pUserContext))
	{
		return;
	}
	// Clear the render target and the zbuffer 
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );

	// Render the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		//g_renderObject.OnD3D9FrameRender(pd3dDevice,fTime,fElapsedTime,pUserContext);

	//	UI::OnD3D9FrameRender(pd3dDevice,fTime,fElapsedTime,pUserContext);
		g_Cboard.SetTexture(g_renderObject.GetTexture()); 
		g_Cboard.OnD3D9FrameRender(pd3dDevice,fTime,fElapsedTime,pUserContext);
		V( pd3dDevice->EndScene() );
	}


}


LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
						 bool* pbNoFurtherProcessing, void* pUserContext )
{

	UI::MsgProc(hWnd,uMsg,wParam,lParam,pbNoFurtherProcessing,pUserContext);

     g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );
	return 0;

}


void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
	UI::OnD3D9LostDevice(pUserContext );

	if( g_pFont )
		g_pFont->OnLostDevice();
	g_renderObject.OnD3D9LostDevice(pUserContext);
	g_Cboard.OnD3D9LostDevice(pUserContext);
}

void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	UI::OnD3D9DestroyDevice( pUserContext);
	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pTextSprite);
	g_renderObject.OnD3D9DestroyDevice(pUserContext);
	g_Cboard.OnD3D9DestroyDevice(pUserContext);
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







