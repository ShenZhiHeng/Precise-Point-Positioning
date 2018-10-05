#ifndef CORRECTION_H
#define CORRECTION_H
#include "ParameterData.h"
#include "Structs.h"

/***************************  ��������ģ��  ***************************/
class Ionosphere
{
public:
	// ԭ��� �����ֵ�����ӳٸ��������ĶԱ��о�-��ѩ�塷
	static double Klobuchar(const double alpha[], const double beta[], const double& gpstime, const Cartesian& Sat, const Cartesian& rcv);
	static double PPModel(const double& P1, const double& P2);
};

/***************************  ���������ģ��  ***************************/
class Troposphere
{
private:
	const double scale = 1.0E-6;
	//��׼�ο�����������Դ��˶ʿ���ġ��й�����GPS���Դ����춥�ӳ��о���Ӧ�á�
	const double T0 = 288.15;
	const double P0 = 1013.25;
	const double e0 = 11.691;
	
	// ���¡���ѹ��ˮ��ѹ
	double T = 0.0;
	double P = 0.0;
	double e = 0.0;

	int mode;                          // ѡ��ģ�ͣ�0 ��ʾ Hopfield ģ�ͣ�1 ��ʾ SAAS ģ�ͣ�2 ��ʾ��ģ��
public:
	static double Correct(int mode);   // δʵ��

	/*********************   HopField ģ��   ***********************/
	// h Ϊ��վ�ĸ̣߳���mΪ��λ��
	double Hopfield_Standard(double h);
	double Hopfield_Practical(double T, double P, double RH, double h);

	/***********************   SAAS ģ��   ***********************/
	// phi Ϊ��վ��γ�ȣ�h Ϊ��վ�ĸ̣߳���mΪ��λ��
	double Saastamoinen_Standard(double phi, double h);
	double Saastamoinen_Practical(double T, double P, double RH, double phi, double h);

	/***********************    ��ģ��   ***********************/
	// E Ϊ��������ڲ�վ�ĸ߶Ƚ�
	static double Simple(const Cartesian& sat,const Cartesian& rcv);

	double e_cal(double RH, double T);  // ���ݲ�վ���ʪ�Ⱥ͸��¼��� ˮ��ѹ
	void Atmosphere_Standard(double h);
};

class Earth_rotation
{
public:
	static Cartesian Correct(double t, Cartesian& X);
};


#endif // !CORRECTION_H
