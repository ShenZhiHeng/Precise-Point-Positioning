/******************  ���ܹ���;����Ӳ�  *********************/
#ifndef PFILE_H
#define PFILE_H
#include <fstream>
#include <sstream>
#include "Structs.h"
#include "Trans.h"


/********************  SP3 �ļ���ȡ  ***********************/
class SP3File
{
private:
	string filename;
	SP3 sp3;

public:
	SP3File(){};
	SP3File(const string& filename);
	int ReadFile(const string& filename);

	// ���Ҿ�ʱ��t�����n�����ǣ������������ղ�ֵ 
	int Find_Sats(std::vector<Satellite>& Vec_Sat, string PRN, int n, double t)const;



}; 


/********************  CLK �ļ���ȡ  ***********************/
class ClkFile
{
private:
	string filename;
	CLK clk;
public:
	ClkFile(){}
	ClkFile(const string& filename);
	int ReadFile(const string& filename);

	//  delta_t Ϊ���ҵ��ģ�PRNΪ��ţ�nΪ����+1 ��t1Ϊ�۲�ʱ��
	int Find_t(double *delta_t, double *t, string PRN, int n, double t1)const;

	int Find_ttt(double *delta_t, double *t, string PRN, int n, double t1)const;


};


#endif // !PFILE_H
