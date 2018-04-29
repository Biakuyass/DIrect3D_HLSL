
texture g_Texture;
int   choice;
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

	float divide_x=1;
	float divide_y=1;
	float add_x=0;
	float add_y=0;
	int temp_choice=choice;
	switch(abs(temp_choice))
	{
	case 1:
		divide_x=2;
		divide_y=2;
		add_x=-0.5;
		add_y=0.5;
		break;
    case 2:
		divide_x=2;
		divide_y=2;
		add_x=0.5;
		add_y=0.5;
		break;
	case 3:
		divide_x=1;
		divide_y=2;
		add_x=0;
		add_y=-0.5;
		break;

	}
	
	Output.position.x=sign(input.position.x)/divide_x+add_x;
	Output.position.y=sign(input.position.y)/divide_y+add_y;
	Output.position.z=0.5;
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
		VertexShader=compile vs_2_0 RenderSceneVS();
		PixelShader =compile ps_2_0 RenderScenePS();
	}
}