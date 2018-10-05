#include "ObsFile.h"

ObsFile::ObsFile()
{
}

ObsFile::ObsFile(const string& filename)
{
	ReadFile(filename);
}

ObsFile::~ObsFile()
{
}

int ObsFile::ReadFile(const string& filename)
{
	ifstream infile(filename);
	if (!infile.is_open())
	{
		cout << "��ȡʧ��" << endl;
		return 0;
	}

	int i, j, k;
	string LineBuffer;
	stringstream ss;
	OFH.HeadLineNumber = 0;
	/*********��ȡͷ�ļ�**********/
	do
	{
		getline(infile, LineBuffer);
		OFH.HeadLineNumber++;
		ss.clear();
		ss.str("");
		ss << LineBuffer;

		if (LineBuffer.find("RINEX VERSION / TYPE", 20) != string::npos)
		{
			ss >> OFH.version;
			//cout<<OFH.version;
		}
		if (LineBuffer.find("MARKER NAME", 11) != string::npos)
		{
			ss >> OFH.mark_name;
		}
		else if (LineBuffer.find("MARKER NUMBER", 13) != string::npos)
		{
			ss >> OFH.mark_Number;
		}
		else if (LineBuffer.find("REC # / TYPE / VERS", 19) != string::npos)
		{
			ss >> OFH.receiverNumber;
			ss >> OFH.receiveType;
		}
		else if (LineBuffer.find("ANT # / TYPE", 12) != string::npos)
		{
			ss >> OFH.antennaNumber;
			ss >> OFH.antennaType;
		}
		else if (LineBuffer.find("APPROX POSITION XYZ", 19) != string::npos)
		{
			ss >> OFH.approxPos.X;
			ss >> OFH.approxPos.Y;
			ss >> OFH.approxPos.Z;
		}
		else if (LineBuffer.find("ANTENNA: DELTA H/E/N", 20) != string::npos)
		{
			ss >> OFH.antennaDelta.U;
			ss >> OFH.antennaDelta.E;
			ss >> OFH.antennaDelta.N;
		}
		else if (LineBuffer.find("# / TYPES OF OBSERV", 19) != string::npos)
		{
			ss >> OFH.ObsTypeNumber;
			OFH.ObsType.resize(OFH.ObsTypeNumber);
			int a = int(OFH.ObsTypeNumber / 9);
			if (a == 0)//С��9�����ͣ����û���
			{
				for (i = 0; i<OFH.ObsTypeNumber; i++)
				{
					OFH.ObsType[i] = LineBuffer.substr(10 + 6 * i, 2);
				}
			}
			if (a == 1)//���У����ڵ���9�����ͣ�ĩβ���������ͬ
			{
				for (i = 0; i<9; i++)
				{
					OFH.ObsType[i] = LineBuffer.substr(10 + 6 * i, 2);
				}
				getline(infile, LineBuffer);
				OFH.HeadLineNumber++;
				for (i = 9; i<OFH.ObsTypeNumber; i++)
				{
					OFH.ObsType[i] = LineBuffer.substr(10 + 6 * (i - 9), 2);
				}
			}
			if (a == 2)//����
			{
				for (i = 0; i<9; i++)
				{
					OFH.ObsType[i] = LineBuffer.substr(10 + 6 * i, 2);
				}
				getline(infile, LineBuffer);
				OFH.HeadLineNumber++;
				for (i = 9; i<18; i++)
				{
					OFH.ObsType[i] = LineBuffer.substr(10 + 6 * (i - 9), 2);
				}
				getline(infile, LineBuffer);
				OFH.HeadLineNumber++;
				for (i = 18; i<OFH.ObsTypeNumber; i++)
				{
					OFH.ObsType[i] = LineBuffer.substr(10 + 6 * (i - 18), 2);
				}
			}
		}
		else if (LineBuffer.find("INTERVAL", 8) != string::npos)
		{
			ss >> OFH.interval;
		}
		else if (LineBuffer.find("TIME OF FIRST OBS", 17) != string::npos)
		{
			CalenderTime CT;
			ss >> CT.Year;
			ss >> CT.Month;
			ss >> CT.Day;
			ss >> CT.Hour;
			ss >> CT.Minute;
			ss >> CT.Second;
			if (CT.Year >= 80 && CT.Year <= 99)
			{
				CT.Year = 1900 + CT.Year;
			}
			else if (CT.Year<80 && CT.Year >= 0)
			{
				CT.Year = 2000 + CT.Year;
			}
			OFH.startTime = Trans::JLD2GPST(Trans::Cal2JLD(CT));
		}
		else if (LineBuffer.find("TIME OF LAST OBS", 16) != string::npos)
		{
			CalenderTime CT;
			ss >> CT.Year;
			ss >> CT.Month;
			ss >> CT.Day;
			ss >> CT.Hour;
			ss >> CT.Minute;
			ss >> CT.Second;
			if (CT.Year >= 80 && CT.Year <= 99)
			{
				CT.Year = 1900 + CT.Year;
			}
			else if (CT.Year<80 && CT.Year >= 0)
			{
				CT.Year = 2000 + CT.Year;
			}
			OFH.endTime = Trans::JLD2GPST(Trans::Cal2JLD(CT));
		}
		else if (LineBuffer.find("LEAP SECONDS", 12) != string::npos)
		{
			ss >> OFH.LeapSec;
		}
	} while (LineBuffer.find("END OF HEADER", 13) == string::npos);
	//cout << LineBuffer << endl;
	//cout << OFH.HeadLineNumber << endl;
	/************ͷ�ļ���ȡ����*************/

	/*************�����ļ���ȡ**************/

	ObsFileRecord OFR0;//��Ϊÿ��push��vector�Ŀն���
	for (i = 0; !infile.eof(); i++)
	{
		//if (i==5)//�������
		//{
		//	cout<<"stop"<<endl;
		//}

		getline(infile, LineBuffer);
		if (LineBuffer == "")//�����һ�еĴ���
		{
			cout << " O �ļ���ȡ��ϣ�" << endl;
			return 1;
		}
		if (atoi(LineBuffer.substr(28, 1).c_str()) == 4)
		{
			getline(infile, LineBuffer);
			while (LineBuffer.find("COMMENT", 7) != string::npos)
			{
				getline(infile, LineBuffer);
			}
		}

		OFR.push_back(OFR0);

		ss.clear();
		ss.str("");
		ss << LineBuffer;

		ss >> OFR[i].OFRH.CT.Year;//��ȡ��ʱ��
		ss >> OFR[i].OFRH.CT.Month;
		ss >> OFR[i].OFRH.CT.Day;
		ss >> OFR[i].OFRH.CT.Hour;
		ss >> OFR[i].OFRH.CT.Minute;
		ss >> OFR[i].OFRH.CT.Second;
		if (OFR[i].OFRH.CT.Year >= 80 && OFR[i].OFRH.CT.Year <= 99)
		{
			OFR[i].OFRH.CT.Year = 1900 + OFR[i].OFRH.CT.Year;
		}
		else if (OFR[i].OFRH.CT.Year<80 && OFR[i].OFRH.CT.Year >= 0)
		{
			OFR[i].OFRH.CT.Year = 2000 + OFR[i].OFRH.CT.Year;
		}
		//ת��ΪGPSʱ
		OFR[i].OFRH.GT = Trans::JLD2GPST(Trans::Cal2JLD(OFR[i].OFRH.CT));

		//��¼������������PRN�ţ�һ�������12���������ͣ�������������������12��������
		ss >> OFR[i].OFRH.epoch_Mark;
		ss >> OFR[i].OFRH.SatNum;
		OFR[i].Obs_Data.resize(OFR[i].OFRH.SatNum);
		int a = int(OFR[i].OFRH.SatNum / 12);
		int b = int(OFR[i].OFRH.SatNum % 12);
		if (a == 0 || (a == 1 && b == 0))//һ��
		{
			for (j = 0; j<OFR[i].OFRH.SatNum; j++)
			{
				OFR[i].Obs_Data[j].PRN = LineBuffer.substr(32 + 3 * j, 3);
			}
		}
		if ((a == 1 && b != 0) || (a == 2 && b == 0))//����
		{
			for (j = 0; j<12; j++)
			{
				OFR[i].Obs_Data[j].PRN = LineBuffer.substr(32 + 3 * j, 3);
			}
			getline(infile, LineBuffer);
			for (j = 12; j<OFR[i].OFRH.SatNum; j++)
			{
				OFR[i].Obs_Data[j].PRN = LineBuffer.substr(32 + 3 * (j - 12), 3);
			}
		}
		if ((a == 2 && b != 0) || (a == 3 && b == 0))//����
		{
			for (j = 0; j<12; j++)
			{
				OFR[i].Obs_Data[j].PRN = LineBuffer.substr(32 + 3 * j, 3);
			}
			getline(infile, LineBuffer);
			for (j = 12; j<24; j++)
			{
				OFR[i].Obs_Data[j].PRN = LineBuffer.substr(32 + 3 * (j - 12), 3);
			}
			getline(infile, LineBuffer);
			for (j = 24; j<OFR[i].OFRH.SatNum; j++)
			{
				OFR[i].Obs_Data[j].PRN = LineBuffer.substr(32 + 3 * (j - 24), 3);
			}
		}

		int A_line = int(OFH.ObsTypeNumber / 5);//ÿ�����ǵĹ۲�������ռ������
		int B_line = int(OFH.ObsTypeNumber % 5);
		for (j = 0; j < OFR[i].OFRH.SatNum; j++)//һ��ʱ�̹۲����ݵ�������
		{
			OFR[i].Obs_Data[j].SingleData.resize(OFH.ObsTypeNumber);

			for (int k = 0; k < A_line; k++)
			{
				getline(infile, LineBuffer);
				while ((int)LineBuffer.length() < 80)
				{
					LineBuffer = LineBuffer + " ";
				}
				for (int t = 0; t < 5; t++)
				{
					OFR[i].Obs_Data[j].SingleData[k * 5 + t] = strtod(LineBuffer.substr(t * 16, 14).c_str(), NULL);
				}
			}
			if (B_line != 0)
			{
				getline(infile, LineBuffer);
				while ((int)LineBuffer.length() < B_line * 16)
				{
					LineBuffer = LineBuffer + " ";
				}
				for (int k = 0; k < B_line; k++)
				{
					OFR[i].Obs_Data[j].SingleData[A_line * 5 + k] = strtod(LineBuffer.substr(k * 16, 14).c_str(), NULL);
				}

			}
		}
	}

	infile.close();
	return 0;
}

int ObsFile::ReadFile1(const string& filename)
{
	ifstream infile(filename);
	if(!infile.is_open())
	{
		cout<<"��ȡʧ��"<<endl;
		return 0;
	}

	int i,j,k;
	string LineBuffer;
	stringstream ss;
	OFH.HeadLineNumber=0;
	/*********��ȡͷ�ļ�**********/
	do 
	{
		getline(infile,LineBuffer);
		OFH.HeadLineNumber++;
		ss.clear();
		ss.str("");
		ss<<LineBuffer;

		if (LineBuffer.find("RINEX VERSION / TYPE",20)!=string::npos)
		{
			ss>>OFH.version;
			//cout<<OFH.version;
		}
		if (LineBuffer.find("MARKER NAME",11)!=string::npos)
		{
			ss>>OFH.mark_name;
		}
		else if (LineBuffer.find("MARKER NUMBER",13)!=string::npos)
		{
			ss>>OFH.mark_Number;
		}
		else if (LineBuffer.find("REC # / TYPE / VERS",19)!=string::npos)
		{
			ss>>OFH.receiverNumber;
			ss>>OFH.receiveType;
		}
		else if (LineBuffer.find("ANT # / TYPE",12)!=string::npos)
		{
			ss>>OFH.antennaNumber;
			ss>>OFH.antennaType;
		}
		else if (LineBuffer.find("APPROX POSITION XYZ",19)!=string::npos)
		{
			ss>>OFH.approxPos.X;
			ss>>OFH.approxPos.Y;
			ss>>OFH.approxPos.Z;
		}
		else if (LineBuffer.find("ANTENNA: DELTA H/E/N",20)!=string::npos)
		{
			ss>>OFH.antennaDelta.U;
			ss>>OFH.antennaDelta.E;
			ss>>OFH.antennaDelta.N;
		}
		else if (LineBuffer.find("# / TYPES OF OBSERV",19)!=string::npos)
		{
			ss>>OFH.ObsTypeNumber;
			OFH.ObsType.resize(OFH.ObsTypeNumber);
			int a=int(OFH.ObsTypeNumber/9);
			if (a==0)//С��9�����ͣ����û���
			{
				for(i=0;i<OFH.ObsTypeNumber;i++)
				{
					OFH.ObsType[i]=LineBuffer.substr(10+6*i,2);
				}
			}
			if (a==1)//���У����ڵ���9�����ͣ�ĩβ���������ͬ
			{
				for(i=0;i<9;i++)
				{
					OFH.ObsType[i]=LineBuffer.substr(10+6*i,2);
				}
				getline(infile,LineBuffer);
				OFH.HeadLineNumber++;
				for(i=9;i<OFH.ObsTypeNumber;i++)
				{
					OFH.ObsType[i]=LineBuffer.substr(10+6*(i-9),2);
				}
			}
			if (a==2)//����
			{
				for(i=0;i<9;i++)
				{
					OFH.ObsType[i]=LineBuffer.substr(10+6*i,2);
				}
				getline(infile,LineBuffer);
				OFH.HeadLineNumber++;
				for(i=9;i<18;i++)
				{
					OFH.ObsType[i]=LineBuffer.substr(10+6*(i-9),2);
				}
				getline(infile,LineBuffer);
				OFH.HeadLineNumber++;
				for(i=18;i<OFH.ObsTypeNumber;i++)
				{
					OFH.ObsType[i]=LineBuffer.substr(10+6*(i-18),2);
				}
			}
		}
		else if (LineBuffer.find("INTERVAL",8)!=string::npos)
		{
			ss>>OFH.interval;
		}
		else if (LineBuffer.find("TIME OF FIRST OBS",17)!=string::npos)
		{
			CalenderTime CT;
			ss>>CT.Year;
			ss>>CT.Month;
			ss>>CT.Day;
			ss>>CT.Hour;
			ss>>CT.Minute;
			ss>>CT.Second;
			if (CT.Year>=80&&CT.Year<=99)
			{
				CT.Year=1900+CT.Year;
			}
			else if (CT.Year<80&&CT.Year>=0)
			{
				CT.Year=2000+CT.Year;
			}
			OFH.startTime=Trans::JLD2GPST(Trans::Cal2JLD(CT));
		}
		else if (LineBuffer.find("TIME OF LAST OBS",16)!=string::npos)
		{
			CalenderTime CT;
			ss>>CT.Year;
			ss>>CT.Month;
			ss>>CT.Day;
			ss>>CT.Hour;
			ss>>CT.Minute;
			ss>>CT.Second;
			if (CT.Year>=80&&CT.Year<=99)
			{
				CT.Year=1900+CT.Year;
			}
			else if (CT.Year<80&&CT.Year>=0)
			{
				CT.Year=2000+CT.Year;
			}
			OFH.endTime=Trans::JLD2GPST(Trans::Cal2JLD(CT));
		}
		else if (LineBuffer.find("LEAP SECONDS",12)!=string::npos)
		{
			ss>>OFH.LeapSec;
		}
	} while (LineBuffer.find("END OF HEADER",13)==string::npos);
	cout<<LineBuffer<<endl;
	cout<<OFH.HeadLineNumber<<endl;
	/************ͷ�ļ���ȡ����*************/

	/*************�����ļ���ȡ**************/

	ObsFileRecord OFR0;//��Ϊÿ��push��vector�Ŀն���
	for(i=0;!infile.eof();i++)
	{
		//if (i==5)//�������
		//{
		//	cout<<"stop"<<endl;
		//}
		
		getline(infile,LineBuffer);
		if (LineBuffer=="")//�����һ�еĴ���
		{
			cout<<"oookkk"<<endl;
			return 0;
		}
		if (atoi(LineBuffer.substr(28,1).c_str())==4)
		{
			getline(infile,LineBuffer);
			while(LineBuffer.find("COMMENT",7)!=string::npos)
			{
				getline(infile,LineBuffer);
			}
		}

		OFR.push_back(OFR0);

		ss.clear();
		ss.str("");
		ss<<LineBuffer;

		ss>>OFR[i].OFRH.CT.Year;//��ȡ��ʱ��
		ss>>OFR[i].OFRH.CT.Month;
		ss>>OFR[i].OFRH.CT.Day;
		ss>>OFR[i].OFRH.CT.Hour;
		ss>>OFR[i].OFRH.CT.Minute;
		ss>>OFR[i].OFRH.CT.Second;
		if (OFR[i].OFRH.CT.Year>=80&&OFR[i].OFRH.CT.Year<=99)
		{
			OFR[i].OFRH.CT.Year=1900+OFR[i].OFRH.CT.Year;
		}
		else if (OFR[i].OFRH.CT.Year<80&&OFR[i].OFRH.CT.Year>=0)
		{
			OFR[i].OFRH.CT.Year=2000+OFR[i].OFRH.CT.Year;
		}
		//ת��ΪGPSʱ
		OFR[i].OFRH.GT=Trans::JLD2GPST(Trans::Cal2JLD(OFR[i].OFRH.CT));

		//��¼������������PRN�ţ�һ�������12���������ͣ�������������������12��������
		ss>>OFR[i].OFRH.epoch_Mark;
		ss>>OFR[i].OFRH.SatNum;
		OFR[i].Obs_Data.resize(OFR[i].OFRH.SatNum);
		int a=int(OFR[i].OFRH.SatNum/12);
		int b=int(OFR[i].OFRH.SatNum%12);
		if (a==0||(a==1&&b==0))//һ��
		{
			for(j=0;j<OFR[i].OFRH.SatNum;j++)
			{
				OFR[i].Obs_Data[j].PRN=LineBuffer.substr(32+3*j,3);
			}
		}
		if ((a==1&&b!=0)||(a==2&&b==0))//����
		{
			for(j=0;j<12;j++)
			{
				OFR[i].Obs_Data[j].PRN=LineBuffer.substr(32+3*j,3);
			}
			getline(infile,LineBuffer);
			for(j=12;j<OFR[i].OFRH.SatNum;j++)
			{
				OFR[i].Obs_Data[j].PRN=LineBuffer.substr(32+3*(j-12),3);
			}
		}
		if ((a==2&&b!=0)||(a==3&&b==0))//����
		{
			for(j=0;j<12;j++)
			{
				OFR[i].Obs_Data[j].PRN=LineBuffer.substr(32+3*j,3);
			}
			getline(infile,LineBuffer);
			for(j=12;j<24;j++)
			{
				OFR[i].Obs_Data[j].PRN=LineBuffer.substr(32+3*(j-12),3);
			}
			getline(infile,LineBuffer);
			for(j=24;j<OFR[i].OFRH.SatNum;j++)
			{
				OFR[i].Obs_Data[j].PRN=LineBuffer.substr(32+3*(j-24),3);
			}
		}

		int A_line=int(OFH.ObsTypeNumber/5);//ÿ�����ǵĹ۲�������ռ������
		int B_line=int(OFH.ObsTypeNumber%5);
		for (j=0;j<OFR[i].OFRH.SatNum;j++)//һ��ʱ�̹۲����ݵ�������
		{
			OFR[i].Obs_Data[j].SingleData.resize(OFH.ObsTypeNumber);
			getline(infile,LineBuffer);
			if (A_line==0||(A_line==1&&B_line==0))//ռһ��
			{
				for (k=0;k<OFH.ObsTypeNumber;k++)
				{
					OFR[i].Obs_Data[j].SingleData[k]=strtod(LineBuffer.substr(16*k,14).c_str(),NULL);
				}
			}
			if ((A_line==1&&B_line!=0)||(A_line==2&&B_line==0))//ռ����
			{
				if (LineBuffer.length()<78)
				{
					OFR[i].Obs_Data[j].SingleData[4]=0.0;
					for(int t=4;t>0;t--)
					{
						if (LineBuffer.length()<14+16*(t-1))
						{
							OFR[i].Obs_Data[j].SingleData[t-1]=0.0;
						}
						else
						{
							OFR[i].Obs_Data[j].SingleData[t-1]=strtod(LineBuffer.substr(16*(t-1),14).c_str(),NULL);
						}
					}
				}
				else
				{
					for (k=0;k<5;k++)
					{
						OFR[i].Obs_Data[j].SingleData[k]=strtod(LineBuffer.substr(16*k,14).c_str(),NULL);
					}
				}
				getline(infile,LineBuffer);
				if (LineBuffer.length()<14+16*(B_line-1))
				{
					for (int t=1,k=B_line;k>0;k--,t++)
					{
						if (LineBuffer.length()<14+16*(k-1))
						{
							OFR[i].Obs_Data[j].SingleData[OFH.ObsTypeNumber-t]=0.0;
						}
						else
						{
							OFR[i].Obs_Data[j].SingleData[OFH.ObsTypeNumber-t]=strtod(LineBuffer.substr(16*(k-1),14).c_str(),NULL);
						}
					}
				}
				else
				{
					for (k=5;k<OFH.ObsTypeNumber;k++)
					{
						OFR[i].Obs_Data[j].SingleData[k]=strtod(LineBuffer.substr(16*(k-5),14).c_str(),NULL);
					}
				}
			}
			if ((A_line==2&&B_line!=0)||(A_line==3&&B_line==0))//����
			{
				if (LineBuffer.length()<78)
				{
					OFR[i].Obs_Data[j].SingleData[4]=0.0;
					for(int t=4;t>0;t--)
					{
						if (LineBuffer.length()<14+16*(t-1))
						{
							OFR[i].Obs_Data[j].SingleData[t-1]=0.0;
						}
						else
						{
							OFR[i].Obs_Data[j].SingleData[t-1]=strtod(LineBuffer.substr(16*(t-1),14).c_str(),NULL);
						}
					}
				}
				else
				{
					for (k=0;k<5;k++)
					{
						OFR[i].Obs_Data[j].SingleData[k]=strtod(LineBuffer.substr(16*k,14).c_str(),NULL);
					}
				}
				getline(infile,LineBuffer);
				if (LineBuffer.length()<78)
				{
					OFR[i].Obs_Data[j].SingleData[9]=0.0;
					for(int t=8;t>4;t--)
					{
						if (LineBuffer.length()<14+16*(t-5))
						{
							OFR[i].Obs_Data[j].SingleData[t]=0.0;
						}
						else
						{
							OFR[i].Obs_Data[j].SingleData[t-5]=strtod(LineBuffer.substr(16*(t-5),14).c_str(),NULL);
						}
					}
				}
				else
				{
					for (k=5;k<10;k++)
					{
						OFR[i].Obs_Data[j].SingleData[k]=strtod(LineBuffer.substr(16*(k-5),14).c_str(),NULL);
					}
				}
				getline(infile,LineBuffer);
				if (LineBuffer.length()<14+16*(B_line-1))
				{
					for (int t=1,k=B_line;k>0;k--,t++)
					{
						if (LineBuffer.length()<14+16*(k-1))
						{
							OFR[i].Obs_Data[j].SingleData[OFH.ObsTypeNumber-t]=0.0;
						}
						else
						{
							OFR[i].Obs_Data[j].SingleData[OFH.ObsTypeNumber-t]=strtod(LineBuffer.substr(16*(k-1),14).c_str(),NULL);
						}
					}
				}
				else
				{
					for (k=10;k<OFH.ObsTypeNumber;k++)
					{
						OFR[i].Obs_Data[j].SingleData[k]=strtod(LineBuffer.substr(16*(k-10),14).c_str(),NULL);
					}
				}
			}
		}		
	}


	infile.close();
	return 0;
}
