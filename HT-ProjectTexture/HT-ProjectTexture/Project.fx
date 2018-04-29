
matrix matWorld;
matrix matWorldViewProj;

matrix matWorldViewProjProject;



texture g_Texture;

sampler MeshTexture=sampler_state
{
	Texture=<g_Texture>;
	MipFilter=LINEAR;
	MinFilter=LINEAR;
	MagFilter=LINEAR;
	ADDRESSU=BORDER;
	ADDRESSV=BORDER;

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
	float4 ProjectUV :TEXCOORD1;

};

struct PS_OUTPUT
{
	float4 Color:COLOR0;
};

VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
	VS_OUTPUT Output= (VS_OUTPUT)0;
	Output.position=mul(input.position,matWorldViewProj);
	Output.TextureUV=input.TextureUV;


	Output.ProjectUV=mul(input.position,matWorldViewProjProject);
	Output.ProjectUV.y=-Output.ProjectUV.y;//纹理的y和屏幕的y相反


	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;

    matrix matOffset;
	
	matOffset._11 = matOffset._22 = matOffset._33 = 0.5;
    matOffset._41 = matOffset._42 = matOffset._43 = 0.5;
    //
    matOffset._12 = matOffset._13 = matOffset._14 = 0.0;
    matOffset._21 = matOffset._23 = matOffset._24 = 0.0;
    matOffset._31 = matOffset._32 = matOffset._34 = 0.0;
    matOffset._44 = 1.0;

	float4 fOffset=mul(input.ProjectUV,matOffset);

	float4 Color=tex2D(MeshTexture,input.TextureUV);

	float4 ColorProj=tex2Dproj(MeshTexture,fOffset);//这里在下偷懒只用了一张纹理

	Output.Color=0.5*Color+0.5*ColorProj;

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
		StencilEnable=true;
		StencilFunc=Equal;
	     StencilRef=0x0f;
		StencilPass=Keep;

		VertexShader=compile vs_2_0 RenderSceneVS();
		PixelShader =compile ps_2_0 RenderScenePS();
	}
}
