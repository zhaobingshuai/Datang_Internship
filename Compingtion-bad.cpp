#include<windows.h>
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<string>
#include<cstring>
#include<vector>
#include<map>
#include<iterator>
#include<stdlib.h>
#include<algorithm>
#include<regex>

using namespace std;

#define INPUT_STR_LEN 1000

/****************************************************************************************************************
*�������ܣ�N��keyvalue���ȡһ��value�飬������е���ϣ�����ŵ�tmp_result
* ԭ������ջ��ԭ����ÿ��vec�ĵ�һ��value��ѹ��tmp�������浽tmp_result
*����ĩβvalue�飬��ѹ�����һ�����ݵĵڶ���value�飬����󵯳�����ѹ�����һ�����ݵĵ�����value�飬����󵯳�������ѭ��
**************************************************************************************************************/
void get_result_in_vector(vector<vector<vector<string> > >&vec,unsigned int N,vector<vector<string> >&tmp,vector<vector<vector<string> > >&tmp_result)
{
		for(unsigned int nResultColumn = 1; nResultColumn <(vec[N].size()-1); ++nResultColumn)
		{
				tmp.push_back(vec[N][nResultColumn]);//ѹ���N��value�ĵ�һ��value��
				if (N < (vec.size()-1))
				{
						get_result_in_vector(vec, N+1, tmp, tmp_result); //�ݹ飬ѹ���N��value����һ��value��
				}
				else
				{
						vector<vector<string> >one_result;
						for(unsigned int nTmpColumn = 0; nTmpColumn < tmp.size(); ++nTmpColumn)
						{
								one_result.push_back(tmp.at(nTmpColumn));//���һ�����
						}
						tmp_result.push_back(one_result);//����������
				}
				tmp.pop_back();//����ĩβvalue��
		}
		N--;
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

int main0()
{
		bool bInteger;  //��֤�Ƿ���һ������
		bool bFloat;    //��֤�Ƿ���һ��������
		vector<vector<vector<string> > >vAllKeyValueComment; //����3άvector����Ŷ�ȡ��������key��value��ע��
		vector<vector<vector<string> > >vPartKeyValueComment; //����3άvector��������б��޸ĵ�key��value��ע�ͣ������ļ�������
		vector<vector<string> >vOneKVC; //����2άvector�����ÿһ�ζ�ȡ��vOneKey��vOneValueUnit��vOneCommentUnit
		vector<string>vOneKey; //����һάvector�����ÿһ���key
		vector<string>vOneValueUnit; //����һάvector�����ÿһ���value
		vector<string>vOneCommentUnit; //����һάvector�����ÿһ���ע��

		FILE*fp;                
		if((fp=fopen("StaticSimuConfiguration.txt","r+")) == NULL)  //���ļ� StaticSimuConfiguration.txt
		{
				printf("�޷��򿪻��ҵ��ļ�!\n");
				exit(0);
		}
		printf("��StaticSimuConfiguration.txt�ļ��ɹ�!\n");

		char chReadStringFromFile[INPUT_STR_LEN] = {0};  //chReadStringFromFile��Ŵ��ļ��ж�ȡ���ַ���
		int nDisplayLineNo = 1;            //nDisplayLineNoΪ��ʾ��ȡ���ַ������к�
		int dddd=1;
		while(!feof(fp))           //feof����ļ��Ƿ����       
		{
				chReadStringFromFile[0]=0;
				if(NULL == fgets(chReadStringFromFile, INPUT_STR_LEN, fp)) {
						continue;  
				}
				if(feof(fp)){
						continue;
				}
				if(chReadStringFromFile[0]!=10)
				{
						/*******************��������������ַ������ȵ�����******************/
						int len=strlen(chReadStringFromFile);
						if(len==0) {continue;}
						if(len>=1) {if((chReadStringFromFile[len-1]==0x0a) || (chReadStringFromFile[len-1]==0x0d)){chReadStringFromFile[len-1]=0;}}
						if(len>=2) {if((chReadStringFromFile[len-2]==0x0a) || (chReadStringFromFile[len-2]==0x0d)){chReadStringFromFile[len-2]=0;}}
						if(len==0) {continue;}
						/*******************��������������ַ������ȵ�����*******************/

						printf("%d�� %s\n",nDisplayLineNo,chReadStringFromFile);   //��ʾ��<�кţ��ָ�ǰԭ����>
						nDisplayLineNo++;  //��ʾ�ַ������к�����

						const char *delim = " =\t";  //delimΪ��ȡ��־	
						char *pCut = strtok(chReadStringFromFile, delim);//��pCutָ��chReadStringFromFile�е�һ����ȡ��־ǰ���ַ���
						//����ÿһ�������е�key��vOneKey�У�ͬʱ���浽vOneKVC
						string strKey;
						strKey = pCut;     
						vOneKey.push_back(strKey);
						vOneKVC.push_back(vOneKey);  
						vOneKey.clear();

						pCut = strtok(NULL, delim);//��pCutָ��chReadStringFromFile�е���һ����ȡ��־ǰ���ַ���
						if(pCut)  //��value���������������ע�ͣ� ��û��value����������section���������section1   section2������1����key��2����ע�ͣ�vOneValueUnit������null
						{
								//ͨ���ж��Ƿ���������������value
								string strCut = pCut;
								IsANumberString(strCut,bInteger,bFloat);
								//��value�����
								if((bInteger || bFloat)&& pCut) 
								{
										while((bInteger || bFloat)&& pCut)  //ͨ��ѭ����ÿһ�����ݵ�value����vOneValueUnit������ע���˳�ѭ��������ע�ʹ���ģ��
										{	
												string strValue;
												strValue = pCut;      
												vOneValueUnit.push_back(strValue); 
												pCut = strtok(NULL, delim);	
												if(pCut) //�ж���һ��string�Ƿ�Ϊ�գ������Ϊ�գ��ж��Ƿ�������
												{
														string strCut = pCut;
														IsANumberString(strCut,bInteger,bFloat);
												}
												else//�����һ��stringΪ�գ�����value����û��ע�͵����������ע�ʹ���ģ�飬�˳�����ѭ��
												{continue;}
										}	
										while(pCut)  //ͨ��ѭ����ÿһ�����ݵ�ע������vOneCommentUnit
										{			
												string strComment;
												strComment = pCut;  //�ָ����ַ�������strComment
												vOneCommentUnit.push_back(strComment); //��strComment����vOneCommentUnit
												pCut = strtok(NULL, delim);	
										}
								}
								//û��value�������section1   section2������1����key��2����ע�ͣ�vOneValueUnit������null
								else
								{
										//û��value������vOneValueUnit������NULL
										string strValue;    
										vOneValueUnit.push_back(strValue); 

										//section������Ϊע�ͱ���
										if(pCut)  
										{			
												string strComment;
												strComment = pCut;      //�ָ���ע�͸���strComment
												vOneCommentUnit.push_back(strComment); //��strComment����vOneCommentUnit
										}
								}
								vOneKVC.push_back(vOneValueUnit);
								vOneKVC.push_back(vOneCommentUnit);
								vAllKeyValueComment.push_back(vOneKVC);//��ÿһ�����ݵ�key��value�飬ע��������vAllKeyValueComment

								vOneValueUnit.clear();
								vOneCommentUnit.clear();
								vOneKVC.clear();
								continue;
						}
						else   //û��value��ע�͵����������section
						{
								string strValue;    
								vOneValueUnit.push_back(strValue); 
								vOneKVC.push_back(vOneValueUnit);
								vOneValueUnit.clear();

								//vOneValueUnit,vOneCommentUnit��������null
								string strComment;		
								vOneCommentUnit.push_back(strComment); 
								vOneKVC.push_back(vOneCommentUnit);
								vOneCommentUnit.clear();
						}
						vAllKeyValueComment.push_back(vOneKVC); //��ÿһ�����ݵ�key��value�飬ע��������vAllKeyValueComment
						vOneKVC.clear();
				}
		}
		fclose(fp);


		char chNewValue[INPUT_STR_LEN]={0};   //�û�ÿ�������Ҫ�޸ĵ�value
		unsigned int nNewValueUnit = 0;   //�û�Ҫ�޸ĵ�value�������
		unsigned int nCombinationLineNo;    //nCombinationLineNoΪ�û�Ҫ������ϵ�value����к�
		unsigned int nProduct = 1;    //nProductΪ�������ĳ˻�������ϵ��������
		map<int,int>mFindRepate;  //����һ��map�������û�Ҫ������ϵ�<�кţ����е�value����>�����ڲ����Ƿ��ظ��޸�
		map<int,int>::iterator mItFR;  //mItFRָ��mFindRepate

		printf("������Ҫ��ϵ��кţ��ԡ�0��������\n");
		scanf("%d",&nCombinationLineNo);  
		if(nCombinationLineNo != 0)//�����һ�ξ����롮0�����˳�main0��������ʾ�û�ȷ��Ҫ�˳���
		{
				while(nCombinationLineNo != 0)    //ѭ������key��value�飬ֱ��������ǡ�0��Ϊֹ
				{
						//ȷ��Ҫ�޸ĵ��к��Ѵ��ڣ����������Χ�����û��ٴ�����
						if((nCombinationLineNo > 0) && (nCombinationLineNo < nDisplayLineNo))
						{
								//ȷ��Ҫ�޸ĵ�value��Ϊ�գ�������section��������������û��������޸�section����
								if(!vAllKeyValueComment[nCombinationLineNo-1][1][0].empty() )
								{
										printf("��Ҫ�μ���ϵ�ԭ�������£�\n");
										string strBeforeAlterKey = vAllKeyValueComment[nCombinationLineNo-1][0][0]; //����޸�����ǰ��key
										printf("%s = \t",strBeforeAlterKey.c_str());
										for(unsigned int nBeforeAlter1 = 1;nBeforeAlter1 < vAllKeyValueComment[nCombinationLineNo-1].size();nBeforeAlter1++)
										{
												for(unsigned int nBeforeAlter2 = 0; nBeforeAlter2 < vAllKeyValueComment[nCombinationLineNo-1][nBeforeAlter1].size(); nBeforeAlter2++)
												{
														//ѭ������޸�����ǰ��value��ע��
														string strBeforeAlterVC = vAllKeyValueComment[nCombinationLineNo-1][nBeforeAlter1][nBeforeAlter2];
														printf("%s\t",strBeforeAlterVC.c_str());
												}
										}
										printf("\n"); 
										int nInputUnitCount = vAllKeyValueComment[nCombinationLineNo-1][1].size();//nInputUnitCountΪ�û�Ҫ�޸ĵ�value�������
										//����nEraseΪvAllKeyValueComment[nCombinationLineNo-1]���һ��Ԫ�ص���ţ�����ɾ��
										unsigned int nErase = vAllKeyValueComment[nCombinationLineNo-1].size()-1;
										vector<string>vTempComment;//������ʱ����ע����
										if(vAllKeyValueComment[nCombinationLineNo-1][nErase].size()>0)  //vAllKeyValueComment[nCombinationLineNo-1]�����һ����ע��
										{
												//����strCommentTemp�������ж����һ���Ƿ���ע��
												string strCommentTemp = vAllKeyValueComment[nCombinationLineNo-1][nErase][0];
												IsANumberString(strCommentTemp,bInteger,bFloat);
												if(bInteger || bFloat)//���һ�鲻��ע�ͣ�ʡ�Ա���ע�͵Ĳ��裬ֱ��ɾ��value
												{
														for(;nErase>=1; nErase--)//����ɾ��value�飬ֱ��key��ֹ
														{
																vAllKeyValueComment[nCombinationLineNo-1].erase(vAllKeyValueComment[nCombinationLineNo-1].begin()+nErase);
														}
												}
												else//���һ����ע�ͣ�����ע�ͺ���ɾ��value
												{	
														vector<string>vTempComment2(vAllKeyValueComment[nCombinationLineNo-1][nErase]);//��ʱ����ע����  
														vTempComment.assign(vTempComment2.begin(),vTempComment2.end());
														for(;nErase>=1; nErase--)//����ɾ��value�飬ֱ��key��ֹ
														{
																vAllKeyValueComment[nCombinationLineNo-1].erase(vAllKeyValueComment[nCombinationLineNo-1].begin()+nErase);
														}
												}
										}
										else  //vAllKeyValueComment[nCombinationLineNo-1]�����һ����ע��
										{
												for(;nErase>=1; nErase--)//����ɾ��value�飬ֱ��key��ֹ
												{
														vAllKeyValueComment[nCombinationLineNo-1].erase(vAllKeyValueComment[nCombinationLineNo-1].begin()+nErase);
												}
										}

										printf("��Ҫ���뼸�����ݣ�\n");
										scanf("%d",&nNewValueUnit);
										if(nNewValueUnit<=1) nNewValueUnit = 1;
										if(nNewValueUnit >100) nNewValueUnit = 100;
										vector<string>vOneNewValueUnit;    //���һ���������value
										printf("---������%d�����ݣ��Կո�ָ\n",nNewValueUnit);
										for(int nUnitNo = 0; nUnitNo < nNewValueUnit; nUnitNo++)   //ѭ������value��
										{
												for(int nValueNo = 0; nValueNo < nInputUnitCount; nValueNo++)//ѭ������value
												{
														scanf("%s",chNewValue);
														string strNewValue(chNewValue);
														vOneNewValueUnit.push_back(strNewValue);  //��������ÿһ��strNewValue		
												}
												vAllKeyValueComment[nCombinationLineNo-1].push_back(vOneNewValueUnit);  //����һ���µ�value��
												vOneNewValueUnit.clear();
										}
										vAllKeyValueComment[nCombinationLineNo-1].push_back(vTempComment);  //����֮ǰ�����ע��
										vTempComment.clear();
										printf("�����޸�Ϊ��\n");
										string strAfterAlterKey = vAllKeyValueComment[nCombinationLineNo-1][0][0];
										printf("%s =  ",strAfterAlterKey.c_str()); //����޸ĺ��key
										for(unsigned int nAfterAlter1 = 1; nAfterAlter1 < vAllKeyValueComment[nCombinationLineNo-1].size(); nAfterAlter1++)
										{
												for(unsigned int nAfterAlter2 = 0; nAfterAlter2 < vAllKeyValueComment[nCombinationLineNo-1][nAfterAlter1].size(); nAfterAlter2++)
												{
														string strAfterAlterVC = vAllKeyValueComment[nCombinationLineNo-1][nAfterAlter1][nAfterAlter2];
														printf("%s\t",strAfterAlterVC.c_str());  //ѭ������޸ĺ��value��ע��
												}
										}
										printf("\n");

										mItFR = mFindRepate.find(nCombinationLineNo);  //��mFindRepate�в���֮ǰ�Ƿ��޸Ĺ��к�ΪnCombinationLineNo��value
										if(mItFR == mFindRepate.end()) //֮ǰδ�޸�value
										{
												int nAlterValueUnitNum = (vAllKeyValueComment[nCombinationLineNo-1].size()-2);//nAlterValueUnitNumΪ�����޸ĵ�value�������
												mFindRepate.insert(make_pair(nCombinationLineNo,nAlterValueUnitNum));//����<�����޸ĵ��кţ�value�������>��mFindRepate��
												nProduct = nProduct * (vAllKeyValueComment[nCombinationLineNo-1].size()-2);  //�˴���ͳ��key��ע�ͣ�ֻͳ��value��������ĳ˻�����size��2
												vPartKeyValueComment.push_back(vAllKeyValueComment[nCombinationLineNo-1]);  //���汾���޸ĵ����ݵ�vPartKeyValueComment�У������ļ�������
										}
										else    //֮ǰ�Ѿ��޸Ĺ�value
										{
												int nRAQ = (*mItFR).second;//nRAQ��nRepateAlterQualtity,�ظ��޸ĵ�����
												nProduct = nProduct / nRAQ;  //�ȳ����ϴ��޸ĵ�value�������
												nProduct = nProduct * (vAllKeyValueComment[nCombinationLineNo-1].size()-2);//�ٳ��Ա����޸ĵ�value�������
												int nRALN = ((*mItFR).first)-1;  //����nRALNΪ�ظ��޸ĵ��к�

												//��vPartKeyValueComment�в����ٴ��޸ĵ�Ԫ�أ��ҵ����滻Ϊ���µ�vAllKeyValueComment[nCombinationLineNo-1]
												vector<vector<vector<string> > >::iterator vReplacePartKV;  //vReplacePartKVָ��vPartKeyValueComment
												for(vReplacePartKV =vPartKeyValueComment.begin() ; vReplacePartKV!= vPartKeyValueComment.end();vReplacePartKV++)
												{       
														if((*vReplacePartKV)[0][0] == vAllKeyValueComment[nCombinationLineNo-1][0][0])  //key��ȼ��ҵ�
														{
																*vReplacePartKV = vAllKeyValueComment[nCombinationLineNo-1];  //����vPartKeyValueComment
														}
												}
												int nAlterValueUnitNum = (vAllKeyValueComment[nCombinationLineNo-1].size()-2);//�˴���ͳ��key��ע�ͣ�ֻͳ��value��������ĳ˻�����size��2
												(*mItFR).second = nAlterValueUnitNum;  //����mItFR���к�ΪnCombinationLineNo��value�������
										}
										printf("���������Ҫ��ϵ���ţ��ԡ�0��������\n");
										scanf("%d",&nCombinationLineNo);
								}
								else
								{
										printf("���������������޷��ı�section���֣�\n");
										printf("���������Ҫ��ϵ���ţ��ԡ�0��������\n");
										scanf("%d",&nCombinationLineNo);		
								}
						}
						else
						{
								printf("�������������󣬳�����Χ��\n");
								printf("������1��%d֮�����ţ�\n",nDisplayLineNo-1);
								scanf("%d",&nCombinationLineNo);
						}
						continue;
				}
		}
		else//�����һ�ξ����롮0��
		{
				return 0;
		}

		printf("���������޸Ľ�����         �������޸ĵ��������£�\n"); 
		//ѭ������û�����޸Ĺ�����������
		for(unsigned int nAAAA = 0;nAAAA < vPartKeyValueComment.size();nAAAA++)
		{
				for(unsigned int nDisNe = 0; nDisNe < vPartKeyValueComment[nAAAA].size(); nDisNe++)
				{
						for(unsigned int nNewValue = 0; nNewValue < vPartKeyValueComment[nAAAA][nDisNe].size(); nNewValue++)
						{
								string strN = vPartKeyValueComment[nAAAA][nDisNe][nNewValue];
								printf("%s\t",strN.c_str());
						}
				}
				printf("\n");
		}

		char chCopyFilePath[INPUT_STR_LEN]={0};
		printf("��������Ҫ���Ƶ��ļ��е�·����<�������ļ��нУ�test>\n");
		scanf("%s",chCopyFilePath);

		vector<vector<string> >tmp_vec;//���һ�����
		vector<vector<vector<string> > >tmp_result;//������е����
		vector<vector<vector<string> > >tmp_result2;//������в�����ϵ���������������ļ���
		get_result_in_vector(vAllKeyValueComment, 0, tmp_vec, tmp_result);//������Ϻ���
		get_result_in_vector(vPartKeyValueComment, 0, tmp_vec, tmp_result2);//������Ϻ���
		int nRow = 0;  //����nRowΪtmp_result�����±꣬�������nFileNo���ļ���д���nRow����Ͻ����������ֵ���
		printf("������%d����������\n",nProduct);
		for(int nFileNo= 1; nFileNo <= nProduct; nFileNo++)//Ϊ���е�������ļ���->�ļ�->д������
		{
				//����chFileNo��strFileNameNo�����ļ��е���ţ������ļ�������ʱ��ǰ��������
				char chFileNo[100];  
				sprintf(chFileNo,"%d",nFileNo);
				string strFileNameNo(chFileNo);

				string strFileName;  //����strFileName���յ��ļ���ÿ�������е�key���һ��С�����������������ļ�������
				string strLastKeyForFileName;  //����strLastKeyForFileNameΪÿ�������е�key���һ��С�����������������ļ�������
				strFileName = strFileName + strFileNameNo + "-";
				int nFNRow = 0 ;  //nFNRowΪ�����ļ�������
				vector<vector<vector<string> > >::iterator vFNIter;   //��vIterָ��vAllKeyValueComment
				for(vFNIter = vPartKeyValueComment.begin(); vFNIter != vPartKeyValueComment.end(); vFNIter++)
				{
						//��ȡÿ�������е�key���һ��С�����������������ļ�������
						const char *delim = ".";  //delimΪ��ȡ��־	
						char chFileName[INPUT_STR_LEN];
						strcpy(chFileName, (*vFNIter)[0][0].c_str());
						char *pCut = strtok(chFileName, delim);
						while(pCut)   //ͨ��ѭ���ָ�ԭ����
						{
								strLastKeyForFileName = pCut;    
								pCut = strtok(NULL, delim);
						}
						strFileName = strFileName + strLastKeyForFileName;

						for(int nFNColumn = 0;nFNColumn < tmp_result2[nRow][nFNRow].size(); nFNColumn++) //nFNColumnΪ�����ļ�������
						{
								strFileName = strFileName +"-"+ tmp_result2[nRow][nFNRow][nFNColumn];
						}
						strFileName = strFileName + "_";
						nFNRow++;
				}

				char dirname[2048];//�½��ļ��е�����
				strcpy(dirname,strFileName.c_str());
				dirname[strlen(dirname)-1]=0;
				RemoveDirectory(dirname);
				bool flag =CreateDirectory(dirname, NULL);//�½��ļ���
				char chCopyFile[INPUT_STR_LEN]={0};
				sprintf(chCopyFile,"xcopy %s %s /s /e",chCopyFilePath,dirname);//s�������ļ��У�e���ƿ��ļ���
				system(chCopyFile);
				char buffer[INPUT_STR_LEN];     //�½�TXT�ļ�·��
				sprintf(buffer,"%s\\%s",dirname,"StaticSimuConfiguration.txt");
				FILE*ffp;
				if((ffp = fopen(buffer,"w")) == NULL)  
				{
						printf("�޷��򿪻��ҵ��ļ�!\n");
						exit(0);
				}
				printf("%d : �½�%s�ļ��гɹ�!\n",nFileNo,dirname);

				//����Ϊ�����ϵĽ�����½��ļ���
				vector<vector<vector<string> > >::iterator vIter;   //��vIterָ��vAllKeyValueComment
				int nColumn = 0 ; //����nColumnΪtmp_result�����±꣬������ÿ���½����ļ���д����ϵĵ�nRow��value���
				for(vIter = vAllKeyValueComment.begin(); vIter != vAllKeyValueComment.end(); vIter++)
				{
						fprintf(ffp,"%s  ", (*vIter)[0][0].c_str());  //���ÿ����ϵ�key
						string strDYH = (*vIter)[1][0];  //����strDYH�����ж�(*vIter)[1][0]�Ƿ������֣�������򽫡�=��д���ļ���
						IsANumberString(strDYH,bInteger,bFloat);
						if(((*vIter)[1].size() >= 1)&&(bInteger || bFloat))
						{
								fprintf(ffp,"=  "); 
						}

						for(int nPrintValue = 0;nPrintValue < tmp_result[nRow][nColumn].size(); nPrintValue++)
						{
								fprintf(ffp,"%s    ",tmp_result[nRow][nColumn][nPrintValue].c_str());//���ÿ����ϵ�value��
						}
						nColumn++;

						int nCommentNo = (*vIter).size()-1;  //����nCommentNoΪע����(*vIter)�е��±�
						for(int nPrintComments = 0; nPrintComments < (*vIter)[nCommentNo].size(); nPrintComments++)
						{
								fprintf(ffp,"%s    ",(*vIter)[nCommentNo][nPrintComments].c_str());//���ÿ����ϵ�ע��
						}
						fprintf(ffp,"\n");
				}
				nRow++;
				fclose(ffp);
		}
		return 0 ;
}

int main()
{
		main0();
		while(true)
		{
				printf("***************��ȷ��Ҫ�˳���     Y/N?******************\n");
				char input[2048];
				scanf("%s",input);
				if(input[0]=='Y' || input[0]== 'y')
				{
						exit(0);
				}
				main0();
		}
}