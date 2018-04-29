
matrix matWorld;
matrix matWorldViewProj;

texture g_Texture;

float g_fTime;
float g_fAlpha;
float4 g_vEye;//眼睛位置
float4 g_vPosition;//广告牌位置

float g_iScaleX;//TextureUV x 轴上的缩放
float g_iTransX;//TextureUV x 轴上的平移
float g_iScaleY;//TextureUV y 轴上的缩放
float g_iTransY;//TextureUV y 轴上的平移

sampler MeshTexture=sampler_state
{
	Texture=<g_Texture>;
	MipFilter=LINEAR;
	MinFilter=LINEAR;
	MagFilter=LINEAR;
};

struct VS_INPUT
{
	float4 position  :POSITION;
	float2 TextureUV:TEXCOORD0;

};

struct VS_OUTPUT
{
	float4 position  :POSITION;
	float2 TextureUV:TEXCOORD0;

};

struct PS_OUTPUT
{
	float4 Color:COLOR0;
};

VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
	VS_OUTPUT Output= (VS_OUTPUT)0;

	float3 vLookat=normalize(g_vPosition.xyz-g_vEye.xyz);//看的方向
	float3 vRight =normalize(cross(float3(-1.0,0,0),vLookat));
	float3 vUp    =normalize(cross(vLookat,vRight));

	matrix matLocal;
	matLocal[0]=float4(vRight.xyz,0);
	matLocal[1]=float4(vUp.xyz,0);
	matLocal[2]=float4(vLookat.xyz,0);
	matLocal[3]=float4(0,0,0,1.0);


	input.position=mul(input.position,matLocal);



	Output.position=mul(input.position,matWorldViewProj);
	//Output.TextureUV=input.TextureUV;
	Output.TextureUV.x=input.TextureUV.x/g_iScaleX+(float)g_iTransX/g_iScaleX;
	Output.TextureUV.y=input.TextureUV.y/g_iScaleY+(float)g_iTransY/g_iScaleY;





	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;
	Output.Color=tex2D(MeshTexture,input.TextureUV)*float4(0,1,0,1);//

  //  Output.Color.a=(Output.Color.r+Output.Color.g+Output.Color.b)/3.0f;
	//Output.Color.a=0.5;
	Output.Color.a=g_fAlpha*Output.Color.a;
	Output.Color.x=Output.Color.x*Output.Color.a;
	Output.Color.y=Output.Color.y*Output.Color.a;
	Output.Color.z=Output.Color.z*Output.Color.a;

	return Output;
}

technique CommonRender
{
	pass P0
	{
		//如果离摄像机远的后渲染，则会被遮住不会显示，如果ZwriteEnable=true的话，这个系统没控制

		ZEnable             = true;
        ZWriteEnable        = false;
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         =One;
		DestBlend        =One;
	/*	SrcBlend         =SrcAlpha;
		DestBlend        =InvSrcAlpha;*/


		/*AlphaTestEnable =true;
		AlphaRef=0x00;
		AlphaFunc=Greater;*/

		FillMode=Solid;
	   // FillMode=WireFrame;

		VertexShader=compile vs_2_0 RenderSceneVS();
		PixelShader =compile ps_2_0 RenderScenePS();
	}
}
