
matrix matWorld;
matrix matWorldViewProj;

float4 vEyePosition;
matrix matWorldInverseTranspose;

texture g_Texture;
textureCUBE g_CubeTexture;


sampler MeshTexture=sampler_state
{
	Texture=<g_Texture>;
	MipFilter=LINEAR;
	MinFilter=LINEAR;
	MagFilter=LINEAR;
};

samplerCUBE MeshCubeTexture=sampler_state
{
	Texture=<g_CubeTexture>;
	MipFilter=LINEAR;
	MinFilter=LINEAR;
	MagFilter=LINEAR;
};

struct VS_INPUT
{
	float4 position  :POSITION;
	float3 Normal    :NORMAL;
	float2 TextureUV:TEXCOORD0;

};

struct VS_OUTPUT
{
	float4 position  :POSITION;
	float2 TextureUV :TEXCOORD0;
	float3 Normal    :TEXCOORD1;
	float3 vEyetoVertex     :TEXCOORD2;

};

struct PS_OUTPUT
{
	float4 Color:COLOR0;
};

VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
	VS_OUTPUT Output= (VS_OUTPUT)0;

	Output.position=mul(input.position,matWorldViewProj);

	float4 Position=mul(input.position,matWorld);

	Output.vEyetoVertex=Position-vEyePosition;
	Output.Normal =normalize(mul(input.Normal,(float3x3)matWorldInverseTranspose));

	Output.TextureUV=input.TextureUV;




	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;

	float3 vReflect=reflect(input.vEyetoVertex,input.Normal);
	Output.Color=texCUBE(MeshCubeTexture,vReflect);
//	Output.Color=float4(Output.Color.xyz,1.0);

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
