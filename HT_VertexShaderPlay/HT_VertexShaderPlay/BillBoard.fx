
matrix matWorld;
matrix matWorldViewProj;

texture g_Texture;

float g_fTime;
float4 g_vEye;//眼睛位置
float4 g_vPosition;//广告牌位置

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
	float3 vRight =normalize(cross(float3(0,1.0,0),vLookat));
	float3 vUp    =normalize(cross(vLookat,vRight));

	matrix matLocal;
	matLocal[0]=float4(vRight.xyz,0);
	matLocal[1]=float4(vUp.xyz,0);
	matLocal[2]=float4(vLookat.xyz,0);
	matLocal[3]=float4(0,0,0,1.0);

	input.position=mul(input.position,matLocal);



	Output.position=mul(input.position,matWorldViewProj);
	Output.TextureUV=input.TextureUV;





	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;
	Output.Color=tex2D(MeshTexture,input.TextureUV);
	return Output;
}

technique CommonRender
{
	pass P0
	{
		//AlphaBlendEnable = true;
		//BlendOp          = Add;
		//SrcBlend         =SrcAlpha;
		//DestBlend        =InvSrcAlpha;

		VertexShader=compile vs_2_0 RenderSceneVS();
		PixelShader =compile ps_2_0 RenderScenePS();
	}
}
