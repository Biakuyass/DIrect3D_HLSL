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

	D3DXVECTOR3                   m_vCenter; //����
	FLOAT                         m_fObjectRadius;//����Χ��뾶��������������������ư뾶��


	LPD3DXMESH                 m_pMesh;
	LPDIRECT3DTEXTURE9         m_pTexture;

	LPDIRECT3DVERTEXBUFFER9    m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9     m_pIndexBuffer;
	DWORD                      m_dwNumVertices;
	DWORD                      m_dwNumIndices;

	D3DXMATERIAL             m_Material;

	LPDIRECT3DVERTEXDECLARATION9  m_pDecl;   


	//��Ⱦ������
	LPDIRECT3DTEXTURE9            m_pRTTTexture;     // װ�뷴������
	LPD3DXRENDERTOSURFACE         m_pRTTInterface;   // ���Ʒ�������Ľӿ�
	LPDIRECT3DSURFACE9            m_pRTTSurface;     // ҳ��


	




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
	void  OnD3D9LostDevice( void* pUserContext );
	void OnD3D9DestroyDevice( void* pUserContext );


	void RenderToTexture(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );

	LPDIRECT3DTEXTURE9 GetTexture(){return m_pRTTTexture;}

	HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh );

};


