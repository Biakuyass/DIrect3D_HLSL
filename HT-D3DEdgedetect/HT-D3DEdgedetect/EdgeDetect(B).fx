
texture g_Texture;

float offset=1.0/640.0;
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

	float s_11=GreyValue(tex2D(BoardTexture,input.TextureUV+float2(-offset,-offset)));
	float s_12=GreyValue(tex2D(BoardTexture,input.TextureUV+float2(    0  ,-offset)));
	float s_13=GreyValue(tex2D(BoardTexture,input.TextureUV+float2( offset,-offset)));
	float s_21=GreyValue(tex2D(BoardTexture,input.TextureUV+float2(-offset,0)));
	float s_23=GreyValue(tex2D(BoardTexture,input.TextureUV+float2(offset,0)));
	float s_31=GreyValue(tex2D(BoardTexture,input.TextureUV+float2(-offset,offset)));
	float s_32=GreyValue(tex2D(BoardTexture,input.TextureUV+float2(0,      offset)));
	float s_33=GreyValue(tex2D(BoardTexture,input.TextureUV+float2(offset, offset)));

	float sobelX=(s_13+2*s_23+s_33)-(s_11+2*s_21+s_31);
	float sobelY=(s_11+2*s_12+s_13)-(s_31+2*s_32+s_33);

	float EdgeJudge=sobelX*sobelX+sobelY*sobelY;

	if(EdgeJudge>0.5)
	{
		Output.Color=float4(1.0,1.0,1.0,1.0);
	}
	else
	{
		Output.Color=float4(0.0,0.0,0.0,0.0);
	}

//	Output.Color=tex2D(BoardTexture,input.TextureUV);
	return Output;
}

technique Default
{
	pass P0
	{
		AlphaBlendEnable=true;
		SrcBlend = SrcAlpha;
	   DestBlend= InvSrcAlpha;



		VertexShader=compile vs_2_0 RenderSceneVS();
		PixelShader =compile ps_2_0 RenderScenePS();
	}
}