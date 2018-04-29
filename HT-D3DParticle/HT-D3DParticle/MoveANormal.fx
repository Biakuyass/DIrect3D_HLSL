
matrix matWorld;
matrix matWorldViewProj;

float g_fTime;

texture g_Texture;

//广告牌用
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
	float4 Normal    :NORMAL;
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

//Test Function
float4 Modify(float4 In)
{
	float4 vOut=In;
	matrix matOffset=(matrix)0;
	matOffset._11=	matOffset._22=	matOffset._33=	matOffset._44=1.0;
	float x= In.x+18.0*sin(g_fTime/2.0);
	float y= In.x+18.0*cos(g_fTime);

	matOffset._22=1.0+exp(-x*x/15.0);
	matOffset._33=1.0+exp(-x*x/15.0);
//	matOffset._22=1.0+sin(g_fTime);
//	matOffset._33=1.0+cos(g_fTime);

	vOut=mul(In,matOffset);

	return vOut;
}




VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
	VS_OUTPUT Output= (VS_OUTPUT)0;

	input.position=Modify(input.position);
	//input.position=float4(input.position.xyz+0.5*sin(g_fTime)*input.Normal,input.position.w);



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
