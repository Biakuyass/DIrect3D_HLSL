//-----------------------------------------------------------------------
// SSAOЧ���ļ�
// ���
//-----------------------------------------------------------------------
texture g_texDepthTexture;    // �������
texture g_texNoiseMap;        // ��������

sampler g_SceneDepthSampler = sampler_state
  {
    Texture = <g_texDepthTexture>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    ADDRESSU  = Border;//MIRROR;
    ADDRESSV  = Border;
  };

sampler g_RotSampler4x4 = sampler_state
  {
    Texture = <g_texNoiseMap>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
  };

struct VS_INPUT
  {
    float4 pos    : POSITION;
    float4 color  : COLOR0;
    float2 tex    : TEXCOORD0;
  };
// 
struct VS_OUTPUT
  {
    float4 pos     : POSITION;
    float4 color   : COLOR0;
    float2 tex     : TEXCOORD0;
  };

VS_OUTPUT VS_AO( VS_INPUT In )
  {
    VS_OUTPUT Out = ( VS_OUTPUT )0;
    In.pos.x = sign( In.pos.x ) ;
    In.pos.y = sign( In.pos.y ) ;
    Out.pos  = float4( In.pos.x , In.pos.y , 0 , 1 );
    Out.tex       = In.tex;
    Out.color = In.color;
    return Out;
  }

float4 PS_AO( VS_OUTPUT In ) : COLOR0
  {
    float4 v4OutColor = float4( 0.0 , 0.0 , 0.0 , 1.0 );
    const float2  screenSize = float2( 640 , 410 );
    const float farClipDist = 70.0f;
    float2 screenTC = In.tex ;
    // 
    // �õ���ת��������ת����4����Ļ������
    float2 rotationTC = screenTC * screenSize/4 ;
    float3 vRotation = 2 * tex2D( g_RotSampler4x4 ,rotationTC).rgb-1;
    // ����ת������������
    float3x3 rotMat;
    float h = 1 / ( 1 + vRotation.z );
    rotMat._m00 = h * vRotation.y * vRotation.y + vRotation.z;
    rotMat._m01 = h * vRotation.y * vRotation.x ;
    rotMat._m02 = -vRotation.x ;
    rotMat._m10 = -h * vRotation.y * vRotation.x ;
    rotMat._m11 = -h * vRotation.x * vRotation.x + vRotation.z;
    rotMat._m12 = -vRotation.y ;
    rotMat._m20 = vRotation.x;
    rotMat._m21 = vRotation.y ;
    rotMat._m22 = vRotation.z;
    // �õ���ǰ��ȣ�ת������
    float fSceneDepthP = tex2D( g_SceneDepthSampler , screenTC ).r * farClipDist; 
    // paramters affection offset points number an distribution
    const int nSamplesNum = 16;
    float offsetScale = 0.01;
    const float offsetScaleStep = 1 + 2.4 / nSamplesNum;
    float Accessibility = 0;
    // ����������ۻ�
    for( int i = 0 ; i < ( nSamplesNum / 8 ) ; i ++ )
      {
        for( int x = -1 ; x <= 1; x= x +2 )
          {
            for( int y = -1 ; y <= 1; y = y +2 )
              {
                for( int z = -1 ; z <= 1; z = z+2 )
                  {
                    // ����ƫ���������˴�����ֻ�ڱ���׶�ִ��
                    float3 vOffset = normalize( float3( x , y , z )) * ( offsetScale *= offsetScaleStep );
                    // ͨ����ת������תƫ������
                    float3 vRotatedOffset = mul( vOffset , rotMat );
                    // �õ�3D���������λ��
                    float3 vSamplePos = float3( screenTC , fSceneDepthP );
                    // shift
                    vSamplePos += float3( vRotatedOffset.xy , vRotatedOffset.z * fSceneDepthP * 2);
                    // read 
                    float fSceneDepthS = tex2D( g_SceneDepthSampler , vSamplePos.xy ).r * farClipDist;
                    // 
                    //
                    float fRangeIsInvalid = saturate((( fSceneDepthP - fSceneDepthS ) / fSceneDepthS ));
                    //
                     if(fSceneDepthS > vSamplePos.z)
                   Accessibility += lerp( 1, 0.5 , fRangeIsInvalid );
                     else
                     Accessibility += lerp( 0, 0.5 , fRangeIsInvalid );
                   // Accessibility += lerp( fSceneDepthS > fSceneDepthP , 0.5 , fRangeIsInvalid );
                  }
              }
          }
      }
    //
    Accessibility = Accessibility / nSamplesNum ;
    // �ڱ�Ҫ������£��ڽ������źͱ�������
    return saturate( Accessibility * Accessibility + Accessibility );
   // return Accessibility ;
  }


technique techSSAO
  {
    pass Pass0
      {
        VertexShader = compile vs_3_0 VS_AO();
        PixelShader  = compile ps_3_0 PS_AO();
        
        CullMode = CCW;//None;
      }
  }
