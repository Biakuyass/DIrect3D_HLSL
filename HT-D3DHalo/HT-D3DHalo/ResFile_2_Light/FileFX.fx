//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------

texture g_ColorTexture;              // Color texture for mesh

float    g_fTime;                   // App's time in seconds
float4x4 g_matWorld;                  // World matrix for object
float4x4 g_matView;
float4x4 g_matProject;

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
    float2 TextureUV  : TEXCOORD0;  
  };


struct VS_OUTPUT
  {
    float4 Position   : POSITION;   
    float2 TextureUV  : TEXCOORD0;  
    //float2 TextureAB  : TEXCOORD1;
  };

VS_OUTPUT RenderSceneVS( VS_INPUT In )
  {
    VS_OUTPUT Out = ( VS_OUTPUT )0;
    float4 v4PositionIn;
    v4PositionIn = float4( In.Position.x , In.Position.y , In.Position.z , 1.0 );
    float4x4 matWorldView = mul( g_matWorld ,g_matView );
    float4x4 matWorldViewProject = mul( matWorldView ,g_matProject );
    Out.Position = mul( v4PositionIn , matWorldViewProject );
    Out.TextureUV = In.TextureUV;
    //Out.TextureAB = In.TextureUV + g_fTime;
    return Out;
  }

float4 RenderScenePS( VS_OUTPUT In ) :COLOR0
  { 
    float4 v4ColorOut1 = tex2D( MeshTextureSampler ,In.TextureUV ) ;
    //float4 v4ColorOut2 = tex2D( MeshTextureSampler ,In.TextureAB ) ;
    //return ( v4ColorOut1 * v4ColorOut2 ) * 2.0;
    return v4ColorOut1 ;
  }

//------------------------------------------------------------------------------
// technique
//------------------------------------------------------------------------------
technique DefaultTech
  {
    pass P0
      {
        
        FillMode            = Solid;
        //ZEnable             = true;
        ZFunc               = Less;
        ZWriteEnable        = true;
        CullMode            = CCW;
        
        VertexShader = compile vs_2_0 RenderSceneVS( );
        PixelShader  = compile ps_2_0 RenderScenePS( );
      }
  }
