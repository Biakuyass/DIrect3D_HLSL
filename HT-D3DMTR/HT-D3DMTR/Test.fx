
matrix matWorld;
matrix matWorldViewProj;

texture g_Texture;

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
	float2 TextureUV :TEXCOORD0;
	float4 vPosition :TEXCOORD1;
	float4 vNormal   :TEXCOORD2;


};

struct PS_OUTPUT
{
	float4 Color  :COLOR0;
	float4 FColor :COLOR1;
	float4 SColor :COLOR2;
};

VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
	VS_OUTPUT Output= (VS_OUTPUT)0;
	Output.position=mul(input.position,matWorldViewProj);
	Output.TextureUV=input.TextureUV;

	Output.vPosition=mul(input.position,matWorld);
	Output.vNormal   =mul(input.Normal,matWorld);

	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;

	Output.Color=tex2D(MeshTexture,input.TextureUV);
	Output.FColor=float4(1.0f,0,0,1.0f);
	Output.SColor=float4(0,0,1.0f,1.0f);
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
