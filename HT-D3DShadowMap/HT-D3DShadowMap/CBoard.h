//----------------------------------------------------------------
//这个类就是糊在屏幕上的大贴图的·感觉
//----------------------------------------------------------------

#include<d3d9.h>
#include<d3dx9.h>
#include "DXUT.h"

struct Board_Vertex
{
	float x,y,z,w;//齐次坐标
	DWORD color;//颜色
	float u,v;//纹理坐标

};

class CBoard
{
	LPDIRECT3DTEXTURE9    m_pTexture;
	LPD3DXEFFECT              m_pEffect;

	LPDIRECT3DVERTEXBUFFER9   m_pVertex;
	LPDIRECT3DINDEXBUFFER9    m_pIndex;

	DWORD                     m_dwNumVertices;
	DWORD                     m_dwNumIndices;
	
	LPDIRECT3DVERTEXDECLARATION9  m_pDecl;   
public:
	CBoard();
	~CBoard();

	HRESULT CreateSquare(LPDIRECT3DDEVICE9 pd3dDevice);
	void InitApp();
	HRESULT OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfaceDesc,void *pUserText);
	HRESULT OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfaceDesc,void *pUserText);
	void OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
	void  OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
	void  OnD3D9LostDevice( void* pUserContext );
	void OnD3D9DestroyDevice( void* pUserContext );

	void SetTexture(LPDIRECT3DTEXTURE9 texture_){m_pEffect->SetTexture("g_Texture",texture_);}

};
