//--------------------------------------------------------------------------------------
// File: 受光物体.fx
//
//--------------------------------------------------------------------------------------
float    g_fFar = 40;

texture g_ColorTexture;              // Color texture for mesh

float    g_fTime;                   // App's time in seconds
float4x4 g_matWorld;                  // World matrix for object
float4x4 g_matView;
float4x4 g_matProject;

float4x4 g_matWorldNormalInverseTranspose; 
float4   g_vEyePosition;            // 观察者位置
float4   g_vLightPosition ;         // 灯光位置

float4x4 g_matWorldRTT;             // 渲染到纹理使用的世界矩阵
float4x4 g_matViewRTT;              // 渲染到纹理使用的观察矩阵
float4x4 g_matProjectRTT;           // 渲染到纹理使用的投影矩阵


sampler MeshTextureSampler = sampler_state
  {
    Texture = <g_ColorTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
  };

//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_INPUT
  {
    float4 Position   : POSITION;  
    float3 Normal     : NORMAL; 
    float2 TextureUV  : TEXCOORD0;  
  };

struct VS_OUTPUTDS
  {
    float4 Position  : POSITION;
    float3 PositionInTex : TEXCOORD0;
    float3 NormalInTex   : TEXCOORD1;
    float  Depth         : TEXCOORD2;
  };

//-------- 渲染位置到纹理 ------------
/*
VS_OUTPUTDS RenderPositionVS( VS_INPUT In )
  {
    VS_OUTPUTDS Out = ( VS_OUTPUTDS ) 0 ;
    float4x4 matWorldView = mul( g_matWorldRTT , g_matViewRTT );
    float4x4 matWorldViewProject = mul( matWorldView , g_matProjectRTT );
    Out.Position = mul( In.Position , matWorldViewProject );
    Out.PositionInTex =  Out.Position ;
    Out.NormalInTex =  In.Normal ;
    return Out;
  }

float4 RenderPositionPS( VS_OUTPUTDS In ): COLOR0
  {
    return float4( In.PositionInTex , 1.0 );
  }
//-------- 渲染法向量到纹理 ------------
VS_OUTPUTDS RenderNormalVS( VS_INPUT In )
  {
    VS_OUTPUTDS Out = ( VS_OUTPUTDS ) 0 ;
    float4x4 matWorldView = mul( g_matWorldRTT , g_matViewRTT );
    float4x4 matWorldViewProject = mul( matWorldView , g_matProjectRTT );
    Out.Position = mul( In.Position , matWorldViewProject );
    Out.PositionInTex =  Out.Position ;
    Out.NormalInTex =  In.Normal ;
    return Out;
  }

float4 RenderNormalPS( VS_OUTPUTDS In ): COLOR0
  {
    return float4( In.NormalInTex , 1.0 );
  }
*/
//-------- 渲染深度到纹理 ------------
VS_OUTPUTDS RenderDepthVS( VS_INPUT In )
  {
    VS_OUTPUTDS Out = ( VS_OUTPUTDS ) 0 ;
    float4x4 matWorldView = mul( g_matWorldRTT , g_matViewRTT );
    float4x4 matWorldViewProject = mul( matWorldView , g_matProjectRTT );
    Out.Position = mul( In.Position , matWorldViewProject );
    Out.PositionInTex =  Out.Position ;
    Out.NormalInTex =  In.Normal ;
    Out.Depth = Out.Position.z ;
    return Out;
  }

float4 RenderDepthPS( VS_OUTPUTDS In ): COLOR
  {
    float fDepth = In.Depth ;
    return fDepth;
  }

//------------------------------------------------------------------------------
// technique
//------------------------------------------------------------------------------
/*
technique RenderNormalTech
  {
    pass P0
      {
        FillMode  = Solid;
        VertexShader = compile vs_2_0 RenderNormalVS( );
        PixelShader  = compile ps_2_0 RenderNormalPS( );
      }
  }
*/
technique RenderDepthTech
  {
    pass P0
      {
        VertexShader = compile vs_2_0 RenderDepthVS( );
        PixelShader  = compile ps_2_0 RenderDepthPS( );
      }
  }
