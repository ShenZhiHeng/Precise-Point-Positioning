#ifndef CMATRIX_H
#define CMATRIX_H
#include <vector>

class CMatrix
{
private:
	double** element;
	int Rows;//��
	int Cols;//��
public:
	static int count;

	CMatrix(int mRows=1,int mCols=1);
	CMatrix(double* pArray,int mRows,int mCols);
	CMatrix(std::vector<double> vec);
	CMatrix(const CMatrix& CMat);
	~CMatrix();

	int GetRows()const{ return Rows; }
	int GetCols()const{ return Cols; }
	double GetNum(int x, int y)const;
	void SetNum(int x, int y, double t);
	void SetDiag(std::initializer_list<double> il);
	void SetDiag2(std::initializer_list<double> il);
	CMatrix& operator=(const CMatrix& CMat);
	CMatrix& operator+=(const CMatrix& CMat);
	CMatrix operator+(const CMatrix& CMat)const;
	CMatrix operator-(const CMatrix& CMat)const;
	CMatrix operator-()const;                        // ȡ��
	CMatrix operator*(const CMatrix& CMat)const;     // �������
	CMatrix operator*(int n)const;                   // ����
	CMatrix operator^(int n)const;                   // �ݴη�
	CMatrix Row_Exchange(int m,int n)const;          // ��������
	CMatrix Col_Exchange(int m,int n)const;          // ��������
	CMatrix RowAdd(int m,int n,double t)const;       // ����m�г�t�ӵ���n��
	CMatrix RowMulti(int m,double t)const;           // ��m�г�t
	CMatrix E_Matrix()const;                         // ����һ��ͬ����С�ĵ�λ��
	CMatrix Abs()const;
	CMatrix Transpose()const;
	CMatrix Inverse()const;
	double GetDet()const;
	void Show()const;

	static CMatrix E_Matrix(int m, int n);           // ����һ����λ��
	
};

#endif