matrix matWorld;
matrix matWorldViewProj;
matrix matWorldInverseTranspose;

matrix material;

float4 EyePosition;
float4 LightPosition;
float4 vLightDirection;//聚光灯的正对方向

static float g_fConeMin=0.8f;
static float g_fConeMax=0.85;

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
	float4 Position  :POSITION;
	float3 Normal    :NORMAL;
	float2 TextureUV:TEXCOORD0;

};

struct VS_OUTPUT
{
	float4 Position  :POSITION;
	float2 TextureUV:TEXCOORD0;
	float3 Normal    :TEXCOORD1;
	float3 vEye      :TEXCOORD2;//由眼睛指向顶点位置的向量
	float3 vLight    :TEXCOORD3;//由顶点位置指向光源的向量
	float3 pos       :TEXCOORD4;//经过世界变换的顶点位置（非裁剪空间内的坐标）

};

struct PS_OUTPUT
{
	float4 Color:COLOR0;
};

VS_OUTPUT RenderSceneVS(VS_INPUT Input)
{
	VS_OUTPUT Output  = (VS_OUTPUT)0;
	Output.Position   =mul(Input.Position,matWorldViewProj);
	Output.TextureUV  =Input.TextureUV;

	Output.Normal     = normalize(mul(Input.Normal,matWorldInverseTranspose));//法向量的转换
	float4 Position   =mul(Input.Position,matWorld);//顶点位置

	Output.vEye       =normalize(Position-EyePosition);
	Output.vLight     =LightPosition-Position;

	Output.pos        =Position;



	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT Input)
{
	PS_OUTPUT Output      =(PS_OUTPUT)0;

	float fattenuation=1/length(Input.vLight);

//	float4 AmbientMaterial=float4(material._11,material._12,material._13,1.0);
	//float4 AmbientMaterial=float4(0.3,0.3,0.3,1.0);
//	float4 DiffuseMaterial=float4(material._21,material._22,material._23,1.0);
//	float4 SpecularMaterial=float4(material._31,material._32,material._33,1.0);
	float4 AmbientMaterial=float4(1.0,1.0,1.0,1.0);
	float4 DiffuseMaterial=float4(5.0,5.0,5.0,1.0);
	float4 SpecularMaterial=float4(10.0,10.0,10.0,1.0);

	float3 InversevLight=normalize(-Input.vLight);


	float AmbientPower=0.1;//环境光强度
	float DiffusePower=0;
	float SpecularPower=0;

	float  angle=dot(InversevLight,normalize(vLightDirection));
	if(angle>g_fConeMin)
	{
		float fDensity=clamp(angle,g_fConeMin,g_fConeMax)-g_fConeMin;
		fDensity*=30;
	DiffusePower= fDensity*saturate(1*dot(Input.Normal,normalize(Input.vLight)))*fattenuation;//漫反射的强度
	SpecularPower=fDensity*pow(saturate(dot(reflect(normalize(Input.vLight),Input.Normal),Input.vEye)),4)*fattenuation;//镜面反射的强度
	}
	else
	{
		DiffusePower=0;
		SpecularPower=0;
	}
	float4 Light=AmbientMaterial*AmbientPower+DiffuseMaterial*DiffusePower+SpecularMaterial*SpecularPower;//最终的光照

	Output.Color=tex2D(MeshTexture,Input.TextureUV)*Light;

	return Output;
}

technique CommonRender
{
	pass P0
	{
		CullMode = None;

		VertexShader=compile vs_2_0 RenderSceneVS();
		PixelShader =compile ps_2_0 RenderScenePS();
	}
}