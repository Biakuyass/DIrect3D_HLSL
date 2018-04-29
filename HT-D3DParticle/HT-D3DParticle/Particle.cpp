#include"Particle.h"

Particle::Particle()
{
	movespeed=D3DXVECTOR3(0,0,0);//������

	m_iMaxnumber=MAXNUMBER;   //���������
	m_Particles=new Particle_individual[m_iMaxnumber];    //���ӳ�
	for(int i=0;i<m_iMaxnumber;i++)
	{
		//��ʼʱ�������Ӷ�����Ϊ����״̬
		m_Particles[i].alive=false;
	}
	m_iorderrecord=0;   //��ʼ��������Ϊ0
	isStart=false;

	m_Position=D3DXVECTOR3(0,0,0);     //���ӷ���Դ��λ��

	//�ͷ�״̬
	m_state=Par_TimeRelease;



	m_flastupadtetime=0;//��ʼ����һ֡��ʱ��
	m_vLastPosition=m_Position; //��ʼ����һ֡��ʱ��
    m_fnumberpersecond=200;//ÿ�뷢���������
	m_finterval=1000.0f/m_fnumberpersecond;    //���ӷ�����
	m_flasttime=0;//��ʼʱ����Ϊ�����ʱ�䣬��֤һ��ʼ�����������

	//�ٶ�
	m_fminspeed=0.015f;    //���ӷ�����С�ٶ�   
	m_fmaxspeed=0.035f;    //���ӷ�������ٶ�
	//���ٶ�
	m_fminAccelerateSpeedX=0;
	m_fmaxAccelerateSpeedX=0;
	m_fminAccelerateSpeedY=0;
	m_fmaxAccelerateSpeedY=0;
	m_fminAccelerateSpeedZ=0;
	m_fmaxAccelerateSpeedZ=0;

	m_fLifeTime=5000;       //���ӵ�����ʱ��



    m_fAccumulatedDistance=0;//��ʼ���ۼƾ���
	m_numberdistance=1; //ÿ�ƶ�һ��λ�ͷŵ�������
	m_ReleaseDistance=1.0f/m_numberdistance;

	m_fAngleLifeTime=0;       //����������ת�ĽǶ�

	m_fscaleXLifeTime=10.0f;      //����������X�������
	m_fscaleYLifeTime=10.0f;      //����������Y�������
	m_fscaleZLifeTime=10.0f;      //����������Z�������

	//�����ͷ���״
	m_ReleaseShape=Par_Circle;
	m_ConeAngle=25;                                    //����Բ׶�ĽǶ�
	m_fSphereRadius=10.0f;                              //���κͰ������ͷŵİ뾶
	m_vCubeXYZ=D3DXVECTOR3(5.0f,5.0f,5.0f);              //�������ͷŵĳ����




	m_fAlphaLifeTime=0;         //�����������ڵ�͸���ȱ仯������ֵ,Ĭ�ϰٷֱȱ任����1.0��ʼ


	//�����������ű���
	m_fTexture_X=1;
	m_fTexture_Y=1;

	//��ʼ�����䷽��ı���������
	m_directionY=D3DXVECTOR3(0,1.0,0);    //�������䷽��
	D3DXVec3Normalize(&m_directionY,&m_directionY);

	m_directionZ=D3DXVECTOR3(0,0,1.0f);
	D3DXVec3Cross(&m_directionX,&m_directionY,&m_directionZ);
	D3DXVec3Normalize(&m_directionX,&m_directionX);

	D3DXVec3Cross(&m_directionZ,&m_directionX,&m_directionY);
	D3DXVec3Normalize(&m_directionZ,&m_directionZ);

	//Ҫ��Ⱦ�Ĺ�����
	m_pBillBoard=NULL;
}

Particle::Particle(const Particle & particle_)
{
	movespeed=particle_.movespeed;//������

	m_iMaxnumber=particle_.m_iMaxnumber;   //���������

	m_Particles=new Particle_individual[m_iMaxnumber];    //���ӳ�
	for(int i=0;i<m_iMaxnumber;i++)
	{
		//��ʼʱ�������Ӷ�����Ϊ����״̬
		m_Particles[i].alive=false;
	}
	m_iorderrecord=0;   //��ʼ��������Ϊ0
	isStart=false;

	m_Position=particle_.m_Position;     //���ӷ���Դ��λ��

	//�ͷ�״̬
	m_state=particle_.m_state;



	m_flastupadtetime=0;//��ʼ����һ֡��ʱ��
	m_vLastPosition=m_Position; //��ʼ����һ֡��ʱ��
	m_fnumberpersecond=particle_.m_fnumberpersecond;//ÿ�뷢���������
	m_finterval=1000.0f/m_fnumberpersecond;    //���ӷ�����
	m_flasttime=0;//��ʼʱ����Ϊ�����ʱ�䣬��֤һ��ʼ�����������


	m_fminspeed=particle_.m_fminspeed;    //���ӷ�����С�ٶ�   
	m_fmaxspeed=particle_.m_fmaxspeed;    //���ӷ�������ٶ�
	//���ٶ�
	m_fminAccelerateSpeedX=particle_.m_fminAccelerateSpeedX;
	m_fmaxAccelerateSpeedX=particle_.m_fmaxAccelerateSpeedX;
	m_fminAccelerateSpeedY=particle_.m_fminAccelerateSpeedY;
	m_fmaxAccelerateSpeedY=particle_.m_fmaxAccelerateSpeedY;
	m_fminAccelerateSpeedZ=particle_.m_fminAccelerateSpeedZ;
	m_fmaxAccelerateSpeedZ=particle_.m_fmaxAccelerateSpeedZ;


	m_fLifeTime=particle_.m_fLifeTime;       //���ӵ�����ʱ��



	m_fAccumulatedDistance=0;//��ʼ���ۼƾ���
	m_numberdistance=particle_.m_numberdistance; //ÿ�ƶ�һ��λ�ͷŵ�������
	m_ReleaseDistance=1.0f/m_numberdistance;

	m_fAngleLifeTime=particle_.m_fAngleLifeTime;       //����������ת�ĽǶ�

	m_fscaleXLifeTime=particle_.m_fscaleXLifeTime;      //����������X�������
	m_fscaleYLifeTime=particle_.m_fscaleYLifeTime;      //����������Y�������
	m_fscaleZLifeTime=particle_.m_fscaleZLifeTime;      //����������Z�������

	//�����ͷ���״
	m_ReleaseShape=particle_.m_ReleaseShape;
	m_ConeAngle=particle_.m_ConeAngle;                                    //����Բ׶�ĽǶ�
	m_fSphereRadius=particle_.m_fSphereRadius;                              //���κͰ������ͷŵİ뾶
	m_vCubeXYZ=particle_.m_vCubeXYZ;              //�������ͷŵĳ����




	m_fAlphaLifeTime=particle_.m_fAlphaLifeTime;         //�����������ڵ�͸���ȱ仯������ֵ,Ĭ�ϰٷֱȱ任����1.0��ʼ


	//�����������ű���
	m_fTexture_X=particle_.m_fTexture_X;
	m_fTexture_Y=particle_.m_fTexture_Y;

	//��ʼ�����䷽��ı���������
	m_directionY=particle_.m_directionY;    //�������䷽��
	m_directionX=particle_.m_directionX;   
	m_directionZ=particle_.m_directionZ;    


	//Ҫ��Ⱦ�Ĺ�����
	m_pBillBoard=particle_.m_pBillBoard;    //�������䷽��;

}


void Particle::operator=(const Particle & particle_)
{
	movespeed=particle_.movespeed;//������

	m_iMaxnumber=particle_.m_iMaxnumber;   //���������

	if(m_Particles)
	{
	delete [] m_Particles;
	m_Particles=NULL;
	}

	m_Particles=new Particle_individual[m_iMaxnumber];    //���ӳ�
	for(int i=0;i<m_iMaxnumber;i++)
	{
		//��ʼʱ�������Ӷ�����Ϊ����״̬
		m_Particles[i].alive=false;
	}
	m_iorderrecord=0;   //��ʼ��������Ϊ0
	isStart=false;

	m_Position=particle_.m_Position;     //���ӷ���Դ��λ��

	//�ͷ�״̬
	m_state=particle_.m_state;



	m_flastupadtetime=0;//��ʼ����һ֡��ʱ��
	m_vLastPosition=m_Position; //��ʼ����һ֡��ʱ��
	m_fnumberpersecond=particle_.m_fnumberpersecond;//ÿ�뷢���������
	m_finterval=1000.0f/m_fnumberpersecond;    //���ӷ�����
	m_flasttime=0;//��ʼʱ����Ϊ�����ʱ�䣬��֤һ��ʼ�����������


	m_fminspeed=particle_.m_fminspeed;    //���ӷ�����С�ٶ�   
	m_fmaxspeed=particle_.m_fmaxspeed;    //���ӷ�������ٶ�

	m_fLifeTime=particle_.m_fLifeTime;       //���ӵ�����ʱ��



	m_fAccumulatedDistance=0;//��ʼ���ۼƾ���
	m_numberdistance=particle_.m_numberdistance; //ÿ�ƶ�һ��λ�ͷŵ�������
	m_ReleaseDistance=1.0f/m_numberdistance;

	m_fAngleLifeTime=particle_.m_fAngleLifeTime;       //����������ת�ĽǶ�

	m_fscaleXLifeTime=particle_.m_fscaleXLifeTime;      //����������X�������
	m_fscaleYLifeTime=particle_.m_fscaleYLifeTime;      //����������Y�������
	m_fscaleZLifeTime=particle_.m_fscaleZLifeTime;      //����������Z�������

	//�����ͷ���״
	m_ReleaseShape=particle_.m_ReleaseShape;
	m_ConeAngle=particle_.m_ConeAngle;                                    //����Բ׶�ĽǶ�
	m_fSphereRadius=particle_.m_fSphereRadius;                              //���κͰ������ͷŵİ뾶
	m_vCubeXYZ=particle_.m_vCubeXYZ;              //�������ͷŵĳ����




	m_fAlphaLifeTime=particle_.m_fAlphaLifeTime;         //�����������ڵ�͸���ȱ仯������ֵ,Ĭ�ϰٷֱȱ任����1.0��ʼ


	//�����������ű���
	m_fTexture_X=particle_.m_fTexture_X;
	m_fTexture_Y=particle_.m_fTexture_Y;

	//��ʼ�����䷽��ı���������
	m_directionY=particle_.m_directionY;    //�������䷽��
	m_directionX=particle_.m_directionX;   
	m_directionZ=particle_.m_directionZ;    


	//Ҫ��Ⱦ�Ĺ�����
	m_pBillBoard=particle_.m_pBillBoard;    //�������䷽��;
}

Particle::~Particle()
{
	delete [] m_Particles;
}



HRESULT Particle::OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,wchar_t* chXFilename,wchar_t* chEffectname,wchar_t* chTexturename)
{
	//BillBoard.OnD3D9CreateDevice(pd3dDevice,chXFilename,chEffectname,chTexturename);
	//��ʼ������
	//D3DXCreateFont(pd3dDevice,30,0,0,1,false,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,0,L"΢���ź�",&m_pfont);
	return S_OK;
}

HRESULT Particle::OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText)
{
	//	BillBoard.OnD3D9ResetDevice(pd3dDevice,pBackBufferSurfacfeDesc,pUserText);
		return S_OK;
}

void Particle::OnFrameMove(double fTime, float fElapsedTime,const D3DXMATRIX* pmatWorld,const D3DXMATRIX* pmView,const D3DXMATRIX* pmProj,D3DXVECTOR4 *vEyept)
{
	//��ʼ��¼����Ϊ�����ʼ��ʱ�����ĺܶ��ʱ�䣬����һ��ʼ���ӻ���־���������ƶ�����Զ�ĵط�ȥ��
	//���������õ�һ֡����ɳ�ʼʱ��ĵ���



	//��ʼʱ������
	if(!isStart)
	{
		isStart=true;
		//��ʼ֡��ʱ��
		m_flastupadtetime=(float)timeGetTime();
		//Ϊ��һ��ʼ���ܷų�����
		m_flasttime=(float)timeGetTime()-m_finterval;
		return;
	}



	//��һ֡������Ҫ��ʼ���ܶණ��������ʱ�ϳ�����Ҫ������
	float interval=0;//���ӷ�����ʱ��������	
	float deltatime=0;//ÿ֡������ʱ��

	deltatime=timeGetTime()-m_flastupadtetime;//������һ֡��ʱ��deltatime
	interval =timeGetTime()-m_flasttime;//��������ϴη����ʱ����


	m_flastupadtetime=(float)timeGetTime();

	/*wchar_t temp[50];
	swprintf_s(temp,L"%f",interval);
	MessageBox(NULL,temp,0,0);*/


	//������������ӳ�δ��,ʱ���ͷ�
	if(Par_TimeRelease==m_state)
	{
		if(interval>=m_finterval && (int)m_vRenderOrder.size() < m_iMaxnumber)
		{
			int numberofparticle=(int)(interval/m_finterval);//����һ֡ʱ������Ӧ������������,����������ʱһ֡������Ҫͬʱ�����ܶ����Ӷ�����һ��
			for(int i=0;i<numberofparticle;i++)
			{
				//���ӳ�����
				if((int)m_vRenderOrder.size()>=m_iMaxnumber)
					break;
				//���ӳ�δ��
				int start=m_iorderrecord;//��bug��
				//����Ѱ�����õĳ�������
				while(true==m_Particles[m_iorderrecord].alive)
				{
					m_iorderrecord++;
					m_iorderrecord=m_iorderrecord%m_iMaxnumber;
					//ѭ��һ��bug
					if(m_iorderrecord==start)
					{
						MessageBoxA(NULL,"Particle Error: Pool FULL error",0,0);
					}
				}

				InitParticle_individual(m_Particles[m_iorderrecord]);//��ʼ����¼��ŵ�����
				m_vRenderOrder.push_back(m_iorderrecord++);//���ñ�ŷ������ż�¼����
				m_iorderrecord=m_iorderrecord%m_iMaxnumber;
			}
			m_flasttime=(float)timeGetTime();//����ʱ��
		}
	}

	//�����ͷ�
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
				//���ӳ�����
				if((int)m_vRenderOrder.size()>=m_iMaxnumber)
					break;
				//���ӳ�δ��
				int start=m_iorderrecord;//��bug��
				//����Ѱ�����õĳ�������
				while(true==m_Particles[m_iorderrecord].alive)
				{
					m_iorderrecord++;
					m_iorderrecord=m_iorderrecord%m_iMaxnumber;
					//ѭ��һ��bug
					if(m_iorderrecord==start)
					{
						MessageBoxA(NULL,"Particle Error: Pool FULL error",0,0);
					}
				}

				InitParticle_individual(m_Particles[m_iorderrecord]);//��ʼ����¼��ŵ�����
				m_vRenderOrder.push_back(m_iorderrecord++);//���ñ�ŷ������ż�¼����
				m_iorderrecord=m_iorderrecord%m_iMaxnumber;
			}
			//�ۼƾ�������
			m_fAccumulatedDistance=0;
		}

	}

	//��������Ӧ��Ⱦ����
	vector<int>::iterator it;
	for(it=m_vRenderOrder.begin();it!=m_vRenderOrder.end();)
	{
		//��������ʱ������
		if(timeGetTime()-m_Particles[*it].birthtime>m_Particles[*it].disappeartime)
		{
			//��������
			m_Particles[*it].alive=false;
			it=m_vRenderOrder.erase(it);//��ʱ����

			/*wchar_t temp[50];
			swprintf_s(temp,L"%f",timeGetTime()-m_Particles[*it].birthtime);
			MessageBox(NULL,temp,0,0);*/
			continue;
		}

		//�Դ��ڵ����,��ر�������
		//�ٶ�
		m_Particles[*it].velocity=m_Particles[*it].velocity+(m_Particles[*it].acclerate-m_Particles[*it].damp)*deltatime;//�����ٶ�
		//λ��
		m_Particles[*it].position+=m_Particles[*it].velocity*deltatime;//����λ��

		//Alpha����
		m_Particles[*it].Alpha-=m_Particles[*it].AlphaChangeRate*deltatime;
		if(m_Particles[*it].Alpha<=0)
		{
			m_Particles[*it].Alpha=0;
			//MessageBoxA(NULL,"",0,0);
		}


		/*wchar_t temp[50];
		swprintf_s(temp,L"%f,%f,%f",m_Particles[*it].position.x,m_Particles[*it].position.y,m_Particles[*it].position.z);
	
		
        m_pfont->DrawTextW(NULL,temp,-1,&temp_rect,DT_LEFT,D3DCOLOR_RGBA(133,222,222,255));*/
		

		//����任
		//�������
		
		D3DXMATRIX matWorld,matTranslation,matRotation,matScaling;
		//���� 
		m_Particles[*it].scaleX+=m_Particles[*it].m_fscaleXSpeed*deltatime;
		m_Particles[*it].scaleY+=m_Particles[*it].m_fscaleYSpeed*deltatime;
		m_Particles[*it].scaleZ+=m_Particles[*it].m_fscaleZSpeed*deltatime;
		D3DXMatrixScaling(&matScaling,m_Particles[*it].scaleX,m_Particles[*it].scaleY,m_Particles[*it].scaleZ);

	//	m_Particles[*it].position.y=(m_Particles[*it].scaleY-1)*0.25;
		//ƽ��
		D3DXMatrixTranslation(&matTranslation,m_Particles[*it].position.x,m_Particles[*it].position.y,m_Particles[*it].position.z);

		//��ת
		m_Particles[*it].rotation+=m_Particles[*it].palstance*deltatime;
		//��ת����Ϊ��Ƭ�ĳ���
		D3DXVECTOR3  RotateAxis=m_Particles[*it].position-D3DXVECTOR3(vEyept->x,vEyept->y,vEyept->z);
		D3DXMatrixRotationAxis(&matRotation,&RotateAxis,m_Particles[*it].rotation);

		//�������ţ���ת��ƽ�Ƶ�˳��ó��������
		matWorld=matScaling*matRotation*matTranslation;
		//����ȡ��ͶӰ�任����
		D3DXMATRIX m_matWorldViewProj;
		m_matWorldViewProj=matWorld*(*pmView)*(*pmProj);

	    D3DXVECTOR4 v4BillBoardPosition=D3DXVECTOR4(m_Particles[*it].position.x,m_Particles[*it].position.y,m_Particles[*it].position.z,1.0f);

		m_Particles[*it].matWorld=matWorld;
		m_Particles[*it].matWorldViewProj=m_matWorldViewProj;
		m_Particles[*it].v4Eyept=*vEyept;
		m_Particles[*it].v4Position=v4BillBoardPosition;



		//������ϵͳ����
		for(int i=0;i<m_Particles[*it].ChildParticles_Birth.size();i++)
		{
			m_Particles[*it].ChildParticles_Birth[i].MovetoPosition(m_Particles[*it].position);
			m_Particles[*it].ChildParticles_Birth[i].OnFrameMove(fTime,fElapsedTime,pmatWorld,pmView,pmProj,vEyept);
		}


		//��һ��������
		it++;

	}


	//����Դ������ƶ�
   m_Position+=movespeed*deltatime;//������

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
		//��Ⱦ������ϵͳ
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




//��ʼ�����Ӹ���������
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
	particle_.Alpha=1.0f;//����ָ�ٷֱȣ�����ʵ���ϵ�Alpha

	//�������������
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
	//�ٶȷ�����
	D3DXVec3Normalize(&particle_.movedirection,&particle_.movedirection);
	particle_.position = vposition+m_Position;//���λ��+������Դ��λ��

	float speed = randrangefloat(m_fminspeed,m_fmaxspeed);//�����ʼ�ٶ�
	particle_.velocity = speed*particle_.movedirection;

	//���������ϵͳ
	for(int i=0;i<m_ChildParticles_Birth.size();i++)
	{
		particle_.ChildParticles_Birth.push_back(m_ChildParticles_Birth[i]);
		particle_.ChildParticles_Birth[i].SetPosition(particle_.position);
	}

}

//������
void Particle::OffSetCube(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_)
{
	offsetX_    =randrangefloat(-1,1);//����X��ƫ�������
	offsetY_    =randrangefloat(-1,1);//����Y��ƫ�������
	offsetZ_    =randrangefloat(-1,1);//����Z��ƫ�������
	//position_= D3DXVECTOR3(offsetX_*m_vCubeXYZ.x,offsetY_*m_vCubeXYZ.y,offsetZ_*m_vCubeXYZ.z);
	position_= m_vCubeXYZ.x*offsetX_*m_directionX+m_vCubeXYZ.y*offsetY_*m_directionY+m_vCubeXYZ.z*offsetZ_*m_directionZ;

}
//��������
void Particle::OffSetHaflCube(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_)
{
	offsetX_    =randrangefloat(-1,1);//����X��ƫ�������
	offsetY_    =randrangefloat(0,1);//����Y��ƫ�������
	offsetZ_    =randrangefloat(-1,1);//����Z��ƫ�������
	position_= m_vCubeXYZ.x*offsetX_*m_directionX+m_vCubeXYZ.y*offsetY_*m_directionY+m_vCubeXYZ.z*offsetZ_*m_directionZ;
		//D3DXVECTOR3(offsetX_*m_vCubeXYZ.x,offsetY_*m_vCubeXYZ.y,offsetZ_*m_vCubeXYZ.z);
}

//��
void Particle::OffsetSphere(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_)
{
	offsetY_=randrangefloat(-1.0f,1.0f);//����Y��ƫ�������
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
//����
void Particle::OffsetHemiSphere(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_)
{
	offsetY_=randrangefloat(0.0f,1.0f);//����Y��ƫ�������
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
//Բ׶
void Particle::OffsetCone(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_)
{
	float angle=(float)PI*m_ConeAngle/180.0f;
	//offsetY_=randrangefloat(cos(angle),1.0f);//����Y��ƫ�������
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

//Բ
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


//�˺���ֻ������0.0001���ϵ�С�����
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
	m_iorderrecord=0;   //��ʼ��������Ϊ0
}
//�����ƶ�
void Particle::SetPosition(D3DXVECTOR3 position_)
{
	m_Position=position_;
	m_vLastPosition=m_Position;

}
//�����ƶ�
void Particle::MovetoPosition(D3DXVECTOR3 position_)
{
	m_Position=position_;
}


void Particle::SetReleaseDirection(D3DXVECTOR3 direction_)
{
	//��Ҫ����direction=D3DXVECTOR3(0,0,1.0f)���������
	m_directionY=direction_;    //�������䷽��
	D3DXVec3Normalize(&m_directionY,&m_directionY);


	//wchar_t temp[50];
	//swprintf_s(temp,L"%f,%f,%f",m_directionY.x,m_directionY.y,(int)m_directionY.z);
	//MessageBox(NULL,temp,0,0);
	D3DXVECTOR3 tempvec(0,0,1.0f);
	//��Ϊ��񻯻��������Բ���ֱ����m_directionY==D3DXVECTOR3(0,0,1.0f)
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

	m_fnumberpersecond=number_;//ÿ�뷢���������
	m_finterval=1000.0f/m_fnumberpersecond;    //���ӷ�����
	m_flasttime=-m_finterval;//��ʼʱ����Ϊ�����ʱ�䣬��֤һ��ʼ�����������
	isStart=false;//���ü��ʱ��

}

void Particle::SetNumberDistance(float number_)
{
	m_numberdistance=number_; //ÿ�ƶ�һ��λ�ͷŵ�������
	m_ReleaseDistance=1.0f/m_numberdistance;
}

void Particle::Setvelocity(float minspeed_,float maxspeed_)
{
	m_fminspeed=minspeed_;    //���ӷ�����С�ٶ�   
	m_fmaxspeed=maxspeed_;    //���ӷ�������ٶ�
}

void Particle::SetRotationLifeTime(float angle_)
{
	m_fAngleLifeTime=angle_;       //����������ת�ĽǶ�
}

void Particle::SetScaleLifeTime(float scaleX_,float scaleY_,float scaleZ_)
{
	m_fscaleXLifeTime=scaleX_;      //����������X�������
	m_fscaleYLifeTime=scaleY_;      //����������Y�������
	m_fscaleZLifeTime=scaleZ_;      //����������Z�������

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
	m_ConeAngle=angle_;     //����Բ׶�ĽǶ�Ϊ45��

}

void Particle::SetRadius(float radius_)
{
	m_fSphereRadius=radius_;                              //���κͰ������ͷŵİ뾶

}

void Particle::SetCubeXYZ(float x_,float y_,float z_)
{
	m_vCubeXYZ=D3DXVECTOR3(x_,y_,z_);              //�������ͷŵĳ����
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




