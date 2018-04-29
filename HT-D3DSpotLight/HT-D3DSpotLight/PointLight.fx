
matrix matWorld;
matrix matWorldViewProj;
matrix matWorldInverseTranspose;

matrix material;
float4 EyePosition;
float4 LightPosition;

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



	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT Input)
{
	PS_OUTPUT Output      =(PS_OUTPUT)0;

	float fattenuation=1/length(Input.vLight);

//	float4 AmbientMaterial=float4(material._11,material._12,material._13,1.0);
//	float4 AmbientMaterial=float4(0.3,0.3,0.3,1.0);
//	float4 DiffuseMaterial=float4(material._21,material._22,material._23,1.0);
//	float4 SpecularMaterial=float4(material._31,material._32,material._33,1.0);
	float4 AmbientMaterial=float4(1.0,1.0,1.0,1.0);
	float4 DiffuseMaterial=float4(1.0,1.0,1.0,1.0);
	float4 SpecularMaterial=float4(1.0,1.0,1.0,1.0);

	float AmbientPower=1;//环境光强度
	float DiffusePower= saturate(1*dot(Input.Normal,normalize(Input.vLight)));//漫反射的强度
	float SpecularPower=pow(saturate(dot(reflect(normalize(Input.vLight),Input.Normal),Input.vEye)),10);//镜面反射的强度

	float4 Light=AmbientMaterial*AmbientPower+DiffuseMaterial*DiffusePower+SpecularMaterial*SpecularPower;//最终的光照

	Output.Color=Light*fattenuation;

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