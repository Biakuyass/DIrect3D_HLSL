#include"CRenderObject.h"
CRenderObject::CRenderObject()
{
	m_Effect=NULL;
	m_pTexture=NULL;
	m_pMesh=NULL;
	m_fObjectRadius=0;
}

CRenderObject::~CRenderObject()
{
	SAFE_RELEASE(m_Effect);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pMesh);

}

HRESULT CRenderObject::OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,wchar_t* chXFilename,wchar_t* chEffectname,wchar_t* chTexturename)
{
	//读模型
	HRESULT hr;

	V_RETURN(LoadMesh(pd3dDevice,chXFilename,&m_pMesh))

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

HRESULT  CRenderObject::OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText)
{
	
	HRESULT hr;
	//technique和常量设置
	V_RETURN(m_Effect->SetTechnique("CommonRender"));
	V_RETURN(m_Effect->SetTexture("g_Texture",m_pTexture));

	return S_OK;
}

void  CRenderObject:: OnFrameMove( double fTime, float fElapsedTime,D3DXMATRIX matWorld,D3DXMATRIX matView,D3DXMATRIX matProj )
{
	HRESULT hr;
	//矩阵设置
	D3DXMATRIX mWorld=matWorld;
	D3DXMATRIX mWorldViewProj=matWorld*matView*matProj;
	V(m_Effect->SetMatrix("matWorld",&mWorld));
	V(m_Effect->SetMatrix("matWorldViewProj",&mWorldViewProj));

}
void  CRenderObject::OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	HRESULT hr;
	UINT cPasses;
	V(m_Effect->Begin(&cPasses,0));
	for(int i=0;i<cPasses;i++)
	{
		m_Effect->BeginPass(i);
		m_pMesh->DrawSubset(0);
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

}