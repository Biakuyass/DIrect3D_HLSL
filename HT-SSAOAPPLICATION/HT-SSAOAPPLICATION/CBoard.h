//----------------------------------------------------------------
//�������Ǻ�����Ļ�ϵĴ���ͼ�ġ��о�
//----------------------------------------------------------------

#include<d3d9.h>
#include<d3dx9.h>
#include "DXUT.h"

struct Board_Vertex
{
	float x,y,z,w;//�������
	DWORD color;//��ɫ
	float u,v;//��������

};
class CBoard
{
	LPDIRECT3DTEXTURE9        m_pDepthTexture;
	LPDIRECT3DTEXTURE9        m_pNoiseTexture;
	LPD3DXEFFECT              m_Effect;

	LPDIRECT3DVERTEXBUFFER9   m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9    m_pIndexBuffer;

	DWORD                     m_dwNumVertices;
	DWORD                     m_dwNumIndices;
	LPDIRECT3DVERTEXDECLARATION9  m_pDecl;   

	LPDIRECT3DTEXTURE9            m_pRTTTextureSSAO;    // ����
	LPD3DXRENDERTOSURFACE         m_pRTTInterfaceSSAO;  // ��������Ľӿ�
	LPDIRECT3DSURFACE9            m_pRTTSurfaceSSAO;    // ҳ��

public:
	CBoard();
	~CBoard();
	void SetTexture(LPDIRECT3DTEXTURE9 Texture_)
	{   m_pDepthTexture=Texture_;	
	    m_Effect->SetTexture("g_DepthTexture",m_pDepthTexture);
	}

	HRESULT CreateSquare(LPDIRECT3DDEVICE9 pd3dDevice);
	void InitApp();
	HRESULT OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfaceDesc,void *pUserText);
	HRESULT OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfaceDesc,void *pUserText);
	void OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
	void  OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
	void  OnD3D9LostDevice( void* pUserContext );
	void OnD3D9DestroyDevice( void* pUserContext );

	void ReturnToTexture(LPDIRECT3DDEVICE9 pd3dDevice,D3DXMATRIX* pmatWorld,D3DXMATRIX* pmWorldViewProj);

	LPDIRECT3DTEXTURE9 ReturnSSAOTexture(){return m_pRTTTextureSSAO;}

};
