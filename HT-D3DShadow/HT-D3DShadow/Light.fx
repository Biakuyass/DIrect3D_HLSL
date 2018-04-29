
matrix matWorld;
matrix matWorldViewProj;

matrix matWorldInverseTrans;//转换法线的矩阵

float4 vEyePosition;
float4 vLightPosition;

matrix matShadowWorldViewProj;

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
	float4 Normal    :NORMAL;
	float2 TextureUV:TEXCOORD0;

};

struct VS_OUTPUT
{
	float4 position      :POSITION;
	float2 TextureUV     :TEXCOORD0;
	float3 Normal        :TEXCOORD1;
	float3 vEyeToVertex  :TEXCOORD2;
	float3 vVertexToLight:TEXCOORD3;

};

struct VS_InputShadow
{
	float4 position  :POSITION;
	float2 TextureUV:TEXCOORD0;
};

struct VS_OutputShadow
{
	float4 position  :POSITION;
};

struct PS_OUTPUT
{
	float4 Color:COLOR0;
};

VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
	VS_OUTPUT Output= (VS_OUTPUT)0;
	float4 Position=mul(input.position,matWorld);

	Output.position=mul(input.position,matWorldViewProj);
	Output.Normal=normalize((mul(input.Normal,matWorldInverseTrans)).xyz);

	Output.vEyeToVertex=normalize((Position-vEyePosition).xyz);
	Output.vVertexToLight=normalize((vLightPosition-Position).xyz);



	Output.TextureUV=input.TextureUV;
	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;
	//默认材质全为1
	float4 Ambient=float4(0.3,0.3,0.3,1.0);
	float4 Specular=float4(1.0,1.0,1.0,1.0);
	float4 Diffuse=float4(0.8,0.8,0.8,1.0);

	//镜面高光强度
	float ReflectPower=pow(saturate(dot(normalize(reflect(input.vVertexToLight,input.Normal)),input.vVertexToLight)),10);

	//漫反射强度
	float DiffusePower=0.4*dot(normalize(input.vVertexToLight),input.Normal);

	float4 LightDensity=ReflectPower*Specular+DiffusePower*Diffuse+Ambient;

	Output.Color=tex2D(MeshTexture,input.TextureUV)*LightDensity;
	return Output;
}


VS_OutputShadow Shadow_VS(VS_InputShadow Input)
{
	VS_OutputShadow Output=(VS_OutputShadow)0;

	Output.position=mul(Input.position,matShadowWorldViewProj);


	return Output;
}

PS_OUTPUT Shadow_PS(VS_OutputShadow Input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;

	Output.Color=float4(0.0,0.0,0.0,0.7f);
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
		StencilFunc=Always;
		StencilRef=0x0f;
		StencilPass=Replace;

		VertexShader=compile vs_2_0 RenderSceneVS();
		PixelShader =compile ps_2_0 RenderScenePS();
	}
}

technique Shadow
{
	pass P0
	{
		AlphaBlendEnable = true;
	    BlendOp          = Add;
		SrcBlend         =SrcColor;
		DestBlend        =DestColor;


		//模板
		StencilEnable=true;
		StencilFunc=Equal;
		StencilRef=0x0f;
		StencilPass=Incr;

		VertexShader=compile vs_2_0 Shadow_VS();
		PixelShader =compile ps_2_0 Shadow_PS();
	}
}
