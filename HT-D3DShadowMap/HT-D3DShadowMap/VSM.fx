
matrix matWorld;
matrix matWorldViewProj;

matrix matWorldViewProjFromLight;//�ӵƹ�λ�ÿ�����

texture g_Texture;
texture g_DepthTexture;

float g_fFar=30.0f;//˼��

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
	float2 Depth     :TEXCOORD0;
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

    return float4( fIn , fIn*fIn , 1.0 , 0);
  }

/*float TextureToFloat( float2 tex )
  {
    float4 fTexture  = tex2D( DepthTexture ,tex ) ;
    float4 fFactor = float4( 16777216.0 / 16777216.0 ,65536.0 / 16777216.0 ,256.0 / 16777216.0 , 1 / 16777216.0 );
    float  fReturn = dot( fTexture , fFactor );
    return fReturn ;
  }*/


VS_OUTPUTDEPTH VS_RenderDepth(VS_INPUTDEPTH Input)
{
	VS_OUTPUTDEPTH Output=(VS_OUTPUTDEPTH)0;
	Output.position=mul(Input.position,matWorldViewProjFromLight);
	Output.Depth=float2(Output.position.z,Output.position.w);//
	return Output;
}

PS_OUTPUT   PS_RenderDepth(VS_OUTPUTDEPTH Input)
{
	PS_OUTPUT Output=(PS_OUTPUT)0;
	float Depth=Input.Depth.x/Input.Depth.y;
	Output.Color=FloatToTexture(Depth);

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

	//��������ϵ�����
	float2 DepthUV=0.5*input.RTTposition.xy/input.RTTposition.w+float2(0.5,0.5);
	DepthUV.y=1.0-DepthUV.y;

	//Output.Color=tex2D(MeshTexture,input.TextureUV)*inShadow;

	//return Output;

	float2 v4DepthValue=tex2D( DepthTexture ,DepthUV).xy;
	//�б�ѩ�򲻵�ʽ
	//P(x��t) �� �� ^2/(�� ^2+(t-��) ^2)
	//tȥ��ǰ��ת��������ֵ����RealDistance
	float E=v4DepthValue.x;//E(x)
	float E1=v4DepthValue.y;//E(x^2)
	float E2=E*E;//E(x)^2
	float D=E1-E2;
	float TminusE=RealDistance-E;

	//�ȵ�ǰȡ���������ĸ��ʣ�Խ����ӰԽǿ
	float probability=D/(D+(TminusE*TminusE));

	//����������ڵ���ǰ�棬��ȡ����Ӱ״̬
	float variance=max(probability,RealDistance<=E);

	inShadow=variance+0.2;

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

