/************************************************************************/
/* 

������״������,��Χ������// Բ׶δ���
��ת���� ������Ŀǰ��ת�����ž���������ģʽ,�Ժ������ΪCurveģʽ//���
ʱ���ͷţ������ͷ�//���
����Դ�ĽǶȺ�λ�ƣ����ӵ���ԽǶȺ�λ��//Ŀǰ�Ƕ��õ���3��float��֮��������Կ�ŷ���Ǻ���Ԫ��
���������ڵ��ٶȣ����ٶȣ����ţ�͸���ȵȱ仯 //�Ƕȣ����ţ�͸���������
������,�������//����������
������ϵͳ����������������ײ�� //����������ϵͳ ���
*/
/************************************************************************/

#ifndef PARTICLE_H_
#define  PARTICLE_H_
#include<vector> 
#include<algorithm>
#include<windows.h>
#include"CRenderObject.h"
#include<math.h>

using namespace std;

#pragma comment(lib,"winmm.lib")



#define MAXNUMBER 1000
#define PI        3.14159

class Particle;

struct Particle_individual//���Ӹ���
{

	//ƽ��
	D3DXVECTOR3       position;     
	D3DXVECTOR3       velocity;
	D3DXVECTOR3       movedirection;
	//��ת
	float             rotation;//��Ƭʼ����������������������Ϊƽ����ת�ĽǶ�
	float             palstance;//���ٶ�,��Ƭʼ����������������������Ϊƽ����ת�Ľ��ٶ�
	//����
	float             scaleX;//X���������
	float             scaleY;//Y���������
	float             scaleZ;//Z���������
	float                    m_fscaleXSpeed;      //����������X��������ٶ�
	float                    m_fscaleYSpeed;      //����������Y��������ٶ�
	float                    m_fscaleZSpeed;      //����������Z��������ٶ�
	//����ʱ��
	float             disappeartime;//��ʧʱ��
	float             birthtime;
	bool              alive; //����ʱ����

	//�ƶ��ٶȱ仯
	D3DXVECTOR3       damp;//����
	D3DXVECTOR3       acclerate;

	//TextureUV
	int UVScaleX;
	int UVTransX;
	int UVScaleY;
	int UVTransY;

	//Alpha�仯���ٷֱ�
	float        AlphaChangeRate;
	float        Alpha;

	//���ص�����ϵͳ��������Դϵͳ�м̳�
	vector<Particle> ChildParticles_Birth;

	//���ĸ��ǲ�������Ҫ�õ���
	D3DXMATRIX        matWorld;
	D3DXMATRIX        matWorldViewProj;
	D3DXVECTOR4       v4Position;
	D3DXVECTOR4       v4Eyept;
};

//�����ͷ�״̬
enum Particle_ReleaseState
{
	Par_TimeRelease,
	Par_DistanceRelease

};
//�����ͷ���״
enum Particle_ReleaseShape
{
	Par_Circle,
	Par_Cone,
	Par_Sphere,
	Par_HemiSphere,
	Par_Cube,
	Par_HalfCube,

};


class Particle     //����ϵͳ
{
private:


	int                      m_iMaxnumber;           //���������
	vector<int>              m_vRenderOrder;         //����Ҫ��Ⱦ���������
	bool                     isStart;                //�Ƿ�ʼ�ͷ�

	Particle_individual*     m_Particles;            //���ӳ�
	int                      m_iorderrecord;         //��¼��Ⱦ�����ӵ�������
	//λ��
	D3DXVECTOR3              m_Position;             //���ӷ���Դ��λ��

	//�Ƕ�
	D3DXVECTOR3              m_directionY;           //�������䷽��
	D3DXVECTOR3              m_directionZ;           //�������䷽���Z����
	D3DXVECTOR3              m_directionX;           //�������䷽���X����
	//D3DXVECTOR3              m_EulerAngle;         //����Դ��ŷ���� //������

	//����ʱ��
	float                    m_fLifeTime;            //���ӵ�����ʱ��

	//�ͷ�״̬
	Particle_ReleaseState    m_state;                //���ӵ��ͷ�״̬(ʱ��|����)

	//ʱ���ͷ�
	float                    m_finterval;            //���ӷ�����
	float                    m_fnumberpersecond;     //ÿ��ķ���������
	float                    m_flasttime;            //�ϴ����ӷ����ʱ���¼
	float                    m_flastupadtetime;      //��һ֡���õ�ʱ��

	//�����ͷ�
	float                    m_numberdistance;         //ÿ�ƶ�һ��λ�ͷŵ�������
	float                    m_ReleaseDistance;        //�ͷ�һ����������ľ��룬��m_numberdistance�ĵ���
	D3DXVECTOR3              m_vLastPosition;          //����Դ��һ֡��λ��
	float                    m_fAccumulatedDistance;   //�ۼ��ƶ�����


	//�ƶ��ٶ�
	float                    m_fminspeed;            //���ӷ�����С�ٶ�   
	float                    m_fmaxspeed;            //���ӷ�������ٶ�
	
	//���ٶ�
	float                    m_fminAccelerateSpeedX;
	float                    m_fmaxAccelerateSpeedX;
	float                    m_fminAccelerateSpeedY;
	float                    m_fmaxAccelerateSpeedY;
	float                    m_fminAccelerateSpeedZ;
	float                    m_fmaxAccelerateSpeedZ;


	//��ת
	float                    m_fAngleLifeTime;       //����������ת�ĽǶ�

	//����
	float                    m_fscaleXLifeTime;      //����������X�������
	float                    m_fscaleYLifeTime;      //����������Y�������
	float                    m_fscaleZLifeTime;      //����������Z�������

	//͸���ȱ仯
	float                    m_fAlphaLifeTime;       //�����������ڵ�͸���ȱ仯


	//�����������ű���
	int                    m_fTexture_X;
	int                    m_fTexture_Y;

	//�ͷŵ���״
	Particle_ReleaseShape   m_ReleaseShape;

	//������״�Ĳ���
	float                    m_ConeAngle;             //Բ׶��ƫ�ƽǶ�

	float                    m_fSphereRadius;         //Բ׶��Բ�����κͰ������ͷŵİ뾶

	D3DXVECTOR3              m_vCubeXYZ;              //�������ͷŵĳ����


	//������ϵͳ
	vector<Particle>         m_ChildParticles_Birth;         //������ϵͳ


	//��Ⱦ�õĹ�����
	CRenderObject*           m_pBillBoard;             //���ӱ���(��ͬλ�ö���һ������Ⱦ)

//	ID3DXFont * m_pfont;//������
	
public:
	Particle();
	Particle(const Particle & particle_); //��� 
	void operator=(const Particle & particle_);
	~Particle();
	HRESULT OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,wchar_t* chXFilename,wchar_t* chEffectname,wchar_t* chTexturename);
	HRESULT OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText);
	void OnFrameMove(double fTime, float fElapsedTime,const D3DXMATRIX* pmatWorld,const D3DXMATRIX* pmView,const D3DXMATRIX* pmProj,D3DXVECTOR4 *vEyept);
	void  OnD3D9FrameRender( LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
	void  OnD3D9LostDevice( void* pUserContext );
	void OnD3D9DestroyDevice( void* pUserContext );

	//���ù�����
	void SetBillBoard(CRenderObject * billboard_);
	//�������������
	void SetMaxnumber(int maxnumber_);
	//����λ��,�����ƶ�,�����������ͷ�
	void SetPosition(D3DXVECTOR3 position_);
    //�ƶ������ᴥ�������ͷ�
	void MovetoPosition(D3DXVECTOR3 position_);
	//�����ͷŷ���
	void SetReleaseDirection(D3DXVECTOR3 direction_);
	//��������ʱ��
	void SetLifeTime(float life_);    
	//�����ͷ�״̬
	void SetReleaseState(Particle_ReleaseState state_);
	//����ÿ���ͷŵ�������
	void SetNumberPerSecond(float number_);
	//����ÿ�ƶ�һ��λ�����ͷŵ�������
	void SetNumberDistance (float number_);
	//���������С�ٶ�
	void Setvelocity(float minspeed_,float maxspeed_);
	//��������ʱ����ת�ĽǶ�
	void SetRotationLifeTime(float angle_);
	//��������ʱ���ڵ����ű仯
	void SetScaleLifeTime(float scaleX_,float scaleY_,float scaleZ_);
	//��������ʱ���ڵ�͸���ȱ仯���ٷֱȣ�Ĭ�ϴ�1.0��ʼ��The number set here is the last number of life time
	void SetAlphaLifeTime(float alpha_);
	//�������������
	void SetTextureXY(int x_,int y_);
	//Բ׶�ĽǶ�
	void SetConeAngle(float angle_);
	//�뾶
	void SetRadius(float radius_);
	//���εĳ����
	void SetCubeXYZ(float x_,float y_,float z_);
	//�ͷŵ���״
	void SetShape(Particle_ReleaseShape shape_);

	//���ø��������ϵļ��ٶȷ�Χ
	void SetAccelerateRangeX(float min_,float max_);
	void SetAccelerateRangeY(float min_,float max_);
	void SetAccelerateRangeZ(float min_,float max_);



	//���ӳ���ʱ��ӵ�������ϵͳ
	void AddParticleSysOnBirth(Particle & particle_);


	D3DXVECTOR3      movespeed;//������


private:
	void InitParticle_individual(Particle_individual &);
	void OffsetSphere(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_);
	void OffsetHemiSphere(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_);
	void OffsetCone(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_);
	void OffSetCube(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_);
	void OffSetHaflCube(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_);
	void OffSetCircle(float &offsetX_,float & offsetY_,float &offsetZ_,D3DXVECTOR3 & position_);
	float randrangefloat(float min_,float max_);
};








#endif