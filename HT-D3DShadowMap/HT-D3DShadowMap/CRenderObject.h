#pragma once;

#include<d3d9.h>
#include<d3dx9.h>
#include "DXUT.h"
#include "DXUTcamera.h"

struct VS_INPUT
{
   D3DXVECTOR3 Position;
   D3DXVECTOR3 Normal;
   float u,v;// TEXCOORD0;
};

class CRenderObject
{
	LPD3DXEFFECT               m_Effect;

	D3DXVECTOR3                   m_vCenter; //中心
	FLOAT                         m_fObjectRadius;//最大包围球半径，用来设置摄像机的推移半径的


	LPD3DXMESH                 m_pMesh;
	LPDIRECT3DTEXTURE9         m_pTexture;

	LPDIRECT3DVERTEXBUFFER9    m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9     m_pIndexBuffer;
	DWORD                      m_dwNumVertices;
	DWORD                      m_dwNumIndices;

	D3DXMATERIAL             m_Material;

	LPDIRECT3DVERTEXDECLARATION9  m_pDecl;   

	LPDIRECT3DTEXTURE9            m_pRTTTextureDepth;    // 纹理
	LPD3DXRENDERTOSURFACE         m_pRTTInterfaceDepth;  // 控制纹理的接口
	LPDIRECT3DSURFACE9            m_pRTTSurfaceDepth;    // 页面




public :
	CRenderObject();
	~CRenderObject();

	FLOAT GetRadius(){return m_fObjectRadius;}
	D3DXVECTOR3 GetCenter(){return m_vCenter;}

	void InitApp();


	HRESULT OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,wchar_t* chXFilename,wchar_t* chEffectname,wchar_t* chTexturename);
	HRESULT OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText);
    void OnFrameMove( double fTime, float fElapsedTime,D3DXMATRIX* pmatWorld,D3DXMATRIX* pmWorldViewProj);
	void  OnD3D9FrameRender( LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );


	void SetDepthTexture(LPDIRECT3DTEXTURE9 texture_)
	{
		m_Effect->SetTexture("g_DepthTexture",texture_);
	}

	void SetSpecialMatrix(D3DXMATRIX * matrix_)
	{
		m_Effect->SetMatrix("matWorldViewProjFromLight",matrix_);

	}

	void RenderToDepthTexture(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );

	LPDIRECT3DTEXTURE9 GetDepthTexture(){return m_pRTTTextureDepth;}



	void  OnD3D9LostDevice( void* pUserContext );
	void OnD3D9DestroyDevice( void* pUserContext );


	HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh );

};

