
texture g_Texture;

sampler BoardTexture=sampler_state
{
	Texture=<g_Texture>;
	MipFilter=LINEAR;
	MinFilter=LINEAR;
	MagFilter=LINEAR;
	//ADDRESSU  = MIRROR;
   // ADDRESSV  = MIRROR;
};

struct VS_INPUT
{
	float4 position  :POSITION;
	float4 color     :COLOR0;
	float2 TextureUV:TEXCOORD0;

};

struct VS_OUTPUT
{
	float4 position  :POSITION;
	float4 color     :COLOR0;
	float2 TextureUV:TEXCOORD0;

};

struct PS_OUTPUT
{
	float4 Color:COLOR0;
};

VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
	VS_OUTPUT Output= (VS_OUTPUT)0;
	Output.position.x=sign(input.position.x);//sign这货返回的是整数
	Output.position.x/=2;
	Output.position.y=sign(input.position.y);
	Output.position.y/=2;
	Output.position.z=1;
	Output.position.w=1;


	Output.color=input.color;
	Output.TextureUV=input.TextureUV;
	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;
	float4 temp_color=float4(input.color.x,input.color.y,input.color.z,1);

	Output.Color=tex2D(BoardTexture,input.TextureUV)*temp_color;
    //Output.Color=float4(1,1,1,1);
	return Output;
}

technique Default
{
	pass P0
	{
		/*AlphaBlendEnable=true;
		SrcBlend=Zero;
		DestBlend=One;*/
		AlphaTestEnable=true;
		AlphaRef=0x10;
		AlphaFunc=Greater;


		StencilEnable=true;
		StencilFunc=Always;
		StencilRef=0x0f;
		StencilPass=Replace;


		VertexShader=compile vs_2_0 RenderSceneVS();
		PixelShader =compile ps_2_0 RenderScenePS();
	}
}