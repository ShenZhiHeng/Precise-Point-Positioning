#ifndef TRANS_H
#define TRANS_H

#include "Structs.h"
#include "CMatrix.h"
#include "ParameterData.h"


class Trans
{
public:
	//���������Ϊ ��
	static Cartesian Geo2Car(const Geodetic& geo);                             // �������ת�ѿ�������(B,L,H)����>(X,Y,Z)
	static Geodetic Car2Geo(const Cartesian& car);                             // �ѿ�������ת�������(X,Y,Z)����>(B,L,H)

	static Topocentric Car2Topc(const Cartesian& car1,const Cartesian& car2);  // �ѿ���תվ��������(X,Y,Z)����>(N,E,U)
	// car1��վ������ϵԭ��Ĵ�����꣬car2�Ǵ�ת������
	static Cartesian Topc2Car(const Cartesian& car1, const Topocentric& topc); // վ��������ת�ѿ�������(N,E,U)����>(X,Y,Z)
	// car1��վ������ϵԭ��Ĵ�����꣬topc�Ǵ�ת������
	 
	static Topopolar Topc2Topp(const Topocentric& topc);                       // վ��������תվ�ļ�����
	static Topocentric Topp2Topc(const Topopolar& topp);                       // վ�ļ�����תվ��������



	static JulianDay Cal2JLD(const CalenderTime& CT);   // ͨ��ʱת������
	static CalenderTime JLD2Cal(const JulianDay& JD);   // ������תͨ��ʱ

	static GPSTime JLD2GPST(const JulianDay& JD);       // ������תGPSʱ
	static JulianDay GPST2JLD(const GPSTime& GT);       // GPSʱת������

	static DayofYear JLD2DOY(const JulianDay& JD);      // ������ת�����
	static JulianDay DOY2JLD(const DayofYear& DOY);     // �����ת������

};
#endif