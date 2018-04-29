#pragma once;

#include<d3d9.h>
#include<d3dx9.h>
#include "DXUT.h"
#include "DXUTcamera.h"

#pragma once
class CRenderObject
{
	LPD3DXEFFECT               m_Effect;

	D3DXVECTOR3                   m_vCenter; //中心
	FLOAT                         m_fObjectRadius;//最大包围球半径，用来设置摄像机的推移半径的


	LPD3DXMESH                 m_pMesh;
	LPDIRECT3DTEXTURE9         m_pTexture;




public :
	CRenderObject();
	~CRenderObject();

	FLOAT GetRadius(){return m_fObjectRadius;}
	D3DXVECTOR3 GetCenter(){return m_vCenter;}


	void InitApp();
	HRESULT OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,wchar_t* chXFilename,wchar_t* chEffectname,wchar_t* chTexturename);
	HRESULT OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText);
	void OnFrameMove( double fTime, float fElapsedTime,D3DXMATRIX matWorld,D3DXMATRIX matView,D3DXMATRIX matProj );
	void  OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
	void  OnD3D9LostDevice( void* pUserContext );
	void OnD3D9DestroyDevice( void* pUserContext );
	HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh );

};
