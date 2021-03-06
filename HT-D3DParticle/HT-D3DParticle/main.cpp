
#include<d3d9.h>
#include<d3dx9.h>
#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTsettingsdlg.h"
#include "DXUTcamera.h"
#include"CRenderObject.h"
#include"UI.h"
#include"CBoard.h"
#include"Particle.h"
#include"UIController.h"

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
ID3DXFont*            g_pFont=NULL;
ID3DXSprite*          g_pTextSprite=NULL;
CModelViewerCamera     g_Camera;

//CRenderObject          g_renderObject;
CRenderObject          g_BillBoard;
CRenderObject          g_Ray;
CBoard                 g_Cboard;
D3DXMATRIX             g_matWorld;
D3DXMATRIX             g_matBillBoard;

Particle              g_Particle;//粒子源
Particle              g_TestParticle;

UIController          g_UIController;


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
	//g_renderObject.OnD3D9CreateDevice(pd3dDevice,L"Model.X",L"MoveANormal.fx",L"ColorMap.bmp");
	g_BillBoard.OnD3D9CreateDevice(pd3dDevice,L"Particle.X",L"BillBoard.fx",L"ColorMap.dds");
	g_Ray.OnD3D9CreateDevice(pd3dDevice,L"Particle.X",L"VerticalBillBoard.fx",L"Ray_03.tga");
	g_UIController.Init(pd3dDevice);
	g_UIController.Layout();

//	g_Particle.OnD3D9CreateDevice(pd3dDevice,L"Particle.X",L"BillBoard.fx",L"ColorMap.dds");//Ray_03.tga
//	g_TestParticle.OnD3D9CreateDevice(pd3dDevice,L"Particle.X",L"BillBoard.fx",L"ColorMap.dds");//Ray_03.tga
	//g_TestParticle.OnD3D9CreateDevice(pd3dDevice,L"Particle.X",L"VerticalBillBoard.fx",L"Ray_03.tga");//Ray_03.tga

	//g_TestParticle=Particle(g_Particle);
	g_TestParticle.SetPosition(D3DXVECTOR3(30.0,0,0));
	g_TestParticle.Setvelocity(0,0);
	g_TestParticle.SetScaleLifeTime(40,40,40);
	//g_TestParticle.SetNumberPerSecond(1);
	g_TestParticle.SetBillBoard(&g_Ray);
	g_TestParticle.SetTextureXY(1,4);
	g_TestParticle.SetReleaseState(Par_DistanceRelease);
	//g_TestParticle.movespeed=D3DXVECTOR3(-0.01f,0,0);
	g_TestParticle.SetShape(Par_Cube);
	g_TestParticle.SetCubeXYZ(0,0,0);
	g_TestParticle.SetNumberDistance(2);
	//g_TestParticle.SetAlphaLifeTime(3);


	g_Particle.SetBillBoard(&g_BillBoard);
	g_Particle.SetShape(Par_Cone);
	g_Particle.SetRadius(0);
	g_Particle.SetConeAngle(90);
	g_Particle.SetNumberPerSecond(1);
	g_Particle.SetAccelerateRangeX(-0.0005,0.0005);
	g_Particle.SetAccelerateRangeZ(-0.0005,0.0005);
	g_Particle.AddParticleSysOnBirth(g_TestParticle);

	/*g_Particle.SetBillBoard(&g_BillBoard);
	g_Particle.SetShape(Par_Sphere);
	g_Particle.SetRadius(5);
	g_Particle.SetConeAngle(30);
	g_Particle.SetNumberPerSecond(10000);*/







	/*g_TestParticle.SetPosition(D3DXVECTOR3(30.0,0,0));
	//g_TestParticle.SetReleaseDirection(D3DXVECTOR3(1.0,0,0));
	g_TestParticle.SetShape(Par_HalfCube);
	g_TestParticle.SetAlphaLifeTime(1.0);

	g_Particle.SetReleaseDirection(D3DXVECTOR3(0,0,1.0f));
	g_Particle.SetShape(Par_HemiSphere);*/
	



	D3DXMatrixTranslation(&g_matWorld,0,0,0);
	D3DXMatrixTranslation(&g_matBillBoard,5,6,-5);
	//D3DXMatrixTranslation(&g_matWorld,g_renderObject.GetCenter().x,g_renderObject.GetCenter().y,g_renderObject.GetCenter().z);

	//摄像机的设置
	D3DXVECTOR3 vecEye( 0.0f, 5.0f, -15.0f );
	D3DXVECTOR3 vecAt ( 0.0f, 0.0f, 0.0f );
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
	//g_renderObject.OnD3D9ResetDevice(pd3dDevice,pBackBufferSurfaceDesc,pUserText);
	g_BillBoard.OnD3D9ResetDevice(pd3dDevice,pBackBufferSurfaceDesc,pUserText);
	g_Ray.OnD3D9ResetDevice(pd3dDevice,pBackBufferSurfaceDesc,pUserText);
	//g_Particle.OnD3D9ResetDevice(pd3dDevice,pBackBufferSurfaceDesc,pUserText);
	//g_TestParticle.OnD3D9ResetDevice(pd3dDevice,pBackBufferSurfaceDesc,pUserText);


	return S_OK;

}

void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	/*static bool onceflag=false;
	if(fTime>5 && !onceflag)
	{
		onceflag=true;
		g_Particle.SetMaxnumber(10000);
		g_Particle.SetNumberPerSecond(1000);
	//	g_Particle.SetMaxnumber(1);
	//	g_Particle.SetReleaseState(DistanceRelease);
	}*/


	UI::OnFrameMove(fTime,fElapsedTime,pUserContext);
	g_Camera.FrameMove( fElapsedTime );

	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	D3DXMATRIX mBillWorld;

	D3DXVECTOR3 vtemp_Eye=*g_Camera.GetEyePt();
	D3DXVECTOR4 vEye(vtemp_Eye.x,vtemp_Eye.y,vtemp_Eye.z,1.0f);


//	D3DXMatrixRotationY(&g_matWorld,fTime/10.0f);

	mWorld=g_matWorld ;
	mBillWorld=g_matBillBoard * (*g_Camera.GetWorldMatrix());
	mView=*g_Camera.GetViewMatrix();
	mProj=*g_Camera.GetProjMatrix();

	D3DXMATRIX matWorldViewProj=mWorld*mView*mProj;
	D3DXMATRIX matBillBoardWVP=mBillWorld*mView*mProj;

	D3DXVECTOR4 vBillBoardPosition(5,6,-5,1.0f);

	//g_renderObject.OnFrameMove(fTime,fElapsedTime,&mWorld,&matWorldViewProj,&vEye,&vBillBoardPosition);
	//g_BillBoard.OnFrameMove(fTime,fElapsedTime,&mBillWorld,&matBillBoardWVP,&vEye,&vBillBoardPosition);

	g_Particle.OnFrameMove(fTime,fElapsedTime,g_Camera.GetWorldMatrix(),g_Camera.GetViewMatrix(),g_Camera.GetProjMatrix(),&vEye);
	//g_TestParticle.OnFrameMove(fTime,fElapsedTime,g_Camera.GetWorldMatrix(),g_Camera.GetViewMatrix(),g_Camera.GetProjMatrix(),&vEye);

	g_Cboard.OnFrameMove(fTime, fElapsedTime,pUserContext );

}


void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	HRESULT hr;


	// Clear the render target and the zbuffer 
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255, 50,50,50 ), 1.0f, 0 ) );

	// Render the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
	//	g_renderObject.OnD3D9FrameRender(pd3dDevice,fTime,fElapsedTime,pUserContext);
		//g_BillBoard.OnD3D9FrameRender(pd3dDevice,fTime,fElapsedTime,pUserContext);
		//UI::OnD3D9FrameRender(pd3dDevice,fTime,fElapsedTime,pUserContext);
		g_Cboard.OnD3D9FrameRender(pd3dDevice,fTime,fElapsedTime,pUserContext);
		g_Particle.OnD3D9FrameRender(pd3dDevice,fTime,fElapsedTime,pUserContext);
//		g_TestParticle.OnD3D9FrameRender(pd3dDevice,fTime,fElapsedTime,pUserContext);


		V( pd3dDevice->EndScene() );
	}

	g_UIController.Render();
}


LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
						 bool* pbNoFurtherProcessing, void* pUserContext )
{

	UI::MsgProc(hWnd,uMsg,wParam,lParam,pbNoFurtherProcessing,pUserContext);

     g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

	 UIController::InjectInputs( hWnd, uMsg, wParam, lParam);

	return 0;

}


void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
	UI::OnD3D9LostDevice(pUserContext );

	if( g_pFont )
		g_pFont->OnLostDevice();
	//g_renderObject.OnD3D9LostDevice(pUserContext);
	g_BillBoard.OnD3D9LostDevice(pUserContext);
    g_Ray.OnD3D9LostDevice(pUserContext);
	g_Cboard.OnD3D9LostDevice(pUserContext);

//	g_UIController.Release();
	//g_Particle.OnD3D9LostDevice(pUserContext);
	//g_TestParticle.OnD3D9LostDevice(pUserContext);
}

void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	UI::OnD3D9DestroyDevice( pUserContext);
	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pTextSprite);
	//g_renderObject.OnD3D9DestroyDevice(pUserContext);
	g_BillBoard.OnD3D9DestroyDevice(pUserContext);
	g_Ray.OnD3D9DestroyDevice(pUserContext);


	g_UIController.Release();
	g_Cboard.OnD3D9DestroyDevice(pUserContext);

	//g_Particle.OnD3D9DestroyDevice(pUserContext);
	//g_TestParticle.OnD3D9DestroyDevice(pUserContext);
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







