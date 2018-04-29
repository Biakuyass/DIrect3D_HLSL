#include"BoneAnime.h"

int count=0;
BoneAnime::BoneAnime()
{
	
}

void BoneAnime::Init()
{

	const char* lFilename = "humanoid.fbx";

	 mSdkManager = FbxManager::Create();

	FbxIOSettings *ios = FbxIOSettings::Create(mSdkManager, IOSROOT);
	mSdkManager->SetIOSettings(ios);

	FbxImporter* lImporter = FbxImporter::Create(mSdkManager,"");

	if(!lImporter->Initialize(lFilename, -1, mSdkManager->GetIOSettings())) { 
		MessageBoxA(0,"Call to FbxImporter::Initialize() failed",0,0);
	}
	mScene = FbxScene::Create(mSdkManager,"myScene");
	lImporter->Import(mScene);

	lImporter->Destroy();

	mRootNode = mScene->GetRootNode();


	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, sizeof(D3DXVECTOR3), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0   },
		D3DDECL_END()
	};
	m_pd3dDevice->CreateVertexDeclaration(decl,&mDecl);

	if( FAILED(m_pd3dDevice->CreateVertexBuffer( 2 * sizeof( MyVertex ),
		0, 0, D3DPOOL_DEFAULT, &pVertex, NULL )))
		MessageBoxA(0,"Init Vertex Error",0,0);

}

void BoneAnime::RenderModel()
{
	count=0;
	FbxAMatrix OriginPosiiton;
	FbxPose* temp_pose= mScene->GetPose(0);

	FbxTime temp_time=0;
	DrawNodeRecursive(mRootNode,temp_time,OriginPosiiton,temp_pose);


}

void BoneAnime::DrawNodeRecursive(FbxNode* pNode, FbxTime& pTime, 
								  FbxAMatrix& pParentGlobalPosition,
								  FbxPose* pPose)
{

	FbxAMatrix lGlobalPosition = GetGlobalPosition(pNode, pTime, pPose, &pParentGlobalPosition);

	if (pNode->GetNodeAttribute())
	{
		// Geometry offset.
		// it is not inherited by the children.
		FbxAMatrix lGeometryOffset = GetGeometry(pNode);
		FbxAMatrix lGlobalOffPosition = lGlobalPosition * lGeometryOffset;

		DrawNode(pNode, pTime,  pParentGlobalPosition, lGlobalOffPosition, pPose);
	}

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		DrawNodeRecursive(pNode->GetChild(lChildIndex), pTime,  lGlobalPosition, pPose);
	}
}

BoneAnime::~BoneAnime()
{


		//mScene->Destroy();
	//	mRootNode->Destroy();
}

void BoneAnime::Destroy()
{	   if(mSdkManager)
       mSdkManager->Destroy();
      SAFE_RELEASE(mDecl);
       SAFE_RELEASE(pVertex);

}

void BoneAnime::DrawNode(FbxNode* pNode, FbxTime& lTime, FbxAMatrix& pParentGlobalPosition, FbxAMatrix& pGlobalPosition, FbxPose* pPose)
{
	FbxSkeleton* lSkeleton = (FbxSkeleton*) pNode->GetNodeAttribute();
	// Only draw the skeleton if it's a limb node and if 
	// the parent also has an attribute of type skeleton.
	if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimbNode &&
		pNode->GetParent() &&
		pNode->GetParent()->GetNodeAttribute() &&
		pNode->GetParent()->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{

	
		//char s[256];
	//	sprintf_s(s,"%d",count++);
		//if(count>75)
	//	MessageBoxA(0,s,0,0);
		//»­Ïß
		DrawLine(pParentGlobalPosition,pGlobalPosition);
	}
}

void BoneAnime::DrawLine(FbxAMatrix& pParentGlobalPosition,FbxAMatrix& pGlobalPosition)
{

	//DWORD dwNumVertices=2;
	//SAFE_RELEASE(pVertex);
	//¶¥µã»º´æ



	MyVertex *pVertices=NULL;

	//Get Position
	double *parent_position,*child_position;
	parent_position=(double*)pParentGlobalPosition.GetT();
	child_position=(double*)pGlobalPosition.GetT();

	//¸³Öµ
	pVertex->Lock(0,0,(void**)&pVertices,0);
	pVertices[0].x=parent_position[0];
    pVertices[0].y=parent_position[1];
	pVertices[0].z=parent_position[2];
	pVertices[0].Color=D3DCOLOR_XRGB(255,0,0);

	pVertices[1].x=child_position[0];
	pVertices[1].y=child_position[1];
	pVertices[1].z=child_position[2];
	pVertices[1].Color=D3DCOLOR_XRGB(255,0,0);
	pVertex->Unlock();



	//äÖÈ¾
	m_pd3dDevice->SetVertexDeclaration(mDecl);
	m_pd3dDevice->SetStreamSource(0,pVertex,0,sizeof(MyVertex));

	m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST,0,1);

}

