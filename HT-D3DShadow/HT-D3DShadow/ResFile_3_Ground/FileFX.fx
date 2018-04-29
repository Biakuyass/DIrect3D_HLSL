//--------------------------------------------------------------------------------------
// File: �ڵ�����.fx
//
//--------------------------------------------------------------------------------------

texture g_ColorTexture;              // Color texture for mesh

float    g_fTime;                   // App's time in seconds
float4x4 g_matWorld;                  // World matrix for object
float4x4 g_matView;
float4x4 g_matProject;
float4x4 g_matWorldNormalInverseTranspose;  // δʹ��
float4x4 g_matMatrixShadow;

float4   g_vEyePosition;                    // δʹ��
float4   g_vLightPosition;

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
    float3 vLight     : TEXCOORD2;       // ģ����ÿ��������ָ���Դ�ķ���    
    float3 vLookAt    : TEXCOORD3;       // ģ����ÿ��������ָ��۲��ߵķ���
  };


VS_OUTPUT RenderSceneVS( VS_INPUT In )
  {
    //
    VS_OUTPUT Out = ( VS_OUTPUT )0;
    float4x4 matWorldView = mul( g_matWorld , g_matView );
    float4x4 matWorldViewProject = mul( matWorldView ,g_matProject );
    Out.Position = mul( In.Position , matWorldViewProject );
    Out.TextureUV = In.TextureUV;
    // ������
    Out.Normal = normalize( mul( In.Normal , ( float3x3 )g_matWorldNormalInverseTranspose ));
    float4 Position = mul( In.Position ,g_matWorld );
    Out.vLight = g_vLightPosition - Position ;
    // ÿ�������ϵ�ָ��۲��ߵķ���
    Out.vLookAt = normalize( Position - g_vEyePosition);
    return Out;
  }

float4 RenderScenePS( VS_OUTPUT In ) :COLOR0
  { 
    float fAmbient = 0.1f;
    // �߹�����
    float4   fSpecularColor  = float4( 1.0 ,  1.0 ,1.0 , 1.0 );
    // �������
    float fDiffuse  = 0.4 * dot( In.Normal, normalize( In.vLight ));
    // �߹�
    float fSpecular = pow( saturate( dot( reflect( normalize( In.vLight ) , In.Normal ), In.vLookAt )), 4 );
    return tex2D( MeshTextureSampler ,In.TextureUV ) * ( fAmbient + fDiffuse + fSpecular * fSpecularColor );
  }

//------------------------------------------------------------------------------
// technique
//------------------------------------------------------------------------------
technique DefaultTech
  {
    pass P0
      {
        /*
        AlphaBlendEnable = true;
        BlendOp          = Add;
        //--------------------------------
        SrcBlend         = SrcColor;
        DestBlend        = InvSrcAlpha;
        //==============================
        AlphaTestEnable    = true;
        */
        FillMode            = Solid;
        VertexShader = compile vs_2_0 RenderSceneVS( );
        PixelShader  = compile ps_2_0 RenderScenePS( );
      }
  }
