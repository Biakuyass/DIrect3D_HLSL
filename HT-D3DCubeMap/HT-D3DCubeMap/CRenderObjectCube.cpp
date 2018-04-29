
#include"CRenderObjectCube.h"
HRESULT CRenderObjectCube::OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,wchar_t* chXFilename,wchar_t* chEffectname,wchar_t* chTexturename,wchar_t* chCubeTexturename)
{
	HRESULT hr;

	CRenderObject::OnD3D9CreateDevice(pd3dDevice,chXFilename,chEffectname,chTexturename);

	if(chCubeTexturename!=NULL)
	V_RETURN(D3DXCreateCubeTextureFromFile(pd3dDevice,chCubeTexturename,&m_pCubeTexture));

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

}



CRenderObjectCube::CRenderObjectCube()
{
	m_pCubeTexture=NULL;
	CRenderObject::CRenderObject();
}

CRenderObjectCube::~CRenderObjectCube()
{

}





