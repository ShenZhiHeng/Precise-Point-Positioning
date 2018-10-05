#ifndef PARAMETERDATA_H
#define PARAMETERDATA_H


const double PD_a = 6378137;                 // ���򳤰���
const double PD_e2 = 0.00669437999013;       // ��һƫ���ʵ�ƽ��
const double PD_f = 1 / 298.257223563;       // �������
const double PD_PI = 3.14159265358979;
const double PD_e = 2.718281828459;          // ��Ȼ�����ĵ�e
const double PD_GM = 3.986005E14;            // ������������
const double PD_omega_e = 7.2921151467E-5;   // ������ת���ٶ�
const double PD_c = 2.99792458E8;            // ��չ���
const double PD_R2D = 180 / PD_PI;           // ���Ȼ��ȳ���
const double PD_D2R = PD_PI / 180;           // �Ȼ����ȳ���


// Ƶ��
const double f0 = 10.23;                     // ��λΪ MHZ
const double f1 = 154 * f0;
const double f2 = 120 * f0;
const double f5 = 115 * f0;

// ����
const double L1_Lambda = PD_c / (f1*1e6);    // ��λΪ m
const double L2_Lambda = PD_c / (f2*1e6);
const double L5_Lambda = PD_c / (f5*1e6);




#endif