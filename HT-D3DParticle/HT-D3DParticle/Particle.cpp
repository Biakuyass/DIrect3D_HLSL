#include"Particle.h"

Particle::Particle()
{
	movespeed=D3DXVECTOR3(0,0,0);//测试用

	m_iMaxnumber=MAXNUMBER;   //最大粒子数
	m_Particles=new Particle_individual[m_iMaxnumber];    //粒子池
	for(int i=0;i<m_iMaxnumber;i++)
	{
		//初始时所有粒子都设置为死亡状态
		m_Particles[i].alive=false;
	}
	m_iorderrecord=0;   //初始的下坐标为0
	isStart=false;

	m_Position=D3DXVECTOR3(0,0,0);     //粒子发射源的位置

	//释放状态
	m_state=Par_TimeRelease;



	m_flastupadtetime=0;//初始化上一帧的时间
	m_vLastPosition=m_Position; //初始化上一帧的时间
    m_fnumberpersecond=200;//每秒发射的粒子数
	m_finterval=1000.0f/m_fnumberpersecond;    //粒子发射间隔
	m_flasttime=0;//初始时设置为负间隔时间，保证一开始就能喷出粒子

	//速度
	m_fminspeed=0.015f;    //粒子发射最小速度   
	m_fmaxspeed=0.035f;    //粒子发射最大速度
	//加速度
	m_fminAccelerateSpeedX=0;
	m_fmaxAccelerateSpeedX=0;
	m_fminAccelerateSpeedY=0;
	m_fmaxAccelerateSpeedY=0;
	m_fminAccelerateSpeedZ=0;
	m_fmaxAccelerateSpeedZ=0;

	m_fLifeTime=5000;       //粒子的生命时间



    m_fAccumulatedDistance=0;//初始化累计距离
	m_numberdistance=1; //每移动一单位释放的粒子数
	m_ReleaseDistance=1.0f/m_numberdistance;

	m_fAngleLifeTime=0;       //生命周期旋转的角度

	m_fscaleXLifeTime=10.0f;      //生命周期内X轴的缩放
	m_fscaleYLifeTime=10.0f;      //生命周期内Y轴的缩放
	m_fscaleZLifeTime=10.0f;      //生命周期内Z轴的缩放

	//设置释放形状
	m_ReleaseShape=Par_Circle;
	m_ConeAngle=25;                                    //设置圆锥的角度
	m_fSphereRadius=10.0f;                              //球形和半球形释放的半径
	m_vCubeXYZ=D3DXVECTOR3(5.0f,5.0f,5.0f);              //长方体释放的长宽高




	m_fAlphaLifeTime=0;         //随着生命周期的透明度变化的最终值,默认百分比变换，从1.0开始


	//纹理坐标缩放倍数
	m_fTexture_X=1;
	m_fTexture_Y=1;

	//初始化喷射方向的本地坐标轴
	m_directionY=D3DXVECTOR3(0,1.0,0);    //粒子喷射方向
	D3DXVec3Normalize(&m_directionY,&m_directionY);

	m_directionZ=D3DXVECTOR3(0,0,1.0f);
	D3DXVec3Cross(&m_directionX,&m_directionY,&m_directionZ);
	D3DXVec3Normalize(&m_directionX,&m_directionX);

	D3DXVec3Cross(&m_directionZ,&m_directionX,&m_directionY);
	D3DXVec3Normalize(&m_directionZ,&m_directionZ);

	//要渲染的公告牌
	m_pBillBoard=NULL;
}

Particle::Particle(const Particle & particle_)
{
	movespeed=particle_.movespeed;//测试用

	m_iMaxnumber=particle_.m_iMaxnumber;   //最大粒子数

	m_Particles=new Particle_individual[m_iMaxnumber];    //粒子池
	for(int i=0;i<m_iMaxnumber;i++)
	{
		//初始时所有粒子都设置为死亡状态
		m_Particles[i].alive=false;
	}
	m_iorderrecord=0;   //初始的下坐标为0
	isStart=false;

	m_Position=particle_.m_Position;     //粒子发射源的位置

	//释放状态
	m_state=particle_.m_state;



	m_flastupadtetime=0;//初始化上一帧的时间
	m_vLastPosition=m_Position; //初始化上一帧的时间
	m_fnumberpersecond=particle_.m_fnumberpersecond;//每秒发射的粒子数
	m_finterval=1000.0f/m_fnumberpersecond;    //粒子发射间隔
	m_flasttime=0;//初始时设置为负间隔时间，保证一开始就能喷出粒子


	m_fminspeed=particle_.m_fminspeed;    //粒子发射最小速度   
	m_fmaxspeed=particle_.m_fmaxspeed;    //粒子发射最大速度
	//加速度
	m_fminAccelerateSpeedX=particle_.m_fminAccelerateSpeedX;
	m_fmaxAccelerateSpeedX=particle_.m_fmaxAccelerateSpeedX;
	m_fminAccelerateSpeedY=particle_.m_fminAccelerateSpeedY;
	m_fmaxAccelerateSpeedY=particle_.m_fmaxAccelerateSpeedY;
	m_fminAccelerateSpeedZ=particle_.m_fminAccelerateSpeedZ;
	m_fmaxAccelerateSpeedZ=particle_.m_fmaxAccelerateSpeedZ;


	m_fLifeTime=particle_.m_fLifeTime;       //粒子的生命时间



	m_fAccumulatedDistance=0;//初始化累计距离
	m_numberdistance=particle_.m_numberdistance; //每移动一单位释放的粒子数
	m_ReleaseDistance=1.0f/m_numberdistance;

	m_fAngleLifeTime=particle_.m_fAngleLifeTime;       //生命周期旋转的角度

	m_fscaleXLifeTime=particle_.m_fscaleXLifeTime;      //生命周期内X轴的缩放
	m_fscaleYLifeTime=particle_.m_fscaleYLifeTime;      //生命周期内Y轴的缩放
	m_fscaleZLifeTime=particle_.m_fscaleZLifeTime;      //生命周期内Z轴的缩放

	//设置释放形状
	m_ReleaseShape=particle_.m_ReleaseShape;
	m_ConeAngle=particle_.m_ConeAngle;                                    //设置圆锥的角度
	m_fSphereRadius=particle_.m_fSphereRadius;                              //球形和半球形释放的半径
	m_vCubeXYZ=particle_.m_vCubeXYZ;              //长方体释放的长宽高




	m_fAlphaLifeTime=particle_.m_fAlphaLifeTime;         //随着生命周期的透明度变化的最终值,默认百分比变换，从1.0开始


	//纹理坐标缩放倍数
	m_fTexture_X=particle_.m_fTexture_X;
	m_fTexture_Y=particle_.m_fTexture_Y;

	//初始化喷射方向的本地坐标轴
	m_directionY=particle_.m_directionY;    //粒子喷射方向
	m_directionX=particle_.m_directionX;   
	m_directionZ=particle_.m_directionZ;    


	//要渲染的公告牌
	m_pBillBoard=particle_.m_pBillBoard;    //粒子喷射方向;

}


void Particle::operator=(const Particle & particle_)
{
	movespeed=particle_.movespeed;//测试用

	m_iMaxnumber=particle_.m_iMaxnumber;   //最大粒子数

	if(m_Particles)
	{
	delete [] m_Particles;
	m_Particles=NULL;
	}

	m_Particles=new Particle_individual[m_iMaxnumber];    //粒子池
	for(int i=0;i<m_iMaxnumber;i++)
	{
		//初始时所有粒子都设置为死亡状态
		m_Particles[i].alive=false;
	}
	m_iorderrecord=0;   //初始的下坐标为0
	isStart=false;

	m_Position=particle_.m_Position;     //粒子发射源的位置

	//释放状态
	m_state=particle_.m_state;



	m_flastupadtetime=0;//初始化上一帧的时间
	m_vLastPosition=m_Position; //初始化上一帧的时间
	m_fnumberpersecond=particle_.m_fnumberpersecond;//每秒发射的粒子数
	m_finterval=1000.0f/m_fnumberpersecond;    //粒子发射间隔
	m_flasttime=0;//初始时设置为负间隔时间，保证一开始就能喷出粒子


	m_fminspeed=particle_.m_fminspeed;    //粒子发射最小速度   
	m_fmaxspeed=particle_.m_fmaxspeed;    //粒子发射最大速度

	m_fLifeTime=particle_.m_fLifeTime;       //粒子的生命时间



	m_fAccumulatedDistance=0;//初始化累计距离
	m_numberdistance=particle_.m_numberdistance; //每移动一单位释放的粒子数
	m_ReleaseDistance=1.0f/m_numberdistance;

	m_fAngleLifeTime=particle_.m_fAngleLifeTime;       //生命周期旋转的角度

	m_fscaleXLifeTime=particle_.m_fscaleXLifeTime;      //生命周期内X轴的缩放
	m_fscaleYLifeTime=particle_.m_fscaleYLifeTime;      //生命周期内Y轴的缩放
	m_fscaleZLifeTime=particle_.m_fscaleZLifeTime;      //生命周期内Z轴的缩放

	//设置释放形状
	m_ReleaseShape=particle_.m_ReleaseShape;
	m_ConeAngle=particle_.m_ConeAngle;                                    //设置圆锥的角度
	m_fSphereRadius=particle_.m_fSphereRadius;                              //球形和半球形释放的半径
	m_vCubeXYZ=particle_.m_vCubeXYZ;              //长方体释放的长宽高




	m_fAlphaLifeTime=particle_.m_fAlphaLifeTime;         //随着生命周期的透明度变化的最终值,默认百分比变换，从1.0开始


	//纹理坐标缩放倍数
	m_fTexture_X=particle_.m_fTexture_X;
	m_fTexture_Y=particle_.m_fTexture_Y;

	//初始化喷射方向的本地坐标轴
	m_directionY=particle_.m_directionY;    //粒子喷射方向
	m_directionX=particle_.m_directionX;   
	m_directionZ=particle_.m_directionZ;    


	//要渲染的公告牌
	m_pBillBoard=particle_.m_pBillBoard;    //粒子喷射方向;
}

Particle::~Particle()
{
	delete [] m_Particles;
}



HRESULT Particle::OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,wchar_t* chXFilename,wchar_t* chEffectname,wchar_t* chTexturename)
{
	//BillBoard.OnD3D9CreateDevice(pd3dDevice,chXFilename,chEffectname,chTexturename);
	//初始化字体
	//D3DXCreateFont(pd3dDevice,30,0,0,1,false,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,0,L"微软雅黑",&m_pfont);
	return S_OK;
}

HRESULT Particle::OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText)
{
	//	BillBoard.OnD3D9ResetDevice(pd3dDevice,pBackBufferSurfacfeDesc,pUserText);
		return S_OK;
}

void Particle::OnFrameMove(double fTime, float fElapsedTime,const D3DXMATRIX* pmatWorld,const D3DXMATRIX* pmView,const D3DXMATRIX* pmProj,D3DXVECTOR4 *vEyept)
{
	//起始记录，因为程序初始化时会消耗很多的时间，所以一开始粒子会出现井喷现象和移动到好远的地方去了
	//所以这里用掉一帧来完成初始时间的调整



	//起始时间设置
	if(!isStart)
	{
		isStart=true;
		//起始帧的时间
		m_flastupadtetime=(float)timeGetTime();
		//为了一开始就能放出粒子
		m_flasttime=(float)timeGetTime()-m_finterval;
		return;
	}



	//第一帧可能需要初始化很多东西所以用时较长，需要处理下
	float interval=0;//粒子发射后的时间间隔计算	
	float deltatime=0;//每帧经过的时间

	deltatime=timeGetTime()-m_flastupadtetime;//计算上一帧的时间deltatime
	interval =timeGetTime()-m_flasttime;//计算距离上次发射的时间间隔


	m_flastupadtetime=(float)timeGetTime();

	/*wchar_t temp[50];
	swprintf_s(temp,L"%f",interval);
	MessageBox(NULL,temp,0,0);*/


	//添加粒子且粒子池未满,时间释放
	if(Par_TimeRelease==m_state)
	{
		if(interval>=m_finterval && (int)m_vRenderOrder.size() < m_iMaxnumber)
		{
			int numberofparticle=(int)(interval/m_finterval);//在上一帧时间间隔内应产生的粒子数,粒子数过多时一帧可能需要同时产生很多粒子而不是一个
			for(int i=0;i<numberofparticle;i++)
			{
				//粒子池已满
				if((int)m_vRenderOrder.size()>=m_iMaxnumber)
					break;
				//粒子池未满
				int start=m_iorderrecord;//防bug用
				//遍历寻找能用的池中粒子
				while(true==m_Particles[m_iorderrecord].alive)
				{
					m_iorderrecord++;
					m_iorderrecord=m_iorderrecord%m_iMaxnumber;
					//循环一周bug
					if(m_iorderrecord==start)
					{
						MessageBoxA(NULL,"Particle Error: Pool FULL error",0,0);
					}
				}

				InitParticle_individual(m_Particles[m_iorderrecord]);//初始化记录编号的粒子
				m_vRenderOrder.push_back(m_iorderrecord++);//将该编号放入需编号记录池中
				m_iorderrecord=m_iorderrecord%m_iMaxnumber;
			}
			m_flasttime=(float)timeGetTime();//更新时间
		}
	}

	//距离释放
	if(Par_DistanceRelease==m_state)
	{
		D3DXVECTOR3  temp_vec3=m_Position-m_vLastPosition;
		m_fAccumulatedDistance+=D3DXVec3Length(&temp_vec3);

		m_vLastPosition=m_Position;

		if(m_fAccumulatedDistance >=m_ReleaseDistance && (int)m_vRenderOrder.size() < m_iMaxnumber)
		{
			int numberdistance=int(m_fAccumulatedDistance/m_ReleaseDistance);
			for(int i=0;i<numberdistance;i++)
			{
				//粒子池已满
				if((int)m_vRenderOrder.size()>=m_iMaxnumber)
					break;
				//粒子池未满
				int start=m_iorderrecord;//防bug用
				//遍历寻找能用的池中粒子
				while(true==m_Particles[m_iorderrecord].alive)
				{
					m_iorderrecord++;
					m_iorderrecord=m_iorderrecord%m_iMaxnumber;
					//循环一周bug
					if(m_iorderrecord==start)
					{
						MessageBoxA(NULL,"Particle Error: Pool FULL error",0,0);
					}
				}

				InitParticle_individual(m_Particles[m_iorderrecord]);//初始化记录编号的粒子
				m_vRenderOrder.push_back(m_iorderrecord++);//将该编号放入需编号记录池中
				m_iorderrecord=m_iorderrecord%m_iMaxnumber;
			}
			//累计距离清零
			m_fAccumulatedDistance=0;
		}

	}

	//遍历池中应渲染粒子
	vector<int>::iterator it;
	for(it=m_vRenderOrder.begin();it!=m_vRenderOrder.end();)
	{
		//超过生存时间的情况
		if(timeGetTime()-m_Particles[*it].birthtime>m_Particles[*it].disappeartime)
		{
			//粒子死亡
			m_Particles[*it].alive=false;
			it=m_vRenderOrder.erase(it);//超时消除

			/*wchar_t temp[50];
			swprintf_s(temp,L"%f",timeGetTime()-m_Particles[*it].birthtime);
			MessageBox(NULL,temp,0,0);*/
			continue;
		}

		//仍存在的情况,相关变量更新
		//速度
		m_Particles[*it].velocity=m_Particles[*it].velocity+(m_Particles[*it].acclerate-m_Particles[*it].damp)*deltatime;//更新速度
		//位置
		m_Particles[*it].position+=m_Particles[*it].velocity*deltatime;//更新位置

		//Alpha比例
		m_Particles[*it].Alpha-=m_Particles[*it].AlphaChangeRate*deltatime;
		if(m_Particles[*it].Alpha<=0)
		{
			m_Particles[*it].Alpha=0;
			//MessageBoxA(NULL,"",0,0);
		}


		/*wchar_t temp[50];
		swprintf_s(temp,L"%f,%f,%f",m_Particles[*it].position.x,m_Particles[*it].position.y,m_Particles[*it].position.z);
	
		
        m_pfont->DrawTextW(NULL,temp,-1,&temp_rect,DT_LEFT,D3DCOLOR_RGBA(133,222,222,255));*/
		

		//矩阵变换
		//世界矩阵
		
		D3DXMATRIX matWorld,matTranslation,matRotation,matScaling;
		//缩放 
		m_Particles[*it].scaleX+=m_Particles[*it].m_fscaleXSpeed*deltatime;
		m_Particles[*it].scaleY+=m_Particles[*it].m_fscaleYSpeed*deltatime;
		m_Particles[*it].scaleZ+=m_Particles[*it].m_fscaleZSpeed*deltatime;
		D3DXMatrixScaling(&matScaling,m_Particles[*it].scaleX,m_Particles[*it].scaleY,m_Particles[*it].scaleZ);

	//	m_Particles[*it].position.y=(m_Particles[*it].scaleY-1)*0.25;
		//平移
		D3DXMatrixTranslation(&matTranslation,m_Particles[*it].position.x,m_Particles[*it].position.y,m_Particles[*it].position.z);

		//旋转
		m_Particles[*it].rotation+=m_Particles[*it].palstance*deltatime;
		//旋转的轴为面片的朝向
		D3DXVECTOR3  RotateAxis=m_Particles[*it].position-D3DXVECTOR3(vEyept->x,vEyept->y,vEyept->z);
		D3DXMatrixRotationAxis(&matRotation,&RotateAxis,m_Particles[*it].rotation);

		//按照缩放，旋转，平移的顺序得出世界矩阵
		matWorld=matScaling*matRotation*matTranslation;
		//世界取景投影变换矩阵
		D3DXMATRIX m_matWorldViewProj;
		m_matWorldViewProj=matWorld*(*pmView)*(*pmProj);

	    D3DXVECTOR4 v4BillBoardPosition=D3DXVECTOR4(m_Particles[*it].position.x,m_Particles[*it].position.y,m_Particles[*it].position.z,1.0f);

		m_Particles[*it].matWorld=matWorld;
		m_Particles[*it].matWorldViewProj=m_matWorldViewProj;
		m_Particles[*it].v4Eyept=*vEyept;
		m_Particles[*it].v4Position=v4BillBoardPosition;



		//子粒子系统更新
		for(int i=0;i<m_Particles[*it].ChildParticles_Birth.size();i++)
		{
			m_Particles[*it].ChildParticles_Birth[i].MovetoPosition(m_Particles[*it].position);
			m_Particles[*it].ChildParticles_Birth[i].OnFrameMove(fTime,fElapsedTime,pmatWorld,pmView,pmProj,vEyept);
		}


		//下一个迭代器
		it++;

	}


	//粒子源本身的移动
   m_Position+=movespeed*deltatime;//测试用

}

void Particle::OnD3D9FrameRender(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime, void* pUserContext)
{
	vector<int>::iterator it;
	for(it=m_vRenderOrder.begin();it!=m_vRenderOrder.end();)
	{
		if(m_pBillBoard!=NULL)
		{
		m_pBillBoard->SetAlpha(m_Particles[*it].Alpha);
		m_pBillBoard->SetUVScale(m_Particles[*it].UVScaleX,m_Particles[*it].UVTransX,m_Particles[*it].UVScaleY,m_Particles[*it].UVTransY);
		//BillBoard.SetUVScale(1,0,1,0);
		m_pBillBoard->OnFrameMove(fTime,fElapsedTime,&m_Particles[*it].matWorld,&m_Particles[*it].matWorldViewProj,&m_Particles[*it].v4Eyept,&m_Particles[*it].v4Position);
		m_pBillBoard->OnD3D9FrameRender(pd3dDevice,fTime,fElapsedTime,pUserContext);
		}
		//渲染子粒子系统
		for(int i=0;i<m_Particles[*it].ChildParticles_Birth.size();i++)
		{
			m_Particles[*it].ChildParticles_Birth[i].OnD3D9FrameRender(pd3dDevice,fTime,fElapsedTime,pUserContext);
		}
		it++;
	}
}

void Particle::OnD3D9LostDevice(void* pUserContext)
{
    // BillBoard.OnD3D9LostDevice(pUserContext);
}

void Particle::OnD3D9DestroyDevice(void* pUserContext)
{
//	SAFE_RELEASE(m_pfont);
	//BillBoard.OnD3D9DestroyDevice(pUserContext);
}




//初始化粒子个体的随机量
void Particle::InitParticle_individual(Particle_individual & particle_)
{
	float accelerateX=randrangefloat(m_fminAccelerateSpeedX,m_fmaxAccelerateSpeedX);
	float accelerateY=randrangefloat(m_fminAccelerateSpeedY,m_fmaxAccelerateSpeedY);
	float accelerateZ=randrangefloat(m_fminAccelerateSpeedZ,m_fmaxAccelerateSpeedZ);
	particle_.acclerate=D3DXVECTOR3(accelerateX/100.0,accelerateY/100.0,accelerateZ/100.0);
	particle_.damp=D3DXVECTOR3(0,0,0); 
	particle_.disappeartime=m_fLifeTime;
	particle_.birthtime=(float)timeGetTime();
	particle_.alive=true;

	particle_.scaleX=1;
	particle_.scaleY=1;
	particle_.scaleZ=1;
	particle_.m_fscaleXSpeed=m_fscaleXLifeTime/particle_.disappeartime;
	particle_.m_fscaleYSpeed=m_fscaleYLifeTime/particle_.disappeartime;
	particle_.m_fscaleZSpeed=m_fscaleZLifeTime/particle_.disappeartime;

	particle_.AlphaChangeRate=(1.0f-m_fAlphaLifeTime)/(particle_.disappeartime);
	particle_.Alpha=1.0f;//这里指百分比，不是实际上的Alpha

	//纹理坐标的缩放
	particle_.UVScaleX=m_fTexture_X;
	particle_.UVTransX=rand()%particle_.UVScaleX;
	particle_.UVScaleY=m_fTexture_Y;
	particle_.UVTransY=rand()%particle_.UVScaleY;

	particle_.rotation=0;
	particle_.palstance=m_fAngleLifeTime*((float)PI/180.0f)/particle_.disappeartime;


	float offsetX,offsetY,offsetZ;
	D3DXVECTOR3 vposition;
	switch (m_ReleaseShape)
	{
	case Par_Circle:
		OffSetCircle(offsetX,offsetY,offsetZ,vposition);
		break;
	case Par_HemiSphere:
		OffsetHemiSphere(offsetX,offsetY,offsetZ,vposition);
		break;
	case Par_Sphere:
		OffsetSphere(offsetX,offsetY,offsetZ,vposition);
		break;
	case Par_Cube:
		OffSetCube(offsetX,offsetY,offsetZ,vposition);
		break;
	case Par_Cone:
		OffsetCone(offsetX,offsetY,offsetZ,vposition);
		break;
	case Par_HalfCube:
		OffSetHaflCube(offsetX,offsetY,offsetZ,vposition);
		break;
	default:
		break;
	}
//	OffsetSphere(offsetX,offsetY,offsetZ,vposition);
//	OffsetHemiSphere(offsetX,offsetY,offsetZ,vposition);
//   OffsetCone(offsetX,offsetY,offsetZ,vposition);
//	OffSetHaflCube(offsetX,offsetY,offsetZ,vposition);
//	OffSetCube(offsetX,offsetY,offsetZ,vposition);
//	OffSetCircle(offsetX,offsetY,offsetZ,vposition);


	particle_.movedirection=offsetY*m_directionY+offsetX*m_directionX+offsetZ*m_directionZ;
	//速度方向规格化
	D3DXVec3Normalize(&particle_.movedirection,&particle_.movedirection);
	particle_.position = vposition+m_Position;//相对位移+上粒子源的位置

	float speed = randrangefloat(m_fminspeed,m_fmaxspeed);//计算初始速度
	particle_.velocity = speed*particle_.movedirection;

	//添加子粒子系统
	for(int i=0;i<m_ChildParticles_Birth.size();i++)
	{
		particle_.ChildParticles_Birth.push_back(m_ChildParticles_Birth[i]);
		particle_.ChildParticles_Birth[i].SetPosition(particle_.position);
	}

}

//正方体
void Particle::OffSetCube(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_)
{
	offsetX_    =randrangefloat(-1,1);//计算X的偏移随机量
	offsetY_    =randrangefloat(-1,1);//计算Y的偏移随机量
	offsetZ_    =randrangefloat(-1,1);//计算Z的偏移随机量
	//position_= D3DXVECTOR3(offsetX_*m_vCubeXYZ.x,offsetY_*m_vCubeXYZ.y,offsetZ_*m_vCubeXYZ.z);
	position_= m_vCubeXYZ.x*offsetX_*m_directionX+m_vCubeXYZ.y*offsetY_*m_directionY+m_vCubeXYZ.z*offsetZ_*m_directionZ;

}
//半正方体
void Particle::OffSetHaflCube(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_)
{
	offsetX_    =randrangefloat(-1,1);//计算X的偏移随机量
	offsetY_    =randrangefloat(0,1);//计算Y的偏移随机量
	offsetZ_    =randrangefloat(-1,1);//计算Z的偏移随机量
	position_= m_vCubeXYZ.x*offsetX_*m_directionX+m_vCubeXYZ.y*offsetY_*m_directionY+m_vCubeXYZ.z*offsetZ_*m_directionZ;
		//D3DXVECTOR3(offsetX_*m_vCubeXYZ.x,offsetY_*m_vCubeXYZ.y,offsetZ_*m_vCubeXYZ.z);
}

//球
void Particle::OffsetSphere(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_)
{
	offsetY_=randrangefloat(-1.0f,1.0f);//计算Y的偏移随机量
	float radiosquare=sqrt(1-offsetY_*offsetY_);

	offsetX_=randrangefloat(-radiosquare,radiosquare);

	int choice=rand()%2;
	if(1==choice)
		offsetZ_=sqrt(1-offsetY_*offsetY_-offsetX_*offsetX_);
	else
		offsetZ_=-sqrt(1-offsetY_*offsetY_-offsetX_*offsetX_);

	//position_= D3DXVECTOR3(offsetX_,offsetY_,offsetZ_)*m_fSphereRadius;
	position_= (offsetX_*m_directionX+offsetY_*m_directionY+offsetZ_*m_directionZ)*m_fSphereRadius;
}
//半球
void Particle::OffsetHemiSphere(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_)
{
	offsetY_=randrangefloat(0.0f,1.0f);//计算Y的偏移随机量
	float radiosquare=sqrt(1-offsetY_*offsetY_);

	offsetX_=randrangefloat(-radiosquare,radiosquare);

	int choice=rand()%2;
	if(1==choice)
		offsetZ_=sqrt(1-offsetY_*offsetY_-offsetX_*offsetX_);
	else
		offsetZ_=-sqrt(1-offsetY_*offsetY_-offsetX_*offsetX_);
	//position_= D3DXVECTOR3(offsetX_,offsetY_,offsetZ_)*m_fSphereRadius;
	position_= (offsetX_*m_directionX+offsetY_*m_directionY+offsetZ_*m_directionZ)*m_fSphereRadius;
}
//圆锥
void Particle::OffsetCone(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_)
{
	float angle=(float)PI*m_ConeAngle/180.0f;
	//offsetY_=randrangefloat(cos(angle),1.0f);//计算Y的偏移随机量
	offsetY_=cos(angle);
	float radiosquare=sqrt(1-offsetY_*offsetY_);

	offsetX_=randrangefloat(-radiosquare,radiosquare);

	int choice=rand()%2;
	if(1==choice)
	offsetZ_=sqrt(1-offsetY_*offsetY_-offsetX_*offsetX_);
	else
    offsetZ_=-sqrt(1-offsetY_*offsetY_-offsetX_*offsetX_);
	//position_= D3DXVECTOR3(offsetX_,offsetY_,offsetZ_)*m_fSphereRadius;
	position_= (offsetX_*m_directionX+offsetY_*m_directionY+offsetZ_*m_directionZ)*m_fSphereRadius;
}

//圆
void Particle::OffSetCircle(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_)
{
    offsetX_=0;
	offsetZ_=0;
	offsetY_=1.0f;

	float fX,fZ;
	fX=randrangefloat(-1.0f,1.0f);
	fZ=randrangefloat(-sqrt(1-fX*fX),sqrt(1-fX*fX));


	position_= (fX*m_directionX+fZ*m_directionZ)*m_fSphereRadius;

}


//此函数只能用于0.0001以上的小数随机
float Particle::randrangefloat(float min_,float max_)
{
	if(min_>=max_)
		return max_;
	else
	     return (rand()%(int)((max_-min_)*10000))/10000.0f+min_;
}

void Particle::SetBillBoard(CRenderObject * billboard_)
{
	m_pBillBoard=billboard_;
}


void Particle::SetMaxnumber(int maxnumber_)
{
	m_iMaxnumber=maxnumber_;
	if(m_Particles!=NULL)
		delete [] m_Particles;
	m_Particles=new Particle_individual [m_iMaxnumber];
	for(int i=0;i<m_iMaxnumber;i++)
		m_Particles[i].alive=false;

	m_vRenderOrder.clear();
	m_iorderrecord=0;   //初始的下坐标为0
}
//不算移动
void Particle::SetPosition(D3DXVECTOR3 position_)
{
	m_Position=position_;
	m_vLastPosition=m_Position;

}
//算作移动
void Particle::MovetoPosition(D3DXVECTOR3 position_)
{
	m_Position=position_;
}


void Particle::SetReleaseDirection(D3DXVECTOR3 direction_)
{
	//需要考虑direction=D3DXVECTOR3(0,0,1.0f)的特殊情况
	m_directionY=direction_;    //粒子喷射方向
	D3DXVec3Normalize(&m_directionY,&m_directionY);


	//wchar_t temp[50];
	//swprintf_s(temp,L"%f,%f,%f",m_directionY.x,m_directionY.y,(int)m_directionY.z);
	//MessageBox(NULL,temp,0,0);
	D3DXVECTOR3 tempvec(0,0,1.0f);
	//因为规格化会有误差，所以不能直接用m_directionY==D3DXVECTOR3(0,0,1.0f)
	if(D3DXVec3Dot(&m_directionY,&tempvec)>=0.99)
	{
		m_directionX=D3DXVECTOR3(1.0,0,0);
		D3DXVec3Cross(&m_directionZ,&m_directionX,&m_directionY);
		D3DXVec3Normalize(&m_directionZ,&m_directionZ);


		D3DXVec3Cross(&m_directionX,&m_directionY,&m_directionZ);
		D3DXVec3Normalize(&m_directionX,&m_directionX);

		MessageBoxA(NULL,"SS",0,0);
	}
	else
	{
	  m_directionZ=D3DXVECTOR3(0,0,1.0f);
	  D3DXVec3Cross(&m_directionX,&m_directionY,&m_directionZ);
	  D3DXVec3Normalize(&m_directionX,&m_directionX);

	  D3DXVec3Cross(&m_directionZ,&m_directionX,&m_directionY);
	  D3DXVec3Normalize(&m_directionZ,&m_directionZ);
	//  MessageBoxA(NULL,"SS",0,0);
	}
}

void Particle::SetLifeTime(float life_)
{
	m_fLifeTime=life_;
}

void Particle::SetReleaseState(Particle_ReleaseState state_)
{
	m_state=state_;
}

void Particle::SetNumberPerSecond(float number_)
{

	m_fnumberpersecond=number_;//每秒发射的粒子数
	m_finterval=1000.0f/m_fnumberpersecond;    //粒子发射间隔
	m_flasttime=-m_finterval;//初始时设置为负间隔时间，保证一开始就能喷出粒子
	isStart=false;//重置间隔时间

}

void Particle::SetNumberDistance(float number_)
{
	m_numberdistance=number_; //每移动一单位释放的粒子数
	m_ReleaseDistance=1.0f/m_numberdistance;
}

void Particle::Setvelocity(float minspeed_,float maxspeed_)
{
	m_fminspeed=minspeed_;    //粒子发射最小速度   
	m_fmaxspeed=maxspeed_;    //粒子发射最大速度
}

void Particle::SetRotationLifeTime(float angle_)
{
	m_fAngleLifeTime=angle_;       //生命周期旋转的角度
}

void Particle::SetScaleLifeTime(float scaleX_,float scaleY_,float scaleZ_)
{
	m_fscaleXLifeTime=scaleX_;      //生命周期内X轴的缩放
	m_fscaleYLifeTime=scaleY_;      //生命周期内Y轴的缩放
	m_fscaleZLifeTime=scaleZ_;      //生命周期内Z轴的缩放

}

void Particle::SetAlphaLifeTime(float alpha_)
{
		m_fAlphaLifeTime=alpha_; 
}

void Particle::SetTextureXY(int x_,int y_)
{
	m_fTexture_X=x_;
	m_fTexture_Y=y_;
}

void Particle::SetConeAngle(float angle_)
{
	m_ConeAngle=angle_;     //设置圆锥的角度为45度

}

void Particle::SetRadius(float radius_)
{
	m_fSphereRadius=radius_;                              //球形和半球形释放的半径

}

void Particle::SetCubeXYZ(float x_,float y_,float z_)
{
	m_vCubeXYZ=D3DXVECTOR3(x_,y_,z_);              //长方体释放的长宽高
}

void Particle::SetShape(Particle_ReleaseShape shape_)
{
	m_ReleaseShape=shape_;

}

void Particle::AddParticleSysOnBirth(Particle & particle_)
{
	m_ChildParticles_Birth.push_back(particle_);
}

void Particle::SetAccelerateRangeX(float min_,float max_)
{
	m_fminAccelerateSpeedX=min_;
	m_fmaxAccelerateSpeedX=max_;
}

void Particle::SetAccelerateRangeY(float min_,float max_)
{
	m_fminAccelerateSpeedY=min_;
	m_fmaxAccelerateSpeedY=max_;
}

void Particle::SetAccelerateRangeZ(float min_,float max_)
{
	m_fminAccelerateSpeedZ=min_;
	m_fmaxAccelerateSpeedZ=max_;
}




