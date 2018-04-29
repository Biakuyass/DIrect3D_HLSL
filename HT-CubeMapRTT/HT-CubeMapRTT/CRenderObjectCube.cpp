
#include"CRenderObjectCube.h"
HRESULT CRenderObjectCube::OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,wchar_t* chXFilename,wchar_t* chEffectname,wchar_t* chTexturename,wchar_t* chCubeTexturename)
{
	HRESULT hr;

	CRenderObject::OnD3D9CreateDevice(pd3dDevice,chXFilename,chEffectname,chTexturename);

	if(chCubeTexturename!=NULL)
	V_RETURN(D3DXCreateCubeTextureFromFile(pd3dDevice,chCubeTexturename,&m_pCubeTexture));

	//创建立方体纹理
	pd3dDevice->CreateCubeTexture( 256,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A16B16G16R16F,
		D3DPOOL_DEFAULT,
		&m_RenderToCube,
		NULL );


	return S_OK;

}

HRESULT CRenderObjectCube::OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText)
{
	HRESULT hr;
	CRenderObject::OnD3D9ResetDevice(pd3dDevice,pBackBufferSurfacfeDesc,pUserText);

	if(m_pCubeTexture!=NULL)
	m_Effect->SetTexture("g_CubeTexture",m_pCubeTexture);

	return S_OK;
}
void CRenderObjectCube::OnFrameMove(double fTime, float fElapsedTime,D3DXMATRIX* pmatWorld,D3DXMATRIX* pmWorldViewProj,D3DXMATRIX *pmWorldInverseTranspose, D3DXVECTOR4* vEyePosition)
{
	CRenderObject::OnFrameMove(fTime,fElapsedTime,pmatWorld,pmWorldViewProj);
	if(pmWorldInverseTranspose!=NULL)
	{
		m_Effect->SetMatrix("matWorldInverseTranspose",pmWorldInverseTranspose);

	}
	if(vEyePosition!=NULL)
	{
		m_Effect->SetVector("vEyePosition",vEyePosition);
	}


}

void CRenderObjectCube::OnD3D9FrameRender(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime, void* pUserContext)
{
	CRenderObject::OnD3D9FrameRender(pd3dDevice,fTime,fElapsedTime,pUserContext);

}

void CRenderObjectCube::OnD3D9DestroyDevice(void* pUserContext)
{
	CRenderObject::OnD3D9DestroyDevice(pUserContext);
	SAFE_RELEASE(m_pCubeTexture);
	SAFE_RELEASE(m_RenderToCube);

}



CRenderObjectCube::CRenderObjectCube()
{
	m_pCubeTexture=NULL;
	CRenderObject::CRenderObject();
}

CRenderObjectCube::~CRenderObjectCube()
{

}

//获得六个方向的观察矩阵
D3DXMATRIX WINAPI DXUTGetCubeMapViewMatrix( DWORD dwFace )
{
	D3DXVECTOR3 vEyePt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vLookDir;
	D3DXVECTOR3 vUpDir;

	switch( dwFace )
	{
	case D3DCUBEMAP_FACE_POSITIVE_X:
		vLookDir = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		vUpDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_X:
		vLookDir = D3DXVECTOR3( -1.0f, 0.0f, 0.0f );
		vUpDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case D3DCUBEMAP_FACE_POSITIVE_Y:
		vLookDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		vUpDir = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_Y:
		vLookDir = D3DXVECTOR3( 0.0f, -1.0f, 0.0f );
		vUpDir = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		break;
	case D3DCUBEMAP_FACE_POSITIVE_Z:
		vLookDir = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		vUpDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_Z:
		vLookDir = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
		vUpDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	}

	// Set the view transform for this cubemap surface
	D3DXMATRIXA16 mView;
	D3DXMatrixLookAtLH( &mView, &vEyePt, &vLookDir, &vUpDir );
	return mView;
}

void CRenderObjectCube::RenderToCubeTexture(IDirect3DDevice9* pd3dDevice,double fTime, float fElapsedTime, void* pUserContext,CModelViewerCamera & g_Camera,
											void(*RenderScene)(IDirect3DDevice9* pd3dDevice,double fTime, float fElapsedTime, void* pUserContext,D3DXMATRIX &,D3DXMATRIX &))
{
	HRESULT hr;

	// The projection matrix has a FOV of 90 degrees and asp ratio of 1
	D3DXMATRIXA16 mProj;
	D3DXMatrixPerspectiveFovLH( &mProj, D3DX_PI * 0.5f, 1.0f, 0.01f, 100.0f );

	D3DXMATRIXA16 mViewDir( *g_Camera.GetViewMatrix() );
	mViewDir._41 = mViewDir._42 = mViewDir._43 = 0.0f;

	LPDIRECT3DSURFACE9 pRTOld = NULL;
	V( pd3dDevice->GetRenderTarget( 0, &pRTOld ) );
	/*LPDIRECT3DSURFACE9 pDSOld = NULL;
	if( SUCCEEDED( pd3dDevice->GetDepthStencilSurface( &pDSOld ) ) )
	{
		// If the device has a depth-stencil buffer, use
		// the depth stencil buffer created for the cube textures.
		V( pd3dDevice->SetDepthStencilSurface( g_pDepthCube ) );
	}*/

		for( int nFace = 0; nFace < 6; ++nFace )
		{
			LPDIRECT3DSURFACE9 pSurf;

			V( m_RenderToCube->GetCubeMapSurface( ( D3DCUBEMAP_FACES )nFace, 0, &pSurf ) ); 
			V( pd3dDevice->SetRenderTarget( 0, pSurf ) );
			SAFE_RELEASE( pSurf );

			D3DXMATRIXA16 mView = DXUTGetCubeMapViewMatrix( nFace );
		//	D3DXMatrixMultiply( &mView, &mViewDir, &mView );

			V( pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER,
				0x000000ff, 1.0f, 0L ) );

			// Begin the scene
			if( SUCCEEDED( pd3dDevice->BeginScene() ) )
			{
			//
				
				RenderScene(pd3dDevice,fTime,fElapsedTime,pUserContext,mView,mProj);

			//	CRenderObject::OnD3D9FrameRender(pd3dDevice,fTime,fElapsedTime,pUserContext);

				// End the scene.
				pd3dDevice->EndScene();
			}
		}

		// Restore depth-stencil buffer and render target
		/*if( pDSOld )
		{
			V( pd3dDevice->SetDepthStencilSurface( pDSOld ) );
			SAFE_RELEASE( pDSOld );
		}*/
		V( pd3dDevice->SetRenderTarget( 0, pRTOld ) );
		SAFE_RELEASE( pRTOld );
}







