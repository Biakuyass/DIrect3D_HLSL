
#include "CRenderObject.h"
class CRenderObjectCube : public CRenderObject
{
private:
	LPDIRECT3DCUBETEXTURE9 m_pCubeTexture;
public:
	HRESULT OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,wchar_t* chXFilename,wchar_t* chEffectname,wchar_t* chTexturename,wchar_t* chCubeTexturename);
	HRESULT OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText);
	void  OnD3D9FrameRender( LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
    void OnFrameMove( double fTime, float fElapsedTime,D3DXMATRIX* pmatWorld,D3DXMATRIX* pmWorldViewProj,D3DXMATRIX *pmWorldInverseTranspose,D3DXVECTOR4*vEyePosition);
	void OnD3D9DestroyDevice( void* pUserContext );
	CRenderObjectCube();
	~CRenderObjectCube();

};