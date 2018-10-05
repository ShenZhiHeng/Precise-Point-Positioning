#ifndef SPP_H
#define	SPP_H

#include "NavFile.h"
#include "ObsFile.h"
#include "Structs.h"
#include "Correction.h"
#include "PFile.h"
#include "AidFunc.h"

class SPP
{
public:
	vector<Satellite> Sats;
	vector<RCV> Vec_Rcv;


public:
	int SatPos(const NavFileRecord& NFR, GPSTime t, Satellite& sat);
	int SatVel(const NavFileRecord& NFR, GPSTime t, Satellite& sat);  // λ�ò�ּ����ٶ�

	// �����������յķ���ʱ�䣬��������ת���������������
	int Sat_true_PV(const NavFileRecord& NFR, const GPSTime& t, const Cartesian& rcv, Satellite& sat);

	int Sat_true_PV1(const std::vector<Satellite>& Vec_Sat, double* delta_t, double *t, int n, const GPSTime& GT, const Cartesian& rcv, Satellite& sat);

	// ���� ĳһ��Ԫ �������ǵ�λ��
	int Sats_Pos(const NavFile& nav, const ObsFileRecord& OFR, const vector<string>obsType, RCV& rcv);

	// ʹ�þ���������������λ��
	//int Sats_pos_sp3(const NavFile& nav, const SP3File& sp3, const ClkFile& clk, const ObsFileRecord& OFR, const vector<string>obsType);

	// ʹ�����ز���λ�۲�ֵ
	int Sats_Pos_L(const NavFile& nav, const ObsFileRecord& OFR, const vector<string>obsType, RCV& rcv);

	// ��̬PPP����Ԫ����
	int PPP_Static_Single(const NavFile& nav, const SP3File& sp3, const ClkFile&clk, const ObsFileRecord& OFR, const vector<string>obsType, RCV& rcv_last, RCV& rcv);

	// ��̬PPP
	int PPP_Static(const NavFile& nav, const SP3File& sp3, const ClkFile&clk, const ObsFile& obs);

	int Cal_Rcv(const NavFile& nav, const ObsFile& obs);

	int Cal_Rcv_sp3(const NavFile& nav, const SP3File& sp3, const ClkFile&clk, const ObsFile& obs);

};

class PPPKF
{
public:
	int nq, nr;                  // ״̬����������������ά�� 
	CMatrix F, P, H, Q, R;       // ״̬����Ȩ��(�Խ���)���۲�ֵϵ����״̬����(�Խ���)����������(�Խ���)
	CMatrix X, Z;                // ״̬����������ֵ����

	PPPKF(){};
	PPPKF(int nq0, int nr0);

	void Init_PPP(RCV& rcv_last,const std::vector<int>& Vec_PRN);
	
	void SetR(const Cartesian& rcv, const vector<Satellite>& Sats);     // ������������R
	void SetMeas(const std::vector<double>& Vec_l, const CMatrix& B);  // ������������ Z �͹۲�ֵϵ���� H

	void TimeUpdata();           // ʱ�����
	void MeasUpdata(const std::vector<double>& Vec_l, const CMatrix& B);           // �������

};



#endif // !SPP_H
