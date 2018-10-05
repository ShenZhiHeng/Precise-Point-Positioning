#ifndef STRUCTS_H
#define STRUCTS_H
#include <string>
#include <vector>
#include <map>
#include "ParameterData.h"
#include "CMatrix.h"
using namespace std;

/********************************  Define   *******************************/
#define SQR(x)      ((x)*(x))   // ����ƽ��

#define MaxSatNum 85          // ���������

// �����ϵ������
#define gamma    SQR(f1)/SQR(f2)


// ���������ģ��
#define HopField_std  0
#define SAAS_std      1
#define HopField_prac 2    // �����ļ�
#define SAAS_prac     3    // �����ļ�
#define Simple_model  4

// PPP ��������
#define STATIC  0          // ��̬����
#define DYNAMIC 1          // ��̬����



/*******************************  ʱ��ϵͳ  *******************************/
struct TimeofDay    // ��ǰʱ��
{
	int sn;			// ��
	double tos;		// ���С������
	TimeofDay()
	{
		sn=0;
		tos=0;
	}
};

struct TimeofWeek
{
	int sn;          // ��
	double tos;      // ���С������
	TimeofWeek()
	{
		sn=0;
		tos=0;
	}
};

struct CalenderTime  // ͨ��ʱ
{
	int Year;
	int Month;
	int Day;
	int Hour;
	int Minute;
	double Second;
	CalenderTime()
	{
		Year=0;
		Month=0;
		Day=0;
		Hour=0;
		Minute=0;
		Second=0;
	}
	CalenderTime(int a,int b,int c,int d,int e,double f)
	{
		Year = a;
		Month = b;
		Day = c;
		Hour = d;
		Minute = e;
		Second = f;
	}
};

struct JulianDay      // ������
{
	int Day;
	TimeofDay tod;
	JulianDay()
	{
		Day=0;
	}
};

struct DayofYear      // �����
{
	int Year;
	int Day;
	DayofYear()
	{
		Year=0;
		Day=0;
	}
};

struct GPSTime//GPSʱ
{
	int Week;
	double sow;       // ���ڵ���
	//TimeofWeek tow;
	GPSTime()
	{
		Week=0;
	}
	int operator==(GPSTime& GT1)const
	{
		if (Week==GT1.Week&&sow==GT1.sow)
			return 1;
		else
			return 0;
	}
	GPSTime operator+(double t)
	{
		GPSTime res_t;
		res_t = *this;
		res_t.sow += t;
		return res_t;
	}
	GPSTime operator-(double t)
	{
		GPSTime res_t;
		res_t = *this;
		res_t.sow -= t;
		return res_t;
	}
	/*bool operator<(const GPSTime& GT)
	{
		return (Week + (tow.sn + tow.tos) / 604800) < (GT.Week + (GT.tow.sn + GT.tow.tos) / 604800);
	}*/
	friend bool operator<(const GPSTime& GT1, const GPSTime& GT2)
	{
		return (GT1.Week + (GT1.sow) / 604800) < (GT2.Week + (GT2.sow) / 604800);
	}

};

/*******************************  ����ϵͳ  *******************************/
struct Cartesian//�ѿ�������ϵ
{
	double X;
	double Y;
	double Z;
	Cartesian(){}
	Cartesian(double a,double b,double c)
	{
		X = a; Y = b; Z = c;
	}
	Cartesian operator*=(double m)
	{
		X *= m;
		Y *= m;
		Z *= m;
		return *this;
	}
};

struct Geodetic//�������ϵ
{
	double B;
	double L;
	double H;
	Geodetic(){}
	Geodetic(double a, double b, double c)
	{
		B = a; L = b; H = c;
	}
};

struct Topocentric//վ��������ϵ
{
	double N;
	double E;
	double U;
	Topocentric(){}
	Topocentric(double a, double b, double c)
	{
		N = a; E = b; U = c;
	}
};

struct Topopolar//վ�ļ�����ϵ
{
	double S;
	double E;
	double A;
	Topopolar(){}
	Topopolar(double a, double b, double c)
	{
		S = a; E = b; A = c;
	}
};

/**********************�㲥�����ļ�  N �ļ�******************/
struct NavFileHead//���������ļ�ͷ
{
	double version;                  // rinex�汾��
	double ion_alpha[4],ion_beta[4]; // ��������
	double UTC_A0,UTC_A1;            // UTC����
	int UTC_T,UTC_W;                 // UTC����
	int LeapSec;                     // ����
	int HeadLineNum;                 // ͷ�ļ�����
};

struct NavFileRecord//�����������ݼ�¼
{
	//First Line
	string PRN;              // ����PRN��
	CalenderTime CT0;        // �����ӵĲο�ʱ��     ͨ��ʱ
	GPSTime TOC;             // �����ӵĲο�ʱ��     GPSʱ
	double SClickBias;       // �����ӵ�ƫ��(s)
	double SClickDrift;      // �����ӵ�Ư��(s/s)
	double SClickDriftRate;  // �����е�Ư���ٶ�(s/s^2)

	//Second Line
	double IDOE;             // ���ݡ���������ʱ��
	double Crs;              // (m)
	double DeltaN;           // (rad/s)
	double M0;               // (rad)

	//Third Line
	double Cuc;              // (rad)
	double e;                // e���ƫ����
	double Cus;              // (rad)
	double SqrtA;            // (m^(1/2))

	//Fourth Line
	GPSTime TOE;             // �����Ĳο�ʱ�̣�GPS���ڵ�������
	double Cic;              // (rad)
	double OMEGA;            // (rad)
	double Cis;              // (rad)

	//Fifth Line
	double i0;               // (rad)
	double Crc;              // (m)
	double omega1;           // (rad)
	double OMEGADot;         // (rad/s)

	//Sixth Line
	double IDot;             // (rad/s)
	double CodesOnL2Channel; // L2�ϵ���
	double L2PDataFlag;      // L2P������ݱ��

	//Seventh Line
	double SAccuracy;        // ���Ǿ���(m)
	double SHealth;          // ���ǽ���״̬(��һ��֡17~22λ)
	double TGD;              // (sec)
	double IODC;             // IODC�ӵ���������

	//Eighth Line
	double TransTimeOfMsg;   // ���ķ���ʱ��(��λΪGPS�ܵ���)
	double FitInterval;      // �������(h)�����δ֪��Ϊ0
	double spare1;           // ����
	double spare2;           // ����

};

/**********************�۲�ֵ�ļ�  O �ļ�******************/

struct ObsFileHead
{
	double version;          // rinec�汾��
	string system_type;      // ϵͳ����
	string mark_name;        // ���߱�־�����ƣ�������
	string mark_Number;      // ���߱�־���
	string receiverNumber;   // ���ջ����к�
	string receiveType;      // ���ջ�����
	string antennaNumber;    // �������к�
	string antennaType;      // ��������
	Cartesian approxPos;     // ��վ��־�Ľ���λ�ã�WGS84��
	Topocentric antennaDelta;//��U���߸ߣ�E��N������λ��������ڲ�վ��־�ڶ���ͱ����ϵ�ƫ������
	int ObsTypeNumber;       // �۲�ֵ����������������Ÿ�����ʹ������˵��
	vector<string> ObsType;  // ����Ĺ۲�ֵ����
	double interval;         // �۲�ֵ����Ԫ���(s)
	GPSTime startTime;       // �����ļ���һ���۲��¼��ʱ�̣�������ʱ���룩
	GPSTime endTime;         // �����ļ����һ���۲��¼��ʱ�̣�������ʱ���룩
	int LeapSec;             // ��1980��1��6��������������
	int HeadLineNumber;
	
	ObsFileHead()
	{
		ObsTypeNumber=1;
		ObsType.resize(ObsTypeNumber);
	}
};

struct ObsData//ÿ�����ǵĹ۲����ݣ�����ÿ�����͵�����
{
	string PRN;                 // ���ǵ�PRN��
	vector<double> SingleData;  // ��������
	ObsData(){}
	ObsData(int ObsTypeNumber)
	{
		SingleData.resize(ObsTypeNumber);
	}
};
 
struct ObsFileRecordHead
{
	CalenderTime CT;             // �۲���Ԫ��ͨ��ʱ
	GPSTime GT;                  // �۲���Ԫ��GPSʱ
	int epoch_Mark;              // ��Ԫ��ʶ
	int SatNum;                  // ����Ԫ��������
};

struct ObsFileRecord//һ��ʱ�̵����ݣ�����ͷ�ļ���ÿ�����ǵ�����
{
	ObsFileRecordHead OFRH;       // ͷ�ļ�
	vector<ObsData> Obs_Data;     // ÿ�����ǵ�����
	ObsFileRecord(){}
	ObsFileRecord(int SatNum)
	{
		Obs_Data.resize(SatNum);
	}
};

struct Satellite       // ���� 
{
	string PRN;
	GPSTime t;
	Cartesian Pos;
	Cartesian Vel;
	double delta_t;               // �Ӳ�
	double ClkDrift;              // ��Ư
};

struct RCV              // ��վ
{
	GPSTime t;
	Cartesian Pos;
	Cartesian Vel;
	double delta_t;
	int ValidSatNum;
	string PRN;        // ��վ����
	vector<double> N;  // 32
	double PDOP;
	CMatrix X;
	CMatrix Qxx;
	CMatrix D;  // Э������󣬼��������˲��е� P ��

};
/**********************���ܹ���ļ�  SP3 �ļ�******************/
struct SP3
{
	// ��һ�� 
	string mark_ver;              // �汾��ʶ��
	char modeFlag;                // λ�ã�P��  λ���ٶȣ�V��
	CalenderTime CT;              // �����������Ԫ
	int EpochNum;                 // ��Ԫ����
	string DataType;              // ��������
	string coordFrame;            // ������
	string orbitType;             // �������
	string sourceAgency;          // ��������

	// �ڶ���
	GPSTime GT;                   // �����������Ԫ
	double interval;              // ��Ԫ���
	int JLD;                      // ����Ԫ�����յ���������
	double JLD_D;                 // ����Ԫ�����յ�С������

	// ������ �� ��ʮ����
	int SatNum = 0;               // ��������
	string PRN[MaxSatNum];        // ����PRN��
	int SatAccur[MaxSatNum];      // ���Ǿ���

	// ������
	map<GPSTime, map<string, Satellite>> data;
	
};

/**********************�����Ӳ��ļ�  CLK �ļ�******************/
struct Sat_Sta_CLK                  // ���ǻ��վ���Ӳ����
{
	GPSTime GT;
	string PRN;                     // ��վ�������ǵı�ʶ�Ż�PRN��
	double delta_t;                 // �Ӳ�
	double clk_drift;               // ����
	friend bool operator<(const Sat_Sta_CLK& s, const double& t)
	{
		return (s.GT.sow < t);
	}
};


struct CLK
{
	double version;                  // rinex�汾��
	int LeapSec;                     // ����
	int data_type;                   // �������ͣ�1 ��ʾֻ���Ӳ2 ��ʾ���Ӳ������
	int StaNum;                      // ��վ����
	map<string, Cartesian>Sta;       // ��վ������
	int SatNum;                      // ��������
	vector<string>Vec_PRN;           // ����PRN��

	vector<Sat_Sta_CLK> Sat_data;       // ������Ԫ�������ǵ�����
	vector<Sat_Sta_CLK> Sta_data;       // ������Ԫ���в�վ������


};


#endif