//参照ShaderX 7
matrix matWorld;
matrix matWorldViewProj;

texture g_DepthTexture;
texture g_NoiseTexture;

sampler DepthTexture=sampler_state
{
	Texture = <g_DepthTexture>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;


};

sampler NoiseTexture=sampler_state
{
	Texture = <g_NoiseTexture>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
  //  ADDRESSU  = MIRROR;
  //  ADDRESSV  = MIRROR;


};

struct VS_INPUT
{
	float4 Position  : POSITION;
	float2 TextureUV : TEXCOORD0;

};

struct VS_OUTPUT
{
	float4 Position  :POSITION;
	float2 TextureUV :TEXCOORD0;

};

struct PS_OUTPUT
{
	float4 Color:COLOR0;
};

VS_OUTPUT RenderSceneVS(VS_INPUT Input)
{
	VS_OUTPUT Output  = (VS_OUTPUT)0;

	 float temp_x=sign(Input.Position.x);
	 float temp_y=sign(Input.Position.y);
	 Output.Position=float4(temp_x,temp_y,0.5,1.0);
	 Output.TextureUV=Input.TextureUV;
	return Output;
}

PS_OUTPUT RenderScenePS(VS_OUTPUT Input)
{
	PS_OUTPUT Output      =(PS_OUTPUT)0;

	float2 ScreenSize= float2(640,480);
	float2 screenTC = Input.TextureUV;

	//噪音纹理是4*4的，所以这里每4*4的方格循环
    float2 rotationTC = screenTC * ScreenSize/4 ;
    float3 vRotation = 2 * tex2D( NoiseTexture ,rotationTC).rgb-1;
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

	//获得深度值
	float fDepthP=tex2D(DepthTexture,Input.TextureUV).r;


    const int nSamplesNum = 16;
    float offsetScale = 0.01;
    const float offsetScaleStep = 1 + 2.4 / nSamplesNum;
    float Accessibility = 0;

	 for( int i = 0 ; i < ( nSamplesNum / 8 ) ; i ++ )
      {
        for( int x = -1 ; x <= 1; x= x +2 )
          {
            for( int y = -1 ; y <= 1; y = y +2 )
              {
                for( int z = -1 ; z <= 1; z = z+2 )
                  {
					float3 Offset=normalize(float3(x,y,z))*(offsetScale*=offsetScaleStep);
					float3 RoateOffset=mul(Offset,rotMat);

					float3 SamplePos=float3(screenTC,fDepthP);

					SamplePos+=float3(RoateOffset.xy,RoateOffset.z);

					float fDepthS=tex2D(DepthTexture,SamplePos.xy).r;

					//这个点在几何体外面
					if(fDepthS>SamplePos.z)
					    Accessibility+=1;
				    else
				        Accessibility+=0;
 
				}
			}
		}
	}
    Accessibility= Accessibility/nSamplesNum;
	Output.Color=saturate( Accessibility * Accessibility + Accessibility);

	return Output;
}

technique SSAO
{
	pass P0
	{
		CullMode = CCW;

		VertexShader=compile vs_3_0 RenderSceneVS();
		PixelShader =compile ps_3_0 RenderScenePS();
	}
}