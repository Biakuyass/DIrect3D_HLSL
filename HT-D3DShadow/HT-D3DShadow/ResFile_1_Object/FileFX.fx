//--------------------------------------------------------------------------------------
// File: 受光物体.fx
//
//--------------------------------------------------------------------------------------

texture g_ColorTexture;              // Color texture for mesh

float    g_fTime;                   // App's time in seconds
float4x4 g_matWorld;                  // World matrix for object
float4x4 g_matView;
float4x4 g_matProject;

float4x4 g_matWorldNormalInverseTranspose; 
float4x4 g_matWorldToPlane;


float4   g_vEyePosition;            // 观察者位置
float4   g_vLightPosition ;         // 灯光位置

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


struct VS_OUTPUT
  {
    float4 Position   : POSITION;   
    float3 Normal     : TEXCOORD0; 
    float2 TextureUV  : TEXCOORD1;  
    float3 vLight     : TEXCOORD2;       // 模型上每个顶点上指向光源的方向    
    float3 vLookAt    : TEXCOORD3;       // 模型上每个顶点上指向观察者的方向
  };

VS_OUTPUT RenderSceneVS( VS_INPUT In )
  {
    VS_OUTPUT Out = ( VS_OUTPUT )0;
    float4x4 matWorldView = mul( g_matWorld , g_matView );
    float4x4 matWorldViewProject = mul( matWorldView ,g_matProject );
    Out.Position = mul( In.Position , matWorldViewProject );
    Out.TextureUV = In.TextureUV;
    // 法向量
    Out.Normal = normalize( mul( In.Normal , ( float3x3 )g_matWorldNormalInverseTranspose ));
    float4 Position = mul( In.Position ,g_matWorld );
    Out.vLight = g_vLightPosition - Position ;
    // 每个顶点上的指向观察者的方向
    Out.vLookAt = normalize( Position - g_vEyePosition);
    return Out;
  }

float4 RenderScenePS( VS_OUTPUT In ) :COLOR0
  { 
    // 环境光
    float fAmbient = 0.1f;
    // 高光因子
    float4   fSpecularColor  = float4( 1.0 ,  1.0 ,1.0 , 1.0 );
    // 漫反射光
    float fDiffuse  = 0.4 * dot( In.Normal, normalize( In.vLight ));
    // 高光
    float fSpecular = pow( saturate( dot( reflect( normalize( In.vLight ) , In.Normal ), In.vLookAt )), 4 );
    return tex2D( MeshTextureSampler ,In.TextureUV ) * ( fAmbient + fDiffuse + fSpecular * fSpecularColor );
  }

VS_OUTPUT RenderSceneVSShadowToPlane( VS_INPUT In )
  {
    VS_OUTPUT Out = ( VS_OUTPUT )0;
    float4x4 matWorld     = mul( g_matWorld , g_matWorldToPlane );
    float4x4 matWorldView = mul( matWorld , g_matView );
    float4x4 matWorldViewProject = mul( matWorldView ,g_matProject );
    Out.Position = mul( In.Position , matWorldViewProject );
    Out.TextureUV = In.TextureUV;
    return Out;
  }

float4 RenderScenePSShadowToPlane( VS_OUTPUT In ) :COLOR0
  { 
    float4 ColorOut = tex2D( MeshTextureSampler , In.TextureUV );
    return ColorOut;
  }

//------------------------------------------------------------------------------
// technique
//------------------------------------------------------------------------------
technique DefaultTech
  {
    pass P0
      {
        FillMode            = Solid;
        VertexShader = compile vs_2_0 RenderSceneVS( );
        PixelShader  = compile ps_2_0 RenderScenePS( );
      }
  }

technique RenderShadowToPlaneTech
  {
    pass P0
      {
        FillMode            = Solid;
        VertexShader = compile vs_2_0 RenderSceneVSShadowToPlane( );
        PixelShader  = compile ps_2_0 RenderScenePSShadowToPlane( );
      }
  }


