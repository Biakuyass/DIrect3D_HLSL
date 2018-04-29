/************************************************************************/
/* 

各种形状的喷射,范围内生成// 圆锥未完成
旋转缩放 本程序目前旋转和缩放均采用线性模式,以后可升级为Curve模式//完成
时间释放，距离释放//完成
粒子源的角度和位移，粒子的相对角度和位移//目前角度用的是3个float，之后可以试试看欧拉角和四元数
随生命周期的速度，角速度，缩放，透明度等变化 //角度，缩放，透明度已完成
纹理动画,纹理随机//纹理随机完成
子粒子系统（出生，死亡，碰撞） //出生子粒子系统 完成
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

struct Particle_individual//粒子个体
{

	//平移
	D3DXVECTOR3       position;     
	D3DXVECTOR3       velocity;
	D3DXVECTOR3       movedirection;
	//旋转
	float             rotation;//面片始终正对摄像机，所以这里仅为平面旋转的角度
	float             palstance;//角速度,面片始终正对摄像机，所以这里仅为平面旋转的角速度
	//缩放
	float             scaleX;//X的轴的缩放
	float             scaleY;//Y的轴的缩放
	float             scaleZ;//Z的轴的缩放
	float                    m_fscaleXSpeed;      //生命周期内X轴的缩放速度
	float                    m_fscaleYSpeed;      //生命周期内Y轴的缩放速度
	float                    m_fscaleZSpeed;      //生命周期内Z轴的缩放速度
	//生存时间
	float             disappeartime;//消失时间
	float             birthtime;
	bool              alive; //粒子时候存活

	//移动速度变化
	D3DXVECTOR3       damp;//阻尼
	D3DXVECTOR3       acclerate;

	//TextureUV
	int UVScaleX;
	int UVTransX;
	int UVScaleY;
	int UVTransY;

	//Alpha变化，百分比
	float        AlphaChangeRate;
	float        Alpha;

	//搭载的粒子系统，从粒子源系统中继承
	vector<Particle> ChildParticles_Birth;

	//这四个是布告牌需要用的量
	D3DXMATRIX        matWorld;
	D3DXMATRIX        matWorldViewProj;
	D3DXVECTOR4       v4Position;
	D3DXVECTOR4       v4Eyept;
};

//粒子释放状态
enum Particle_ReleaseState
{
	Par_TimeRelease,
	Par_DistanceRelease

};
//粒子释放形状
enum Particle_ReleaseShape
{
	Par_Circle,
	Par_Cone,
	Par_Sphere,
	Par_HemiSphere,
	Par_Cube,
	Par_HalfCube,

};


class Particle     //粒子系统
{
private:


	int                      m_iMaxnumber;           //最大粒子数
	vector<int>              m_vRenderOrder;         //池内要渲染的粒子序号
	bool                     isStart;                //是否开始释放

	Particle_individual*     m_Particles;            //粒子池
	int                      m_iorderrecord;         //记录渲染到粒子的下坐标
	//位置
	D3DXVECTOR3              m_Position;             //粒子发射源的位置

	//角度
	D3DXVECTOR3              m_directionY;           //粒子喷射方向
	D3DXVECTOR3              m_directionZ;           //粒子喷射方向的Z方向
	D3DXVECTOR3              m_directionX;           //粒子喷射方向的X方向
	//D3DXVECTOR3              m_EulerAngle;         //粒子源的欧拉角 //待补完

	//生命时间
	float                    m_fLifeTime;            //粒子的生命时间

	//释放状态
	Particle_ReleaseState    m_state;                //粒子的释放状态(时间|距离)

	//时间释放
	float                    m_finterval;            //粒子发射间隔
	float                    m_fnumberpersecond;     //每秒的发射粒子数
	float                    m_flasttime;            //上次粒子发射的时间记录
	float                    m_flastupadtetime;      //上一帧所用的时间

	//距离释放
	float                    m_numberdistance;         //每移动一单位释放的粒子数
	float                    m_ReleaseDistance;        //释放一个粒子所需的距离，即m_numberdistance的倒数
	D3DXVECTOR3              m_vLastPosition;          //粒子源上一帧的位置
	float                    m_fAccumulatedDistance;   //累计移动距离


	//移动速度
	float                    m_fminspeed;            //粒子发射最小速度   
	float                    m_fmaxspeed;            //粒子发射最大速度
	
	//加速度
	float                    m_fminAccelerateSpeedX;
	float                    m_fmaxAccelerateSpeedX;
	float                    m_fminAccelerateSpeedY;
	float                    m_fmaxAccelerateSpeedY;
	float                    m_fminAccelerateSpeedZ;
	float                    m_fmaxAccelerateSpeedZ;


	//旋转
	float                    m_fAngleLifeTime;       //生命周期旋转的角度

	//缩放
	float                    m_fscaleXLifeTime;      //生命周期内X轴的缩放
	float                    m_fscaleYLifeTime;      //生命周期内Y轴的缩放
	float                    m_fscaleZLifeTime;      //生命周期内Z轴的缩放

	//透明度变化
	float                    m_fAlphaLifeTime;       //随着生命周期的透明度变化


	//纹理坐标缩放倍数
	int                    m_fTexture_X;
	int                    m_fTexture_Y;

	//释放的形状
	Particle_ReleaseShape   m_ReleaseShape;

	//各种形状的参数
	float                    m_ConeAngle;             //圆锥的偏移角度

	float                    m_fSphereRadius;         //圆锥，圆，球形和半球形释放的半径

	D3DXVECTOR3              m_vCubeXYZ;              //长方体释放的长宽高


	//子粒子系统
	vector<Particle>         m_ChildParticles_Birth;         //子粒子系统


	//渲染用的公告牌
	CRenderObject*           m_pBillBoard;             //粒子本体(不同位置都用一个来渲染)

//	ID3DXFont * m_pfont;//测试用
	
public:
	Particle();
	Particle(const Particle & particle_); //深复制 
	void operator=(const Particle & particle_);
	~Particle();
	HRESULT OnD3D9CreateDevice(LPDIRECT3DDEVICE9 pd3dDevice,wchar_t* chXFilename,wchar_t* chEffectname,wchar_t* chTexturename);
	HRESULT OnD3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice,const D3DSURFACE_DESC *pBackBufferSurfacfeDesc,void *pUserText);
	void OnFrameMove(double fTime, float fElapsedTime,const D3DXMATRIX* pmatWorld,const D3DXMATRIX* pmView,const D3DXMATRIX* pmProj,D3DXVECTOR4 *vEyept);
	void  OnD3D9FrameRender( LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
	void  OnD3D9LostDevice( void* pUserContext );
	void OnD3D9DestroyDevice( void* pUserContext );

	//设置公告牌
	void SetBillBoard(CRenderObject * billboard_);
	//设置最大粒子数
	void SetMaxnumber(int maxnumber_);
	//设置位置,不算移动,不触发距离释放
	void SetPosition(D3DXVECTOR3 position_);
    //移动到，会触发距离释放
	void MovetoPosition(D3DXVECTOR3 position_);
	//设置释放方向
	void SetReleaseDirection(D3DXVECTOR3 direction_);
	//设置生命时间
	void SetLifeTime(float life_);    
	//设置释放状态
	void SetReleaseState(Particle_ReleaseState state_);
	//设置每秒释放的粒子数
	void SetNumberPerSecond(float number_);
	//设置每移动一单位距离释放的粒子数
	void SetNumberDistance (float number_);
	//设置最大最小速度
	void Setvelocity(float minspeed_,float maxspeed_);
	//设置生命时间旋转的角度
	void SetRotationLifeTime(float angle_);
	//设置生命时间内的缩放变化
	void SetScaleLifeTime(float scaleX_,float scaleY_,float scaleZ_);
	//设置生命时间内的透明度变化，百分比，默认从1.0开始，The number set here is the last number of life time
	void SetAlphaLifeTime(float alpha_);
	//纹理坐标的缩放
	void SetTextureXY(int x_,int y_);
	//圆锥的角度
	void SetConeAngle(float angle_);
	//半径
	void SetRadius(float radius_);
	//矩形的长宽高
	void SetCubeXYZ(float x_,float y_,float z_);
	//释放的形状
	void SetShape(Particle_ReleaseShape shape_);

	//设置各个方向上的加速度范围
	void SetAccelerateRangeX(float min_,float max_);
	void SetAccelerateRangeY(float min_,float max_);
	void SetAccelerateRangeZ(float min_,float max_);



	//粒子出生时添加的子粒子系统
	void AddParticleSysOnBirth(Particle & particle_);


	D3DXVECTOR3      movespeed;//测试用


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