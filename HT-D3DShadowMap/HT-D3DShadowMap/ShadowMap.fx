
matrix matWorld;
matrix matWorldViewProj;

matrix matWorldViewProjFromLight;//从灯光位置看出来

texture g_Texture;
texture g_DepthTexture;

float g_fFar=30.0f;//思考

sampler MeshTexture=sampler_state
{
	Texture=<g_Texture>;
	MipFilter=LINEAR;
	MinFilter=LINEAR;
	MagFilter=LINEAR;
};

sampler DepthTexture=sampler_state
{
	Texture=<g_DepthTexture>;
	MipFilter=LINEAR;
	MinFilter=LINEAR;
	MagFilter=LINEAR;
};

struct VS_INPUTDEPTH
{
    float4 position  :POSITION;

};
struct VS_OUTPUTDEPTH
{
	float4 position  :POSITION;
	float Depth     :TEXCOORD0;
};

struct VS_INPUT
{
	float4 position  :POSITION;
	float2 TextureUV:TEXCOORD0;

};

struct VS_OUTPUT
{
	float4 position        :POSITION;
	float2 TextureUV       :TEXCOORD0;
	float4 RTTposition     :TEXCOORD1;

};

struct PS_OUTPUT
{
	float4 Color:COLOR0;
};


float4 FloatToTexture( float fIn ) 
  { 
    float fColor =  fIn ;
    // 11111111_11111111_11111111_11111111
    float fR ,fG ,fB ,fA;
    fColor = modf( fColor * 256 ,fR );
    fColor = modf( fColor * 256 ,fG );
    fColor = modf( fColor * 256 ,fB );
    fColor = modf( fColor * 256 , fA );
    return float4( fR / 256 , fG  / 256 , fB  / 256 , fA / 256 );
  }

float TextureToFloat( float2 tex )
  {
    float4 fTexture  = tex2D( DepthTexture ,tex ) ;
    float4 fFactor = float4( 16777216.0 / 16777216.0 ,65536.0 / 16777216.0 ,256.0 / 16777216.0 , 1 / 16777216.0 );
    float  fReturn = dot( fTexture , fFactor );
    return fReturn ;
  }


VS_OUTPUTDEPTH VS_RenderDepth(VS_INPUTDEPTH Input)
{
	VS_OUTPUTDEPTH Output=(VS_OUTPUTDEPTH)0;
	Output.position=mul(Input.position,matWorldViewProjFromLight);
	Output.Depth=Output.position.z/Output.position.w ;//
	return Output;
}

PS_OUTPUT   PS_RenderDepth(VS_OUTPUTDEPTH Input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;
	Input.Depth=Input.Depth;
	Output.Color=FloatToTexture(Input.Depth);

	return Output;

}

VS_OUTPUT RenderSceneVS(VS_INPUT input)
{
	VS_OUTPUT Output= (VS_OUTPUT)0;
	Output.position=mul(input.position,matWorldViewProj);
	Output.TextureUV=input.TextureUV;

	float4 vPosition=mul(input.position,matWorldViewProjFromLight);
	Output.RTTposition=vPosition;

	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;

	float inShadow=1.0f;

	float RealDistance=input.RTTposition.z/input.RTTposition.w;

	//深度纹理上的坐标
	float2 DepthUV=0.5*input.RTTposition.xy/input.RTTposition.w+float2(0.5,0.5);
	DepthUV.y=1.0-DepthUV.y;

	//float TextureDepth=tex2D(DepthTexture,DepthUV).r;
	float TextureDepth=TextureToFloat(DepthUV);
	if(RealDistance-0.005>TextureDepth)
	{
		inShadow=0.5f;

	}
	Output.Color=tex2D(MeshTexture,input.TextureUV)*inShadow;

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

technique Depth
{
	pass P0
	{
		VertexShader=compile vs_2_0 VS_RenderDepth();
		PixelShader =compile ps_2_0 PS_RenderDepth();
	}
}

