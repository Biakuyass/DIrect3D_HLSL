#pragma once
#include <fbxsdk.h>
#include<d3d9.h>
#include<d3dx9.h>
#include "DXUT.h"
#include "DXUTcamera.h"
#include"GetPosition.h"

struct MyVertex
{
	float x,y,z;
	DWORD Color;

};
class BoneAnime
{
private:
	FbxManager * mSdkManager;
	FbxScene * mScene;
	//FbxImporter * mImporter;
	//FbxAnimLayer * mCurrentAnimLayer;
	FbxNode*    mRootNode;
	LPDIRECT3DVERTEXDECLARATION9 mDecl;
	IDirect3DDevice9* m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 pVertex;


public:
	BoneAnime();
	~BoneAnime();

	void DrawNodeRecursive(FbxNode* pNode, FbxTime& pTime,
		FbxAMatrix& pParentGlobalPosition,
		FbxPose* pPose);
	void DrawNode(FbxNode* pNode, 
		FbxTime& lTime, 
		FbxAMatrix& pParentGlobalPosition,
		FbxAMatrix& pGlobalPosition,
		FbxPose* pPose);
	void Init();
	void Destroy();
	void DrawLine(FbxAMatrix& pParentGlobalPosition,FbxAMatrix& pGlobalPosition);
	void RenderModel();
	void SetDevice(IDirect3DDevice9* Device){m_pd3dDevice=Device;}

};
