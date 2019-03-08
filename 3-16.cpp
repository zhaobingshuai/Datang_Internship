#include<stdio.h>
#include<iostream>
#include<vector>
#include<map>
#include<iterator>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<cstring>
#include <regex>    //������ʽ

using namespace std;

//ͨ�����÷�ʽ��ʾstrVec�����е���������
void DisplayVector(vector<vector<string>>&strVec); 
bool IsANumberString(string str,bool &bInteger, bool &bFloat);
//ͨ�����÷�ʽ������strVec,����Ч���ݵ�key��vector(value��ԭ����ע��)���뵽mKeyValue��
void AnalysisVector(vector<vector<string>>&strVec,map<int,string>&mLineNoKey,map<string,vector<string>>&mKeyValue);
//����source.txt����Ч���ݵ�˳�򱣴�key��value��NewData.txt��
void SaveToFile(map<int,string>&mLineNoKey,map<string,vector<string>>&mKeyValue);  
//��ѯ�Ӻ���,����key�������Ӧ�����е���Ϣ��key��value��ԭ����ע��
void FindKey(char strFind[1000],map<string,vector<string>>&mKeyValue);

int main()         //main����                             
{ 
	vector<vector<string>>strVec;    //�����ά����strVec�����ԭ����--SplitString-->�ָ�������
	map<int,string>mLineNoKey;      //����һ��map: <�к�,mKeyValue.key>
	map<string,vector<string>>mKeyValue; //AnalysisVector֮������mKeyValue�����д��key��vector(value��ԭ����ע��)

	FILE*fp;                
	if((fp=fopen("source.txt","r+")) == NULL)  //��source.txt�ļ�     
	{
		printf("�޷��򿪻��ҵ��ļ�!\n");
		exit(0);
	}

	char strOrigin[1000];      //strOrigin��Ž�ȡǰ���ַ���
	while(!feof(fp))           //feof����ļ��Ƿ����       
	{
		fgets(strOrigin, 1000, fp);  //��source.txt�ж�ȡ���ݵ�strOrigin������

		/*******************��������������ַ������ȵ�����******************/
		int len=strlen(strOrigin);
		if(len>=1) 
		{
			if((strOrigin[len-1]==0x0a) || (strOrigin[len-1]==0x0d))
			{
				strOrigin[len-1]=0;
			}
		}
		if(len>=2) 
		{
			if((strOrigin[len-2]==0x0a) || (strOrigin[len-2]==0x0d))
			{
				strOrigin[len-2]=0;
			}
		}

		vector<string>vline;   //����һάstring������vline,���ÿһ���ַ����ָ������н��
		printf("ԭ���ݣ� %s\n",strOrigin);   //��ʾ�ָ�ǰ��ԭ����
		const char *delim =" \t";  //delimΪ��ȡ��־�����������������־������tab	
		char *pCut = strtok(strOrigin, delim);//��pCutָ��strOrigin�еĵ�һ����ȡ��־ǰ���ַ���
		while(pCut)   //ͨ��ѭ���ָ�ԭ����
		{
			//printf("%s\t",pCut);//��ʾÿ�ηָ��Ľ����������
			string strAfter;
			strAfter = pCut;      //�ָ����ַ�������strAfter
			pCut = strtok(NULL, delim);	//��pCutָ��strOrigin�е���һ����ȡ��־ǰ���ַ���
			vline.push_back(strAfter);  //��strAfter����vline��������һά��pCutָ����һ����ȡ��־ǰ���ַ�������û���򷵻�NULL����ѭ��
		}
		strVec.push_back(vline);   //��һά����vline�����ά����strVec
	}
	fclose(fp);

	DisplayVector(strVec);    //ͨ�����÷�ʽ��ʾstrVec�����е���������
	AnalysisVector(strVec,mLineNoKey,mKeyValue);//����strVec,����Ч���ݵ�key��vector(value��ԭ����ע��)���뵽mKeyValue��
	SaveToFile(mLineNoKey,mKeyValue); //����source.txt����Ч���ݵ�˳�򱣴�key��value��NewData.txt��

	printf("*****************************************************************\n");
	printf("��������Ҫ���ҵ�key:\n");
	char strFindKey[1000];  // find_key���鱣��Ҫ���ҵ�keyֵ
	scanf("%s", strFindKey);
	FindKey(strFindKey,mKeyValue);  //���ò�ѯ����������key��������е���Ϣ��key,value��ԭ����ע��

	system("pause");
	return 0;
}	

void DisplayVector(vector<vector<string>>&strVec)  
{
	printf("****************************************************************\n");
	for(unsigned int iDVa = 0; iDVa < strVec.size(); iDVa++)  //��ʾstrVec�����е����н����������
	{
		if(strVec[iDVa].size() > 2)   //���˵���Ч����
		{
			printf("ԭ���ݷָ�����vector�ɹ���");
			for(unsigned int iDVb = 0; iDVb < strVec[iDVa].size(); iDVb++)
			{
				string strvecValue = strVec[iDVa][iDVb];
				printf("%s\t",strvecValue.c_str());
			}
			printf("\n"); 
		}
	}
}


//�������ܣ�����strVec,����Ч���ݵ�key��vector(value��ԭ����ע��)���뵽mKeyValue��
//���룺��άvector��strVec
//�������άmap:mKeyValue
//strVector[1]��strVector[2]��Ϊvalue������strVector[1]Ϊ���յ�value��strVector[3]���ԭ��ֵ��
//strVector[4]��strVector[5]�����ע�ͣ�����strVector[4]Ϊ���յ�ע�ʹ�
void AnalysisVector(vector<vector<string>>&strVec,map<int,string>&mLineNoKey,map<string,vector<string>>&mKeyValue)
{
	bool bInteger;
	bool bFloat;
	vector<vector<string>>vNewVOC;//vNewVOC��������µ�Value��Orignname��Comments,����ѭ��������նˣ�������
	vector<string>vNewLine;   //vNewLine���ÿ�����ݵ�value��ԭ��ֵ��ע�͵�
	int iOriginNo = 1;   //����iOriginNoΪ�к�
	string strKey;  //���key
	string strVector[10];   //���value��ԭ��ֵ��ע�͵�

	for(unsigned int iVec= 0; iVec < strVec.size(); iVec++)  //��������
	{
		if(strVec[iVec].size() > 2)   //1-����������������������ַ���
		{
			vector<string>::iterator vit = strVec[iVec].begin();
			strKey = *vit;   //����key   
			vit++;  //�������ں�
			vit++;  //��һ��value
			IsANumberString(*vit,bInteger,bFloat);//���ú����ж��Ƿ�������
			if(bInteger || bFloat)  //2-��������������֣������ӵ�value�������һ��
			{
				strVector[1] = *vit;  //�����һ��value 
				vit++;   //ָ����һ���ַ�������
				IsANumberString(*vit,bInteger,bFloat);//���ú����ж���һ���Ƿ�������
				while((bInteger || bFloat ) && (vit != strVec[iVec].end())) //�����һ���������֣�ֱ������,���һ��������    
				{
					strVector[2] = *vit;   //������һ��value
					strVector[1] = strVector[1] + "    " + strVector[2];  //�ӵ���һ��value���棬����value֮����Ͽո�  
					vit++;  
					if(vit != strVec[iVec].end())  //2-1�����û�е����������ж�
					{
						IsANumberString(*vit,bInteger,bFloat);//���ú����ж��Ƿ�������
					}
					else  //2-1����Ѿ�ȡ������value������û�������ˣ�eg:num1 = 12.3 +34.5
					{
					    strVector[3] = "";
						strVector[4] = "";
						continue;
					}
				}	

				if(vit != strVec[iVec].end())  //3-���value֮�����ַ�����������ԭ����־����ע��
				{
					if((*vit == "ԭ��") ||(*vit == "origin")) //4-�����ԭ����־
					{
						vit++;   //����ԭ����־
						if (vit != strVec[iVec].end()) //5-�о����ԭ��ֵ
						{
							strVector[3] = *vit;   //��������ԭ��ֵ 
							vit++;   
							if(vit != strVec[iVec].end())    //6-���ԭ��֮����ע��
							{
								//����ע�����ж��ٿո�ͳһ��Ϊע�ͣ�ֱ������
								strVector[4] = *vit;  //�����һ��ע�ʹ�  
								vit++;    
								while(vit != strVec[iVec].end())    
								{
									strVector[5] = *vit;   //������һ��ע�ʹ�  
									strVector[4] = strVector[4] + "  " + strVector[5];//�ӵ���һ���ַ�������,���ע��֮����Ͽո�  
									vit++; 
								}				
							}
							else     //6-ԭ��֮��û��ע��
							{
								strVector[4] = "";  //ע�͵Ŀ�λ
							}
						}
						else   //5-��ԭ����־������û�о����ԭ��ֵ��Ҳû��ע��
						{
							 strVector[3] = ""; //ԭ���Ŀ�λ
							 strVector[4] = ""; //ע�͵Ŀ�λ
						}
					}
					else    //4-û��ԭ����־��ȫ����ע��
					{ 
						strVector[3] = ""; //ԭ���Ŀ�λ
						strVector[4] = *vit;   
					}
				}
				else  //3-value֮��û��������,�˴�������������߶���
				{
					strVector[3] = ""; //ԭ���Ŀ�λ
					strVector[4] = ""; //ע�͵Ŀ�λ
				}
			}
			else//2-���ں�֮���������ˣ�����û��value���ǹ�3���ַ���
			{
				continue;
			}
		}
		else   //1-������ݲ�����������3���ַ�����
		{
			continue;
		}
		vNewLine.push_back(strVector[1]);  //������value����vNewLine
		vNewLine.push_back(strVector[3]);  //�������ԭ��ֵ����vNewLine 
		vNewLine.push_back(strVector[4]);  //������ע������vNewLine

		mLineNoKey[iOriginNo] = strKey;//ԭʼ������,���кź�key����mLineNoKey
		iOriginNo++;  
		mKeyValue.insert(make_pair(strKey,vNewLine)); //��key��vNewLine����map��
		vNewVOC.push_back(vNewLine);  //��ÿ��vNewLine����vNewVOC  
		vNewLine.clear();  //���vNewLine����������һ�����ݽ���
	}  //��������


	printf("*****************************************************************\n");
	for(unsigned int iDVaa = 0; iDVaa < vNewVOC.size(); iDVaa++)//����������mKeyValue��vector�е���������������ã���ȥ��
	{	
		printf("�Ѳ�����vector(value+ԭ��+ע��)��");
		for(unsigned int iDVbb = 0; iDVbb < vNewVOC[iDVaa].size(); iDVbb++)
		{
			string strVoc = vNewVOC[iDVaa][iDVbb];
			printf("%s\t",strVoc.c_str());
		}
		printf("\n"); 
	}

	map<string,vector<string>>::iterator mIter;   //mIterָ��mKeyValue
	printf("*****************************************************************\n");
	for(mIter = mKeyValue.begin(); mIter!=mKeyValue.end(); mIter++) //�������mKeyValue�����е�key��value�������ã���ȥ��
	{
		string strFirst = (*mIter).first.c_str();
		string strSecond = (*mIter).second[0];
		printf("����mKeyValue�ɹ�: %s = %s\n",strFirst.c_str(),strSecond.c_str());
	}
}

//����˵����
//���������str������֤���ַ���
//�������:bInteger���Ƿ���һ������
//�������:bFloat���Ƿ���һ��������
//���أ���һ����������������true
bool IsANumberString(string str,bool &bInteger, bool &bFloat)
{
	const string rsn="^[+-]?([1-9]\\d*|0)$";
	const string rsf="^[+-]?([1-9]\\d*\\.\\d*|0.\\d*[1-9]\\d*|0?.0+|0)$";
	regex rn(rsn);
	regex rf(rsf);
	bInteger = regex_match(str,rn);
	bFloat = regex_match(str,rf);
	return bInteger || bFloat;
}

//�������ܣ�����source.txt����Ч���ݵ�˳�򱣴�key��value��NewData.txt��
void SaveToFile(map<int,string>&mLineNoKey,map<string,vector<string>>&mKeyValue)  
{
	FILE*fp;
	if((fp = fopen("NewData.txt","w")) == NULL)  //��NewData.txt�ļ�
	{
		printf("�޷��򿪻��ҵ��ļ�!\n");
		exit(0);
	}

	map<string,vector<string>>::iterator mIter;   //��mIterָ��mKeyValue
	for(unsigned int i = 1; i <=  mLineNoKey.size(); i++)
	{
		string skey =  mLineNoKey[i];
		mIter = mKeyValue.find(skey);  //����mLineNoKey���к�i��Ӧ��key
		if(mIter != mKeyValue.end())  //����ҵ���
		{
			string svalue = (*mIter).second[0];//��key��Ӧ��value��ֵ��svalue
			fprintf(fp,"%s = %s\n",skey.c_str(),svalue.c_str());//����source.txt����Ч���ݵ�˳�����key = value���ļ���
		}
	}
	fclose(fp);
} 

//��ѯ�Ӻ���,����key����mKeyValue�в��Ҷ�Ӧ��key���ҵ���������е���Ϣ��key��value��ԭ����ע�ͣ�
//�Ҳ�������ʾ�û���������Ҫ���ҵ�key�������Ƿ��ҵ�������0����ʱ�˳���ѯ
//���룺strFindΪҪ��ѯ��key��ͨ�����ô���mKeyValue
void FindKey(char strFind[1000], map<string,vector<string>>&mKeyValue) 
{
	map<string,vector<string>>::iterator mIter;   //mIterָ��mKeyValue
	int  nFlag = 0;         //nFlagΪ������ѯ��־
	while(!nFlag)        //��������key��ֱ���ҵ���Ӧ��keyΪֹ��
	{
		string strFK(strFind);
		mIter = mKeyValue.find(strFK);   //mIterָ���ҵ������ݵ�key
		if(mIter == mKeyValue.end())   //����Ϊ���ʾ����������û�ҵ�
		{
			printf("*****û�ҵ�,����������Ҫ��ѯ��key������˳���ѯ�����롮0��!******\n");
			scanf("%s", strFind);  //������һ�β�ѯ��key
			string strFK(strFind);

			string string0 = "0";
			if(strFK != string0)  //û�ҵ����ǲ������ˣ�����0�ɽ�����ѯ������0�������ѯ��
			{
				mIter = mKeyValue.find(strFK);
				continue;  //�˳�����ѭ����������һ�β���
			}
			else
			{
				printf("**********************   ���β�ѯ������  ************************\n");
				nFlag = 1; //��������
			}
		}
		else   //�ҵ������
		{
			printf("**********************  ��ϲ�����ҵ���!  ***********************\n");
			string second0 = (*mIter).second[0];     
			string second1 = (*mIter).second[1];     
			string second2 = (*mIter).second[2];  
			printf("*   key:       %s\n",(*mIter).first.c_str()); //����û����ҵ����ݵ�key
			printf("*   value��    %s\n",second0.c_str());     //����û����ҵ����ݵ�key��Ӧ��value
			printf("*   ԭ����     %s\n",second1.c_str());     //����û����ҵ����ݵ�key��Ӧ��ԭ��
			printf("*   ע�ͣ�     %s\n",second2.c_str());     //����û����ҵ����ݵ�key��Ӧ��ע��

			printf("******�����Ҫ������ѯ�������롮1',����˳���ѯ�����롮0����****\n");
			int nOutOrGoon;     //nOutOrGoonΪ�˳����߼����ı�־
			scanf("%d", &nOutOrGoon);
			if(nOutOrGoon == 1)
			{   
				printf("********������������Ҫ��ѯ��keyֵ������˳������롮0����********\n"); 
				scanf("%s", strFind);  //������һ�β�ѯ��key
				string strFK(strFind);
				string string0 = "0";
				if(strFK != string0)  //����������ѯ�ֺ���ˣ�����0�ɽ�����ѯ������0�������ѯ��
				{
					mIter = mKeyValue.find(strFK);
					continue;  //�˳�����ѭ����������һ�β���
				}
				else
				{
					printf("**********************   ���β�ѯ������  ************************\n");
					nFlag = 1; //��������
				}
			}
			else if(nOutOrGoon == 0)
			{
				printf("**********************   ���β�ѯ������  ************************\n");
				nFlag = 1; //��������
			}	
			else
			{
				printf("********************   �������ָ������  **********************\n");
				printf("**********************   ���β�ѯ������  ************************\n");
				nFlag = 1; //��������
			}
		}
	}
}