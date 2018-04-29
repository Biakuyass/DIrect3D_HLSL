
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
	float4 Color     :COLOR0;


};

struct VS_OUTPUT
{
	float4 position  :POSITION;
	float4 Color     :COLOR0;

};

struct PS_OUTPUT
{
	float4 Color:COLOR0;
};

VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
	VS_OUTPUT Output= (VS_OUTPUT)0;
	Output.position=mul(input.position,matWorldViewProj);
	/*Output.position.x=sign(input.position.x);
	Output.position.y=sign(input.position.y);
	Output.position.z=0.6;
	Output.position.w=1.0;*/
	Output.Color=input.Color;
	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;
	Output.Color=input.Color;

	//Output.Color=float4(1.0,1.0,1.0,1.0);

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
