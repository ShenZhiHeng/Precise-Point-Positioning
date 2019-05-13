#include "NavFile.h"
#include "ObsFile.h"
#include "AidFunc.h"
#include "PFile.h"
#include "Correction.h"
#include "SPP.h"
#include <string>

using namespace std;

void Test()
{
	// ������������
	double t[] = { 1, 2, 3 };
	double x[] = { 1, 2, 3 };
	
	double res = CAid::Lagrange(t, x, 3, 1.5);
	cout << res << endl;
}

void File()
{
	// ����n�ļ�Ѱ������ĵ�������
	SPP spp;
	GPSTime t;
	t.Week = 0;
	t.sow = 259200.0;
	//t.tow.sn = 259200;
	//t.tow.tos = 0.0;
	Cartesian car = { 1.0, 2.0, 3.0 };

	NavFile navdata("FSAS20150722.15n");

	Satellite sat;
	string p = "G01";
	spp.SatPos(navdata.GetNFR(p, t), t, sat);
	cout << sat.Pos.X << endl;
	cout << sat.Pos.Y << endl;
	cout << sat.Pos.Z << endl;

	SP3File sp3file("grg18543.sp3");


	// ObsFile obsdata("FSAS20150722.15o");
}

void Correct()
{
	// ���Զ��������ģ��
	double T = 8.5;
	double P = 649.3;
	double RH = 66.2;
	double h = 3624.6;
	double fai = 29.658;
	// double es=RH*6.11*Math.Pow(10,7.5*T/(T+273.3));

	Troposphere Trop;

	double h_v_p = Trop.Hopfield_Practical(T, P, RH, h);
	double s_v_p = Trop.Saastamoinen_Practical(T, P, RH, fai, h);
	double h_v_s = Trop.Hopfield_Standard(h);
	double s_v_s = Trop.Saastamoinen_Standard(fai, h);
	int a = 0;

}

void Time()
{
	GPSTime t;
	t.Week = 1854;
	t.sow = 259200.0;

	JulianDay JD = Trans::GPST2JLD(t);
	GPSTime t1 = Trans::JLD2GPST(JD);
	int a = 0;
}

void Position()
{
	SPP spp;
	NavFile navdata("nnor1300.18n");
	ObsFile obsdata("nnor1300.18o");
	spp.Cal_Rcv(navdata, obsdata);
	//spp.Sats_Pos(navdata, obsdata.OFR[1], obsdata.OFH.ObsType);

}

void Least()
{
	// ������С����
	CMatrix CB(3, 3);
	CB.SetNum(0, 0, 1);
	CB.SetNum(0, 1, 1);
	CB.SetNum(0, 2, 1);
	CB.SetNum(1, 0, 1);
	CB.SetNum(1, 1, 3);
	CB.SetNum(1, 2, 2);
	CB.SetNum(2, 0, 1.5);
	CB.SetNum(2, 1, 2);
	CB.SetNum(2, 2, 4.5);
	CB.Show();

	CMatrix CP = CMatrix::E_Matrix(3, 3);
	CP.Show();
	CMatrix CL(3, 1);
	CL.SetNum(0, 0, 3);
	CL.SetNum(1, 0, 6);
	CL.SetNum(2, 0, 8);
	CL.Show();

	CMatrix CX0(3, 1);
	CX0.Show();
	CMatrix X(3, 1);
	X.Show();
	//CAid::LeastSquare(CB, CP, CL, X);
	X.Show();
}

void Trans()
{
	Cartesian sat(-15325689.785974950, 14592853.991976338, 16088456.653760601);
	Cartesian sit(0.1, 0.1, 0.1);
	Topocentric topc = Trans::Car2Topc(sit, sat);
	Topopolar topp = Trans::Topc2Topp(topc);
	topp.E *= PD_D2R;
	int a = 0;
}

void Find()
{
	// ����sp3�ļ�����ʱ�����������
	//SP3File sp3("emr20004.sp3");
	//vector<Satellite> Vec_Sat;
	//string PRN = "G20";
	//int n = 6;
	//CalenderTime CT = { 2018, 5, 10, 2, 3, 0.0 };
	//GPSTime GT = Trans::JLD2GPST(Trans::Cal2JLD(CT));
	//double t = GT.sow;
	//Satellite sat;

	//sp3.Find_Sats(Vec_Sat, PRN, n, t);
	//CAid::Lagrange_Sat(Vec_Sat, t, sat);
	//int ff = 20;

	// ����clk�ļ�����ʱ����������ǺͲ�վ
	ClkFile clkfile("emr20004.clk");
	int n = 6;
	double* delta = new double[n];
	double *t = new double[n];
	double t1 = 355502.1;
	string PRN = "G14";
	clkfile.Find_t(delta, t, PRN, n, t1);



	delete[] delta;
	delete[]t;




}

int main()
{
	//Test();
	//Correct();
	//File();
	//Time();
	//Position();
	//Least();
	//Trans();

	//Find();

	//CMatrix P(4, 4);
	//P.SetDiag2(initializer_list<double>({ 1, 2, 3, 4 }));
	//P.Show();

	SPP spp;
	NavFile navdata("nnor1300.18n");
	SP3File sp3("emr20004.sp3");

	ClkFile clkfile("emr20004.clk");
	
	ObsFile obsdata("nnor1300_T.18o");
	//spp.Cal_Rcv_sp3(navdata, sp3, clkfile, obsdata);
	spp.PPP_Static(navdata, sp3, clkfile, obsdata);
	//spp.Cal_Rcv(navdata, obsdata);


	return 0;
}