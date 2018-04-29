
matrix matWorld;
matrix matWorldViewProj;

texture g_Texture;

struct VS_INPUT
{
	float4 Position  :POSITION;

};

struct VS_OUTPUT
{
	float4 Position  :POSITION;
	float  Depth     :TEXCOORD0;

};

struct PS_OUTPUT
{
	float4 Color:COLOR0;
};

VS_OUTPUT RenderSceneVS(VS_INPUT Input)
{
	VS_OUTPUT Output  = (VS_OUTPUT)0;
	Output.Position   =mul(Input.Position,matWorldViewProj);
	Output.Depth      =Output.Position.z;

	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT Input)
{
	PS_OUTPUT Output      =(PS_OUTPUT)0;
	Output.Color=Input.Depth;
	return Output;
}

technique CommonRender
{
	pass P0
	{
		CullMode = CCW;
		VertexShader=compile vs_2_0 RenderSceneVS();
		PixelShader =compile ps_2_0 RenderScenePS();
	}
}