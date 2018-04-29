
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
	Output.position.x=sign(input.position.x);
	Output.position.y=sign(input.position.y);
	Output.position.z=0.5;
	Output.position.w=1;


	Output.color=input.color;
	Output.TextureUV=input.TextureUV;
	return Output;
}

float GreyValue(float4 vColor)
{
	return (vColor.x+vColor.y+vColor.z)/3.0;
}


PS_OUTPUT RenderScenePS(VS_OUTPUT input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;

	float2 TextureSize=float2(640,480);
	float2 MosicSize=float2(10,10);

	float2 Coordiate=float2(input.TextureUV.x*TextureSize.x,input.TextureUV.y*TextureSize.y);
	float2 Mosic=float2(int(Coordiate.x/MosicSize.x)*MosicSize.x,int(Coordiate.y/MosicSize.y)*MosicSize.y);

	float2 MosicUV=float2(Mosic.x/TextureSize.x,Mosic.y/TextureSize.y);


    Output.Color=tex2D(BoardTexture,MosicUV);
	return Output;
}

technique Default
{
	pass P0
	{
		VertexShader=compile vs_2_0 RenderSceneVS();
		PixelShader =compile ps_2_0 RenderScenePS();
	}
}