#include"CRenderObject.h"
CRenderObject::CRenderObject()
{
	m_Effect=NULL;
	m_pTexture=NULL;
	m_pMesh=NULL;
	m_fObjectRadius=0;

    m_pVertexBuffer=NULL;
	m_pIndexBuffer=NULL;
	m_dwNumVertices=0;
	m_dwNumIndices=0;
}

CRenderObject::~CRenderObject()
{
	SAFE_RELEASE(m_Effect);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pDecl);

}

HRESULT CRenderObject::OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,wchar_t* chXFilename,wchar_t* chEffectname,wchar_t* chTexturename)
{

	//设置骨骼动画的设备
	m_BoneAnime.SetDevice(pd3dDevice);
	m_BoneAnime.Init();
	//读模型
	HRESULT hr;

	//V_RETURN(LoadMesh(pd3dDevice,chXFilename,&m_pMesh))
	LPD3DXMESH   pMeshClone;
	V_RETURN(D3DXLoadMeshFromX(chXFilename,D3DXMESH_SYSTEMMEM,pd3dDevice,0,0,0,0,&pMeshClone));

	//顶点声明
	/*D3DVERTEXELEMENT9 decl[]=
	{
		{ 0, 0,                                   D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0   },
		{ 0,sizeof(D3DXVECTOR3),                  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0   },
		{ 0,sizeof(D3DXVECTOR3)*2,                D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0   },
	      D3DDECL_END()
	};*/
	//一定要跟顶点格式相对应
	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0,                   D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, sizeof(D3DXVECTOR3), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
		D3DDECL_END()
	};

	V_RETURN(pd3dDevice->CreateVertexDeclaration( decl, &m_pDecl ));

	V_RETURN(pMeshClone->CloneMesh(D3DXMESH_MANAGED,decl,pd3dDevice,&m_pMesh));
	V_RETURN(m_pMesh->GetVertexBuffer(&m_pVertexBuffer));
    V_RETURN(m_pMesh->GetIndexBuffer(&m_pIndexBuffer));
	m_dwNumVertices=m_pMesh->GetNumVertices();
	m_dwNumIndices=m_pMesh->GetNumFaces();

	SAFE_RELEASE(pMeshClone);

	//最大包围球半径
	D3DXVECTOR3* pData;
	if( FAILED( hr = m_pMesh->LockVertexBuffer( 0 ,( LPVOID* ) &pData ))) return hr;
	if( FAILED( hr= D3DXComputeBoundingSphere( pData, m_pMesh->GetNumVertices(), D3DXGetFVFVertexSize( m_pMesh->GetFVF() ), &m_vCenter, &m_fObjectRadius ))) return hr;
	if( FAILED( hr = m_pMesh->UnlockVertexBuffer( ))) return hr;


	//读效果文件
	LPD3DXBUFFER errorBuffer=NULL;

	hr=D3DXCreateEffectFromFile(
		pd3dDevice,
		chEffectname,
		0,
		0,
		D3DXSHADER_DEBUG,
		0,
		&m_Effect,
		&errorBuffer
		);
	if(errorBuffer)
	{
		MessageBoxA(0,(char*)errorBuffer->GetBufferPointer(),0,0);
		SAFE_RELEASE(errorBuffer);
	}

	if(FAILED(hr))
	{
		MessageBox(0,L"Shder Compile Failed",0,0);
		return false;
	}

	//读纹理
	V_RETURN( D3DXCreateTextureFromFileEx( pd3dDevice, chTexturename, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0,
		NULL, NULL, &m_pTexture ) );

	return S_OK;
}

HRESULT CRenderObject::LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh )
{
	ID3DXMesh* pMesh = NULL;
	WCHAR str[MAX_PATH];
	HRESULT hr;

	// Load the mesh with D3DX and get back a ID3DXMesh*.  For this
	// sample we'll ignore the X file's embedded materials since we know 
	// exactly the model we're loading.  See the mesh samples such as
	// "OptimizedMesh" for a more generic mesh loading example.
	//V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, strFileName ) );
	V_RETURN( D3DXLoadMeshFromX( strFileName, D3DXMESH_MANAGED, pd3dDevice, NULL, NULL, NULL, NULL, &pMesh ) );

	DWORD* rgdwAdjacency = NULL;

	// Make sure there are normals which are required for lighting
	if( !( pMesh->GetFVF() & D3DFVF_NORMAL ) )
	{
		ID3DXMesh* pTempMesh;
		V( pMesh->CloneMeshFVF( pMesh->GetOptions(),
			pMesh->GetFVF() | D3DFVF_NORMAL,
			pd3dDevice, &pTempMesh ) );
		V( D3DXComputeNormals( pTempMesh, NULL ) );

		SAFE_RELEASE( pMesh );
		pMesh = pTempMesh;
	}

	// Optimize the mesh for this graphics card's vertex cache 
	// so when rendering the mesh's triangle list the vertices will 
	// cache hit more often so it won't have to re-execute the vertex shader 
	// on those vertices so it will improve perf.     
	rgdwAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
	if( rgdwAdjacency == NULL )
		return E_OUTOFMEMORY;
	V( pMesh->GenerateAdjacency( 1e-6f, rgdwAdjacency ) );
	V( pMesh->OptimizeInplace( D3DXMESHOPT_VERTEXCACHE, rgdwAdjacency, NULL, NULL, NULL ) );
	delete []rgdwAdjacency;

	*ppMesh = pMesh;

	return S_OK;
}

HRESULT  CRenderObject::OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfaceDesc,void *pUserText)
{
	
	HRESULT hr;
	//technique和常量设置
 
	V_RETURN(m_Effect->SetTexture("g_Texture",m_pTexture));

	/*DWORD dwNumVertices=2;
	//顶点缓存
	if( FAILED(pd3dDevice->CreateVertexBuffer( dwNumVertices * sizeof( MyVertex ),
		0, 0, D3DPOOL_DEFAULT, &pVertex, NULL )))
		MessageBoxA(0,"Init Vertex Error",0,0);*/


	return S_OK;
}

void  CRenderObject:: OnFrameMove(double fTime, float fElapsedTime,D3DXMATRIX* pmatWorld,D3DXMATRIX* pmWorldViewProj)
{
	HRESULT hr;
	//矩阵设置
//	D3DXMATRIX mWorld=matWorld;
	//D3DXMATRIX mWorldViewProj=matWorld*matView*matProj;
	V(m_Effect->SetMatrix("matWorld",pmatWorld));
	V(m_Effect->SetMatrix("matWorldViewProj",pmWorldViewProj));


}

void  CRenderObject::OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	HRESULT hr;

	//pd3dDevice->SetVertexDeclaration(m_pDecl);

	//pd3dDevice->SetStreamSource(0,m_pVertexBuffer,0,sizeof(VS_INPUT));
	//pd3dDevice->SetIndices(m_pIndexBuffer);
	

	//渲染
	//pd3dDevice->SetVertexDeclaration(m_pDecl);
	//pd3dDevice->SetStreamSource(0,pVertex,0,sizeof(MyVertex));
	V(m_Effect->SetTechnique("CommonRender"));
	UINT cPasses;
	V(m_Effect->Begin(&cPasses,0));
	for(int i=0;i<cPasses;i++)
	{
		m_Effect->BeginPass(i);


	/*	MyVertex *pVertices=NULL;
		//赋值
		pVertex->Lock(0,0,(void**)&pVertices,0);

		pVertices[0].x=-10;
		pVertices[0].y=10;
		pVertices[0].z=0;
		pVertices[0].Color=D3DCOLOR_XRGB(255,0,0);

		pVertices[1].x=10;
		pVertices[1].y=10;
		pVertices[1].z=0;
		pVertices[1].Color=D3DCOLOR_XRGB(255,0,0);

		pVertex->Unlock();
		pd3dDevice->DrawPrimitive(D3DPT_LINELIST,0,1);*/


		//pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_dwNumVertices,0,m_dwNumIndices);
		m_BoneAnime.RenderModel();
		m_Effect->EndPass();

	}
	V(m_Effect->End());
	

}
void CRenderObject::OnD3D9LostDevice( void* pUserContext)
{
	if(m_Effect)
		m_Effect->OnLostDevice();
}
void CRenderObject::OnD3D9DestroyDevice( void* pUserContext )
{
	SAFE_RELEASE(m_Effect);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pDecl);
	m_BoneAnime.Destroy();


}