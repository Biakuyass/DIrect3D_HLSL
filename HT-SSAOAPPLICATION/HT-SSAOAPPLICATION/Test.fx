
matrix matWorld;
matrix matWorldViewProj;

texture g_Texture;
texture g_SSAOTexture;


sampler MeshTexture=sampler_state
{
	Texture=<g_Texture>;
	MipFilter=LINEAR;
	MinFilter=LINEAR;
	MagFilter=LINEAR;
};
sampler SSAOTexture=sampler_state
{
	Texture=<g_SSAOTexture>;
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
	float2 TextureUV :TEXCOORD0;
//	float2 SSAOUV    :TEXCOORD1;
	float  pos_x     : TEXCOORD1;
	float  pos_y     : TEXCOORD2;

};

struct PS_OUTPUT
{
	float4 Color:COLOR0;
};

VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
	VS_OUTPUT Output= (VS_OUTPUT)0;
	Output.position=mul(input.position,matWorldViewProj);

	float temp_x=Output.position.x/Output.position.w;
	float temp_y=Output.position.y/Output.position.w;

	Output.pos_x=temp_x;
	Output.pos_y=temp_y;

	Output.TextureUV=input.TextureUV;
	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;

	


	//float2 SSAOUV=float2((input.pos_x+1)/2.0,(input.pos_y+1)/2.0);
//	float2 SSAOUV=float2(input.pos_x,input.pos_y);

	Output.Color=tex2D(MeshTexture,input.TextureUV);

	return Output;
}

technique CommonRender
{
	pass P0
	{
		//CullMode = None;


		VertexShader=compile vs_2_0 RenderSceneVS();
		PixelShader =compile ps_2_0 RenderScenePS();
	}
}
