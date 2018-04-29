
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
	Output.position.x=sign(input.position.x);
	Output.position.y=sign(input.position.y);
	Output.position.z=0.5;
	Output.position.w=1;


	Output.TextureUV=input.TextureUV;
	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;

	float Pixel_X=200;
	float Pixel_Y=200;
	//float4 temp_color=float4(input.color.x,input.color.y,input.color.z,1);

	float4 Color=float4(0,0,0,0);
	float BlurWeight[7]={0.1,0.358,0.773,1.0,0.773,0.358,0.1};
	float BlurN=3;

	//X的模糊
	Color+=tex2D(BoardTexture,input.TextureUV+float2((-3.0/Pixel_X),0))*BlurWeight[0];
	Color+=tex2D(BoardTexture,input.TextureUV+float2((-2.0/Pixel_X),0))*BlurWeight[1];
	Color+=tex2D(BoardTexture,input.TextureUV+float2((-1.0/Pixel_X),0))*BlurWeight[2];
	Color+=tex2D(BoardTexture,input.TextureUV+float2((0.0/Pixel_X),0))*BlurWeight[3];
	Color+=tex2D(BoardTexture,input.TextureUV+float2((1.0/Pixel_X),0))*BlurWeight[4];
	Color+=tex2D(BoardTexture,input.TextureUV+float2((2.0/Pixel_X),0))*BlurWeight[5];
	Color+=tex2D(BoardTexture,input.TextureUV+float2((3.0/Pixel_X),0))*BlurWeight[6];


    //Y的模糊
	Color+=tex2D(BoardTexture,input.TextureUV+float2(0,(-3.0/Pixel_Y)))*BlurWeight[0];
	Color+=tex2D(BoardTexture,input.TextureUV+float2(0,(-2.0/Pixel_Y)))*BlurWeight[1];
	Color+=tex2D(BoardTexture,input.TextureUV+float2(0,(-1.0/Pixel_Y)))*BlurWeight[2];
	Color+=tex2D(BoardTexture,input.TextureUV+float2(0,(0.0/Pixel_Y)))*BlurWeight[3];
	Color+=tex2D(BoardTexture,input.TextureUV+float2(0,(1.0/Pixel_Y)))*BlurWeight[4];
	Color+=tex2D(BoardTexture,input.TextureUV+float2(0,(2.0/Pixel_Y)))*BlurWeight[5];
	Color+=tex2D(BoardTexture,input.TextureUV+float2(0,(3.0/Pixel_Y)))*BlurWeight[6];



	Color=Color/BlurN*2.0;


	Output.Color=Color;

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