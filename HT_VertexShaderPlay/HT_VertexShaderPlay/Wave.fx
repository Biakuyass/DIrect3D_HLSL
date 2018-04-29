
matrix matWorld;
matrix matWorldViewProj;

texture g_Texture;
float g_fTime;


//广告牌用
float4 g_vEye;//眼睛位置
float4 g_vPosition;//广告牌位置

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
	float2 TextureUV:TEXCOORD0;

};

struct VS_OUTPUT
{
	float4 position  :POSITION;
	float2 TextureUV:TEXCOORD0;
	float4 Color    : COLOR0;

};

struct PS_OUTPUT
{
	float4 Color:COLOR0;
};

VS_INPUT ChangeY(VS_INPUT Input,out float outCos)
{
	float4 fZero= float4(0,0,0,0);
    float4 vVector=Input.position-fZero;

	float fAngle=length(vVector)+sin(g_fTime*3)*15;
	//float Scale=sin(fAngle);
	float Sin,Cos;
	sincos(fAngle,Sin,Cos);
	outCos=Cos;


	Input.position.y=Sin*1;
	/*float4 vPosition=Input.position;
	float fTime=g_fTime;

	float fSin, fCos;   
    float x = length( vPosition ) * sin( fTime ) * 15.0f;
    
    // This HLSL intrinsic computes returns both the sine and cosine of x
    sincos( x, fSin, fCos );
                
	Input.position.y=fSin*1;
	outCos=fCos;*/


	return Input;
}


VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
	VS_OUTPUT Output= (VS_OUTPUT)0;

	float Cos;
	//input.position.y=ReturnY_1(input);
	input=ChangeY(input,Cos);

	Output.position=mul(input.position,matWorldViewProj);
	Output.TextureUV=input.TextureUV;
	Output.Color=0.5-Cos*0.5f;

	return Output;
	
}

PS_OUTPUT RenderScenePS(VS_OUTPUT input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;
	//Output.Color=tex2D(MeshTexture,input.TextureUV);
	Output.Color=input.Color;
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
