#include"CBoard.h"

CBoard::CBoard()
{
	m_pTexture=NULL;
	m_pEffect=NULL;
	m_pVertex=NULL;
	m_pIndex=NULL;
	m_dwNumVertices=0;
	m_dwNumIndices=0;

}

CBoard::~CBoard()
{

}

HRESULT CBoard::CreateSquare(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr;

	m_dwNumIndices=4;
	m_dwNumVertices=4;
	//顶点缓存
	if( FAILED( hr = pd3dDevice->CreateVertexBuffer( m_dwNumVertices * sizeof( Board_Vertex ),
		0, 0, D3DPOOL_DEFAULT, &m_pVertex, NULL )))
		return hr;
	Board_Vertex *pVertices=NULL;

	m_pVertex->Lock(0,0,(void**)&pVertices,0);
	pVertices[ 0 ].x = -10;
	pVertices[ 0 ].y =  10;
	pVertices[ 0 ].z =  0;
	pVertices[ 0 ].w =  1;
	pVertices[ 0 ].color =  D3DCOLOR_ARGB( 0x70 ,0xFF ,0 ,0 );  // 透明顶点
	pVertices[ 0 ].u =  0;
	pVertices[ 0 ].v =  0;
	// 绿
	pVertices[ 1 ].x =  10;
	pVertices[ 1 ].y =  10;
	pVertices[ 1 ].z =  0;
	pVertices[ 1 ].w =  1;
	pVertices[ 1 ].color =  D3DCOLOR_ARGB( 0x70 ,0x0 ,0xFF ,0 );  // 透明顶点
	pVertices[ 1 ].u =  1;
	pVertices[ 1 ].v =  0;
	// 蓝
	pVertices[ 2 ].x = -10;
	pVertices[ 2 ].y = -10;
	pVertices[ 2 ].z = 0;
	pVertices[ 2 ].w =  1;
	pVertices[ 2 ].color =  D3DCOLOR_ARGB( 0x70 ,0x0 ,0 ,0xFF );
	pVertices[ 2 ].u =  0;
	pVertices[ 2 ].v =  1;
	// 白
	pVertices[ 3 ].x =  10;
	pVertices[ 3 ].y = -10;
	pVertices[ 3 ].z =  0;
	pVertices[ 3 ].w =  1;
	pVertices[ 3 ].color =  D3DCOLOR_ARGB( 0x70 ,0x0FF ,0xFF ,0 );
	pVertices[ 3 ].u =  1;
	pVertices[ 3 ].v =  1;
	m_pVertex->Unlock();





	//索引缓存
	if( FAILED( hr = pd3dDevice->CreateIndexBuffer( m_dwNumIndices * sizeof( WORD ),
		0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIndex, NULL )))    
		return hr;
	WORD *pIndices=NULL;
	m_pIndex->Lock(0,0,(void**)&pIndices,0);
	pIndices[ 0 ] = 0;
	pIndices[ 1 ] = 1;
	pIndices[ 2 ] = 2;
	pIndices[ 3 ] = 3;
	m_pIndex->Unlock();
	






	return S_OK;


	
}

void CBoard::InitApp()
{


}

HRESULT CBoard::OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfaceDesc,void *pUserText)
{
	HRESULT hr;
	//加载纹理
	V_RETURN(D3DXCreateTextureFromFileEx(pd3dDevice,  L"Texture2.bmp", D3DX_DEFAULT, D3DX_DEFAULT, 
		D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
		NULL, NULL, &m_pTexture ));
	//---------------------------------------
	//顶点格式声明
	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0   }, 
		{ 0, 4*sizeof(float), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0   },
		{ 0, 4*sizeof(float)+sizeof(DWORD), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0   },
		D3DDECL_END()
	};
	if( FAILED( hr = pd3dDevice->CreateVertexDeclaration( decl, &m_pDecl ))) return hr;


	//-------------------------------------------
	//创建效果文件
	LPD3DXBUFFER errorBuffer;
	hr=D3DXCreateEffectFromFile(
		pd3dDevice,
		L"EdgeDetect(B).fx",
		0,
		0,
		D3DXSHADER_DEBUG,
		0,
		&m_pEffect,
		&errorBuffer
	);
	if(errorBuffer)
	{
		::MessageBoxA(0,(char*)errorBuffer->GetBufferPointer(),0,0);
		SAFE_RELEASE(errorBuffer);
	}
	if(FAILED(hr))
	{
		::MessageBoxA(0,"Shader Compile Failed",0,0);
		return hr;
	}

	return S_OK;
}

HRESULT CBoard::OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText)
{
	HRESULT hr;
	V_RETURN(m_pEffect->SetTexture("g_Texture",m_pTexture));
	V_RETURN(CreateSquare(pd3dDevice));

	return S_OK;

}

void CBoard::OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{

}

void CBoard::OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext)
{
	HRESULT hr;
	pd3dDevice->SetVertexDeclaration(m_pDecl);
	pd3dDevice->SetStreamSource(0,m_pVertex,0,sizeof(Board_Vertex));
	pd3dDevice->SetIndices(m_pIndex);

	m_pEffect->SetTechnique("Default");
	UINT cPasses;
	m_pEffect->Begin(&cPasses,0);

	for(int i=0;i<cPasses;i++)
	{
		m_pEffect->BeginPass(i);
		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0,0,m_dwNumVertices,0,2);
		m_pEffect->EndPass();

	}
	m_pEffect->End();
	

	

}

void CBoard::OnD3D9LostDevice(void* pUserContext)
{
	m_pEffect->OnLostDevice();


}

void CBoard::OnD3D9DestroyDevice(void* pUserContext)
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pVertex);
	SAFE_RELEASE(m_pIndex);
	SAFE_RELEASE(m_pDecl);
}

