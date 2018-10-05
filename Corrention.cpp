#include "Correction.h"
#include "Trans.h"
#include <cmath>

/***************************** ������ӳټ��� *********************************/
double Ionosphere::Klobuchar(const double alpha[], const double beta[], const double& gpstime,const Cartesian& Sat, const Cartesian& rcv)
{
	double	dIonoDelay = 0;                        // unit: meter

	Geodetic rcv_Geo = Trans::Car2Geo(rcv);
	double phi_u = rcv_Geo.B / 180;            // ���ջ�γ��
	double  lambda_u = rcv_Geo.L / 180;         // ���ջ�����
	double  FAIi = 0;                              // �²������γ��
	double	FAIm = 0;                              // �ش�γ��
	double	LAMBDAi = 0;                           // �²�����㾭��

	Topopolar Sat_Topp = Trans::Topc2Topp(Trans::Car2Topc(rcv, Sat));
	double E = Sat_Topp.E / 180;
	double A = Sat_Topp.A / 180;

	double	fai = 0;                               // ���Ľ�
	double	t = 0;                                 // ȫ���Ĺ۲���Ԫ������
	double  F = 0;                                 // ��б����
	double	AMP = 0;                                 
	double	PER = 0;
	double  x = 0;  

	fai = 0.0137 / (E + 0.11) - 0.022;
	FAIi = phi_u + fai * cos(A);
	if (FAIi > 0.416)
	{
		FAIi = 0.416;
	}                            // if(FAIi > 0.416)
	if (FAIi < -0.416)
	{
		FAIi = -0.416;
	}   

	if (lambda_u > 1 && lambda_u <= 2) // 180 < LAMDAi < 360
	{
		lambda_u -= 2;
	}                          
	LAMBDAi = lambda_u + fai * sin(A) / cos(FAIi);
	FAIm = FAIi + 0.064 * cos(LAMBDAi - 1.617);
	t = 4.32e4 * LAMBDAi + gpstime;
	if (t >= 86400)
	{
		t -= 86400;
	}
	if (t < 0)
	{
		t += 86400;
	}

	F = 1.0 + 16 * pow(0.53 - E, 3);

	AMP = alpha[0] + alpha[1] * FAIm + alpha[2] * FAIm * FAIm + alpha[3] * FAIm * FAIm * FAIm;
	if (AMP < 1e-10)
	{
		AMP = 0;
	}

	PER = beta[0] + beta[1] * FAIm + beta[2] * FAIm * FAIm + beta[3] * FAIm * FAIm * FAIm;
	if (PER < 72000)
	{
		PER = 72000;
	}

	x = 2 * PD_PI * (t - 50400) / PER;
	if (fabs(x) >= 1.57)
	{
		dIonoDelay = F * 5E-9 * PD_c;
	}// if(fabs(x) >= 1.57)
	else
	{
		dIonoDelay = F * (5E-9 + AMP * (1 - x * x / 2.0 + pow(x, 4) / 24.0)) * PD_c;
	}// else 

	return dIonoDelay;
}

double Ionosphere::PPModel(const double& P1, const double& P2)
{
	return 1.54573 * (P1 - P2);
}

/***************************** �������ӳټ��� *********************************/
double Troposphere::Correct(int mode)
{
	return 0;
}


double Troposphere::e_cal(double RH, double T)
{
	// RHΪ���ʪ�ȣ����ݲ�վ�����ʪ�Ⱥ͸��¼���ˮ��ѹ
	// ��ʽ��Դ�ڡ�GPS���������ݴ����ڶ���P109
	double es = 0.0;
	double Ts = T + 273.15;
	es = RH * exp(-37.2465 + 0.213166 * Ts - 0.000256908 * Ts * Ts) / 100;
	return es;
}

void  Troposphere::Atmosphere_Standard(double h)
{
	// ��ʽ��Դ��˶ʿ���ġ��й�����GPS���Դ����춥�ӳ��о���Ӧ�á�
	// �ο����ô��²���˹���˴�ѧ������DIPOP��������ü���
	// hΪ��վ�߶ȣ���mΪ��λ��
	T = T0 - 0.0068 * h;
	P = P0*pow((1 - 0.0068*h / T0), 5);
	if (h < 11000)
		e = e0 * pow((1 - 0.0068 * h / T0), 4);
	else
		e = 0.0;
}

double Troposphere::Hopfield_Standard(double h)
{
	// ��ʵ�����������Hopfieldģ�Ͷ������춥�ӳټ���
	// ����Ԫ�زο����ô��²���˹���˴�ѧ������DIPOP��������ü���
	// hΪ��վ�߶ȣ���mΪ��λ��
	Atmosphere_Standard(h);
	double k1 = 77.6;
	double k2 = 71.6;
	double k3 = 3.747E5;
	double Hw = 11000;
	double Ht = 40136 + 148.72 *(T - 273.15);

	double ZTD = scale * (k1 * P * (Ht - h) / T + (k3 + 273 * (k2 - k1)) * (Hw - h) * e / (T * T)) / 5;
	return ZTD;
}

double Troposphere::Hopfield_Practical(double T, double P, double RH, double h)
{
	// �¶�TΪʵ������϶ȣ�PΪ������ѹ��hPa����RHΪ���ʪ�ȣ�hΪ��վ�߶ȣ���mΪ��λ����
	// ������ʵ������Ԫ�ص������Hopfieldģ�͵Ķ������춥�ӳ�
	// ��ʽ��Դ��˶ʿ���ġ��й�����GPS���Դ����춥�ӳ��о���Ӧ�á�
	double Ts = T + 273.15;
	double k1 = 77.6;
	double k2 = 71.6;
	double k3 = 3.747E5;
	double Hw = 11000;
	double Ht = 40136 + 148.72 * T;
	double es = e_cal(RH, T);

	double ZTD = scale *(k1*P*(Ht - h) / Ts + (k3 + 273 * (k2 - k1))*(Hw - h)*es / (Ts*Ts)) / 5;
	return ZTD;
}

double Troposphere::Saastamoinen_Standard(double phi, double h)
{
	// ��ʵ�����������Saastamoinenģ�Ͷ������춥�ӳټ���
	// ����Ԫ�زο����ô��²���˹���˴�ѧ������DIPOP��������ü���
	// hΪ��վ�߶ȣ���mΪ��λ��
	Atmosphere_Standard(h);
	h = h / 1000.0;

	double radian = PD_PI * phi / 180.0;

	double f = 1 - 0.00266 * cos(2 * radian) - 0.00028 * h;
	double ZTD = 0.002277 * (P + (0.05 + 1255 / T) * e) / f;
	return ZTD;
}

double Troposphere::Saastamoinen_Practical(double T, double P, double RH, double phi, double h)
{
	// �¶�TΪʵ������϶ȣ�PΪ������ѹ��hPa����RHΪ���ʪ�ȣ�phiΪ��վγ�ȣ��ԡ�Ϊ��λ����
	// hΪ��վ�߶ȣ���mΪ��λ����������ʵ������Ԫ�ص������Hopfieldģ�͵Ķ������춥�ӳ�
	h = h / 1000.0;
	double Ts = T + 273.15;
	double radian = PD_PI*phi / 180.0;
	double es = e_cal(RH, T);

	double f = 1 - 0.00266 * cos(2 * radian) - 0.00028 * h;
	double ZTD = 0.002277*(P + (0.05 + 1255 / Ts)*es) / f;
	return ZTD;
}

double Troposphere::Simple(const Cartesian& sat, const Cartesian& rcv)
{
	Topopolar Sat_Topp = Trans::Topc2Topp(Trans::Car2Topc(rcv, sat));
	double E = Sat_Topp.E *PD_D2R;
	double ZTD;
	ZTD = 2.47 / (sin(E) + 0.0121);
	return ZTD;
}

/***************************  ��ת����  ******************************/
Cartesian Earth_rotation::Correct(double t, Cartesian& X)
{
	X.X = sin(PD_omega_e*t)*X.Y + cos(PD_omega_e*t)*X.X;
	X.Y = -sin(PD_omega_e*t)*X.X + cos(PD_omega_e*t)* X.Y;
	X.Z = X.Z;
	return X;
}
