
texture g_Texture;

float fScale;
float MoveX;
float MoveY;

static float recordX=0;
static float recordY=0;
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

float2 ComplexAdd( float2 v2A , float2 v2B )
  {
    float2 v2Out;
    v2Out.x = v2A.x + v2B.x ;
    v2Out.y = v2A.y + v2B.y ;
    return v2Out;
  }

float2 ComplexMultiply( float2 v2A , float2 v2B )
  {
    float2 v2Out;
    v2Out.x = v2A.x * v2B.x - v2A.y * v2B.y ;
    v2Out.y = v2A.x * v2B.y + v2A.y * v2B.x ;
    return v2Out;
  }

float ComplexMagnitude( float2 v2In )
  {
    return length( v2In ) * length( v2In );
  }

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


PS_OUTPUT RenderScenePS(VS_OUTPUT input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;
	float4 vJuliaColor;
	
	
	//由纹理坐标转换为中心为（0，0）的坐标系
	input.TextureUV.x=(input.TextureUV.x*2-1);
	input.TextureUV.y=-(input.TextureUV.y*2-1);
	//recordX=MoveX*fScale;
//	recordY=MoveY*fScale;
	
	//以已经变换后的点为缩放中心
	input.TextureUV=(input.TextureUV-float2(MoveX,MoveY))*fScale;

	//移动
	input.TextureUV=input.TextureUV+float2(MoveX,MoveY);

	


	

	float2 Coordiate=float2(input.TextureUV.x,input.TextureUV.y);

    int iterate_times=1000;
	float2 JuliaConstant=float2(-0.8, 0.156);
    //Julia分形
	for(int i=0;i<iterate_times;i++)
	{
		Coordiate=ComplexMultiply(Coordiate,Coordiate)+JuliaConstant;

		if(length(Coordiate)>2)
			break;
	}

	//映射用abs(log10(log10(t)))来做
	vJuliaColor=float4(abs(log10(log10(Coordiate.x))),abs(log10(log10(Coordiate.y))),abs(log10(log10((Coordiate.x+Coordiate.y)/2))),1.0);
	Output.Color=vJuliaColor;
	return Output;
}

technique Default
{
	pass P0
	{
		VertexShader=compile vs_3_0 RenderSceneVS();
		PixelShader =compile ps_3_0 RenderScenePS();
	}
}