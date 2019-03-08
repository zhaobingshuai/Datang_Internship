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
#include<time.h>
#include<ctype.h>

using namespace std;

#define INPUT_STR_LEN 1000

/****************************************************************************************************************
*函数功能：N个keyvalue里各取一个value组，求出所有的组合，并存放到tmp_result
* 原理：利用栈的原理，将每个vec的第一个value组压入tmp，并保存到tmp_result
*弹出末尾value组，再压入最后一组数据的第二个value组，保存后弹出，再压入最后一组数据的第三个value组，保存后弹出，依次循环
**************************************************************************************************************/
void get_result_in_vector(vector<vector<vector<string> > >&vec,unsigned int N,vector<vector<string> >&tmp,vector<vector<vector<string> > >&tmp_result)
{
	for(unsigned int nResultColumn = 2; nResultColumn <(vec[N].size()-1); ++nResultColumn)
	{
		tmp.push_back(vec[N][nResultColumn]);//压入第N组value的第一个value组
		if (N < (vec.size()-1))
		{
			get_result_in_vector(vec, N+1, tmp, tmp_result); //递归，压入第N组value的下一个value组
		}
		else
		{
			vector<vector<string> >one_result;
			for(unsigned int nTmpColumn = 0; nTmpColumn < tmp.size(); ++nTmpColumn)
			{
				one_result.push_back(tmp.at(nTmpColumn));//存放一种情况
			}
			tmp_result.push_back(one_result);//存放所有情况
		}
		tmp.pop_back();//弹出末尾value组
	}
	N--;
}

//参数说明：
//输入参数：str，被验证的字符串
//输出参数:bInteger，是否是一个整数
//输出参数:bFloat，是否是一个浮点数
//返回：是一个浮点数或整数：true
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

//参数说明：
//输入参数：str，被验证的字符串
//输出参数:bComments，是否是一个注释
//返回：是一个行注释标志：true
bool IsACommentsSign(string str,bool &bComments)
{
	const string rsn="^//.*?$";
	regex rn(rsn);
	bComments = regex_match(str,rn);
	return bComments;
}
//函数功能：判断输入的是否是数字，如果不是提示用户重新输入正整数
//输入参数：chInteger，被验证的字符串
//返回：num，用户最终输入的正确的数字
int StrToInt(char*chInteger)
{
	string strInteger(chInteger);
	if(strcmp(chInteger,"over")) //strcmp(chInteger,"over")
	{
		long long num = 0;
		if(chInteger == NULL || *chInteger == '\0')
		{  return 0;}
		int sign = 1;
		while(*chInteger == ' ')
		{  chInteger++;}
		while(*chInteger != '\0')
		{
			if(*chInteger >= '0'  && *chInteger <= '9')
			{
				num = num *10 + (*chInteger - '0')*sign;
				if(num >= INT_MAX)
				{ return INT_MAX;}
				else if(num <= INT_MIN)
				{ return INT_MIN;}
			}
			else    { break;}
			chInteger++;
		}
		while(num == 0)
		{
			char chInteger[1000];
			printf("请重新输入一个正整数：        以‘over’结束！                        \n");
			printf("===============================================================================================\n");
			scanf("%s",chInteger);
			printf("===============================================================================================\n");
			if(strcmp(chInteger,"over")) 
			{
				num = StrToInt(chInteger);
			}
			else
			{return 0;}
		}
		return (int)num;
	}
	else   {return 0;}
}

//函数功能：检查用户要复制的目录是否存在，如果不存在，在combination函数中提示用户未找到该目录，请重新输入
//输入参数：用户输入的要复制的文件目录的路径
//返回：bValue，TRUE表示找到，FALSE表示未找到
bool CheckFolderExist(const string &strPath)
{
	WIN32_FIND_DATA wfd;
	bool bValue= false;
	HANDLE hFind = FindFirstFile(strPath.c_str(),&wfd);
	if((hFind!=INVALID_HANDLE_VALUE)&&(wfd.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)) //如果找到要复制的文件目录
	{
		bValue = true;
	}
	FindClose(hFind);
	return bValue; //TRUE表示找到，FALSE表示未找到
}

//验证从文件中读取的一行字符串是否都是空格，若是返回TRUE；若不是则表示读取的数据有效返回false
//实际文件中，有的行看似是空行，实际由空格组成，通过本函数验证是否是空格组成的空行，如果是不做处理，不是再分析。
//还有一个功能：原配置文件中，注释分为多行写，第二行之后为了保持队形，前面有一大段空格；
//为了处理后保留原空格，保持原来的格式，用strcat计算有多少空格，以strSpace保存，在combination函数中加到key前面。
bool ReadStringIsSpace(char chRead[],bool &bSpace,string &strSpace)
{
	int i = 0;
	bSpace = true;
	char chspace[1000] = {0};
	while(chRead[i] != '\0')
	{
		if(isspace(chRead[i]))  //是空格，继续判断下一个字符
		{
			i++;
			strcat(chspace,"      ");//chspace原本为空，每遇到一个空格，在后面加一段
			continue;
		}
		else    //不是空格，退出
		{
			bSpace = false;
			break;
		}
	}
	string testtest(chspace);
	strSpace = testtest;
	return bSpace;
//	return strSpace;
}


int combination()
{
	bool bInteger;  //验证是否是一个整数
	bool bFloat;    //验证是否是一个浮点数
	bool bComments;  //验证是否是一个行注释
	bool bSpace;    //验证字符串是否都是空格
	vector<vector<vector<string> > >vAllKeyValueComment; //定义3维vector，存放读取出的所有key、value、注释
	vector<vector<vector<string> > >vPartKeyValueComment; //定义3维vector，存放所有被修改的key、value、注释，用于文件夹命名
	vector<vector<string> >vOneKVC; //定义2维vector，存放每一次读取的vOneKey、vOneValueUnit、vOneCommentUnit
	vector<string>vOneKey; //定义一维vector，存放每一组的key
	vector<string>vOneEqual; //定义一维vector，存放每一组的等于号
	vector<string>vOneValueUnit; //定义一维vector，存放每一组的value
	vector<string>vOneCommentUnit; //定义一维vector，存放每一组的注释

	FILE*fp;                
	if((fp=fopen("StaticSimuConfiguration.txt","r+")) == NULL)  //打开文件 StaticSimuConfiguration.txt
	{
		printf("无法打开或找到文件!\n");
		exit(0);
	}

	char chReadStringFromFile[INPUT_STR_LEN] = {0};  //chReadStringFromFile存放从文件中读取的字符串
	int nDisplayLineNo = 1;            //nDisplayLineNo为显示读取的字符串的行号
	while(!feof(fp))           //feof检测文件是否结束       
	{
		chReadStringFromFile[0]=0;
		if(NULL == fgets(chReadStringFromFile, INPUT_STR_LEN, fp)) {
			continue;  
		}
		if(feof(fp)){
			continue;
		}
		string strSpace;
		ReadStringIsSpace(chReadStringFromFile,bSpace,strSpace);
		if((chReadStringFromFile[0]!=10)/*不能是空行*/ && !bSpace/*不是空行但是空格*/)  //chReadStringFromFile[0]!=10
		{
			/*******************以下内容起控制字符串长度的作用******************/
		    int len=strlen(chReadStringFromFile);
			if(len==0) {continue;}
			if(len>=1) {if((chReadStringFromFile[len-1]==0x0a) || (chReadStringFromFile[len-1]==0x0d)){chReadStringFromFile[len-1]=0;}}
			if(len>=2) {if((chReadStringFromFile[len-2]==0x0a) || (chReadStringFromFile[len-2]==0x0d)){chReadStringFromFile[len-2]=0;}}
			if(len==0) {continue;}
			/*******************以上内容起控制字符串长度的作用*******************/

			//chReadStringFromFile = 
			printf(" %d ： %s\n",nDisplayLineNo,chReadStringFromFile);   //显示：<行号：分割前原数据>
			nDisplayLineNo++;  //显示字符串的行号自增

			const char *delim = " \t";  //delim为截取标志	
			char *pCut = strtok(chReadStringFromFile, delim);//将pCut指向chReadStringFromFile中第一个截取标志前的字符串

			//保存每一行数据中的key到vOneKey中，同时保存到vOneKVC
			string strKey;
			strKey = pCut;  
			if(strSpace.size()>0)
			{
				strKey = strSpace + strKey;
			}
			vOneKey.push_back(strKey);
			vOneKVC.push_back(vOneKey);  
			vOneKey.clear();

			pCut = strtok(NULL, delim);//将pCut指向chReadStringFromFile中的下一个截取标志前的字符串
			if(pCut)  //a = 1;a = b; a = 1 2 3 //b c; a b c；其中a当做key，b c当做注释，vOneValueUnit中推入null
			{
				if(strcmp(pCut,"=") == 0) //a = 1;a = b; a = 1 2 3 //b c;
				{
					//保存等于号
					string strEqual = pCut;
					vOneEqual.push_back(strEqual);
					vOneKVC.push_back(vOneEqual);  
					vOneEqual.clear();

					pCut = strtok(NULL, delim);//将pCut指向chReadStringFromFile中的下一个截取标志前的字符串		
					if(pCut)    //         a = 1;           a = b;         a = 1 2 3 //b c;
					{
						while(pCut) //通过循环将每一组数据的value推入vOneValueUnit，遇到注释退出循环，进入注释处理模块
						{	
							string strValue;
							strValue = pCut;  
							IsACommentsSign(strValue,bComments);
							if(bComments)  //如果是“//”，后面全算作注释，不再判断
							{
								while(pCut)  //通过循环将每一组数据的注释推入vOneCommentUnit
								{			
									string strComment;
									strComment = pCut;  //分割后的字符串赋给strComment
									vOneCommentUnit.push_back(strComment); //将strComment推入vOneCommentUnit
									pCut = strtok(NULL, delim);	
								}
							}
							else  //不是注释，则算value保存
							{
								vOneValueUnit.push_back(strValue);
							}
							pCut = strtok(NULL, delim);		
						}	
					}
					vOneKVC.push_back(vOneValueUnit);
					vOneKVC.push_back(vOneCommentUnit);
					vAllKeyValueComment.push_back(vOneKVC);//将每一组数据的key，value组，注释组推入vAllKeyValueComment

					vOneValueUnit.clear();
					vOneCommentUnit.clear();
					vOneKVC.clear();
					continue;
				}
				else //a b c d 全算作注释
				{
					//vOneEqual中推入null
					string strEqual;
					vOneEqual.push_back(strEqual);
					vOneKVC.push_back(vOneEqual);  
					vOneEqual.clear();

					//vOneValueUnit中推入null
					string strValue;    
					vOneValueUnit.push_back(strValue); 
					vOneKVC.push_back(vOneValueUnit);
					vOneValueUnit.clear();

					while(pCut)
					{
						string strComment = pCut;	
						vOneCommentUnit.push_back(strComment); 
						pCut = strtok(NULL, delim);	
					}
					vOneKVC.push_back(vOneCommentUnit);
					vOneCommentUnit.clear();
				}
			}
			else   //a
			{
				//vOneEqual中推入null
				string strEqual;
				vOneEqual.push_back(strEqual);
				vOneKVC.push_back(vOneEqual);  
				vOneEqual.clear();

				//vOneValueUnit中推入null
				string strValue;    
				vOneValueUnit.push_back(strValue); 
				vOneKVC.push_back(vOneValueUnit);
				vOneValueUnit.clear();

				//vOneCommentUnit中推入null
				string strComment;		
				vOneCommentUnit.push_back(strComment); 
				vOneKVC.push_back(vOneCommentUnit);
				vOneCommentUnit.clear();
			}
			vAllKeyValueComment.push_back(vOneKVC); //将每一组数据的key，value组，注释组推入vAllKeyValueComment
			vOneKVC.clear();
		}
		else  //如果遇到空行，屏幕上显示空行，处理后也保留空行保存到配置文件中
		{
			/*******************以下内容起控制字符串长度的作用******************/
			int len=strlen(chReadStringFromFile);
			if(len==0) {continue;}
			if(len>=1) {if((chReadStringFromFile[len-1]==0x0a) || (chReadStringFromFile[len-1]==0x0d)){chReadStringFromFile[len-1]=0;}}
			if(len>=2) {if((chReadStringFromFile[len-2]==0x0a) || (chReadStringFromFile[len-2]==0x0d)){chReadStringFromFile[len-2]=0;}}
			if(len==0) {continue;}
			/*******************以上内容起控制字符串长度的作用*******************/

			printf(" %d ： %s\n",nDisplayLineNo,chReadStringFromFile);   //显示：<行号：分割前原数据>  //显示空行
			nDisplayLineNo++;  //显示字符串的行号自增

			string strKey;     
			vOneKey.push_back(strKey);
			vOneKVC.push_back(vOneKey);  
			vOneKey.clear();

			//vOneEqual中推入null
			string strEqual;
			vOneEqual.push_back(strEqual);
			vOneKVC.push_back(vOneEqual);  
			vOneEqual.clear();

			//vOneValueUnit中推入null
			string strValue;    
			vOneValueUnit.push_back(strValue); 
			vOneKVC.push_back(vOneValueUnit);
			vOneValueUnit.clear();

			//vOneCommentUnit中推入null
			string strComment;		
			vOneCommentUnit.push_back(strComment); 
			vOneKVC.push_back(vOneCommentUnit);
			vOneCommentUnit.clear();

			vAllKeyValueComment.push_back(vOneKVC); //将每一组数据的key，value组，注释组推入vAllKeyValueComment
			vOneKVC.clear();
		}
	}
	fclose(fp);


	char chNewValue[INPUT_STR_LEN]={0};   //用户每次输入的要修改的value
	unsigned int nNewValueUnit = 0;   //用户要修改的value组的数量
	unsigned int nCombinationLineNo;    //nCombinationLineNo为用户要参与组合的value组的行号
	unsigned int nRightNumber = 0;
	unsigned int nProduct = 1;    //nProduct为组合情况的乘积，即组合的总情况数
	map<int,int>mFindRepate;  //定义一个map，保存用户要参与组合的<行号，该行的value组数>，用于查找是否重复修改
	map<int,int>::iterator mItFR;  //mItFR指向mFindRepate
	printf("===============================================================================================\n");
	printf("请输入要参与组合的行号(整数,1到%d)：        以‘over’结束！                  \n",nDisplayLineNo-1);
	printf("===============================================================================================\n");
	char chCombinationLineNo[INPUT_STR_LEN];
	scanf("%s",chCombinationLineNo); 
	printf("===============================================================================================\n");
	nCombinationLineNo = StrToInt(chCombinationLineNo);
	if(strcmp(chCombinationLineNo,"over"))//如果第一次退出main0函数，提示用户确认要退出吗？// || strcmp(chCombinationLineNo,"OVER")==0   strcmp(chCombinationLineNo,"over")
	{
		while(strcmp(chCombinationLineNo,"over"))    //循环输入key和value组，直到输入的是‘0’为止    
		{
			//确保要修改的行号已存在，如果超出范围提醒用户再次输入
			if((nCombinationLineNo > 0) && (nCombinationLineNo < nDisplayLineNo))
			{
				//确保要修改的value不为空，即不是section，如果是则提醒用户不允许修改section名字
				IsACommentsSign(vAllKeyValueComment[nCombinationLineNo-1][0][0],bComments);
				if((vAllKeyValueComment[nCombinationLineNo-1][1][0].size() > 0) && !bComments)  //size() > 0，即不允许修改
				{
					printf("您要参加组合的原数据如下 ：                                           \n");
					printf("===============================================================================================\n");
					for(unsigned int nBeforeAlter1 = 0;nBeforeAlter1 < vAllKeyValueComment[nCombinationLineNo-1].size();nBeforeAlter1++)
					{
						for(unsigned int nBeforeAlter2 = 0; nBeforeAlter2 < vAllKeyValueComment[nCombinationLineNo-1][nBeforeAlter1].size(); nBeforeAlter2++)
						{
							//循环输出修改数据前的value和注释
							string strBeforeAlterVC = vAllKeyValueComment[nCombinationLineNo-1][nBeforeAlter1][nBeforeAlter2];
							printf("%s\t",strBeforeAlterVC.c_str());
						}
					}
					printf("\n");
					printf("===============================================================================================\n");
					int nInputUnitCount = vAllKeyValueComment[nCombinationLineNo-1][2].size();//nInputUnitCount为用户要修改的value组的数量
					//定义nErase为vAllKeyValueComment[nCombinationLineNo-1]最后一组元素的序号，用于删除
					unsigned int nErase = vAllKeyValueComment[nCombinationLineNo-1].size()-1;  //
					vector<string>vTempComment;//用于临时保存注释组
					if(vAllKeyValueComment[nCombinationLineNo-1][nErase].size()>0)  //vAllKeyValueComment[nCombinationLineNo-1]的最后一组有注释
					{
						//定义strCommentTemp，用于判断最后一组是否是注释
						string strCommentTemp = vAllKeyValueComment[nCombinationLineNo-1][nErase][0];
						IsANumberString(strCommentTemp,bInteger,bFloat);
						if(bInteger || bFloat)//最后一组不是注释，省略保存注释的步骤，直接删除value
						{
							for(;nErase>= 2; nErase--)//依次删除value组，直到key =截止
							{
								vAllKeyValueComment[nCombinationLineNo-1].erase(vAllKeyValueComment[nCombinationLineNo-1].begin()+nErase);
							}
						}
						else//最后一组是注释，保存注释后再删除value
						{	
							vector<string>vTempComment2(vAllKeyValueComment[nCombinationLineNo-1][nErase]);//临时保存注释组  
							vTempComment.assign(vTempComment2.begin(),vTempComment2.end());
							for(;nErase>= 2; nErase--)//依次删除value组，直到key =截止
							{
								vAllKeyValueComment[nCombinationLineNo-1].erase(vAllKeyValueComment[nCombinationLineNo-1].begin()+nErase);
							}
						}
					}
					else  //vAllKeyValueComment[nCombinationLineNo-1]的最后一组无注释
					{
						for(;nErase>= 2; nErase--)//依次删除value组，直到key =截止
						{
							vAllKeyValueComment[nCombinationLineNo-1].erase(vAllKeyValueComment[nCombinationLineNo-1].begin()+nErase);
						}
					}

					printf("您要输入几组数据？                                                    \n");
					printf("===============================================================================================\n");
					char chNewValueUnit[INPUT_STR_LEN];
					scanf("%s",chNewValueUnit);
					printf("===============================================================================================\n");
					nNewValueUnit = StrToInt(chNewValueUnit);//防止错误，如果不是数字，反复输入直到是数字
					if(nNewValueUnit<=1) nNewValueUnit = 1;
					if(nNewValueUnit >100) nNewValueUnit = 100;
					vector<string>vOneNewValueUnit;    //存放一组新输入的value
					printf("请您输入%d组数据，以空格或者回车分割：                  \n",nNewValueUnit);
					printf("===============================================================================================\n");
					for(int nUnitNo = 0; nUnitNo < nNewValueUnit; nUnitNo++)   //循环输入value组
					{
						for(int nValueNo = 0; nValueNo < nInputUnitCount; nValueNo++)//循环输入value
						{
							scanf("%s",chNewValue);
							string strNewValue(chNewValue);
							IsANumberString(strNewValue,bInteger,bFloat);
							/*while(!bInteger && !bFloat)
							{	
							printf("只允许输入整型或者浮点型，请您重新输入：\n");
							printf("===============================================================================================\n");
							scanf("%s",chNewValue);
							printf("===============================================================================================\n");
							string strNewValue(chNewValue);
							IsANumberString(strNewValue,bInteger,bFloat);
							}*/
							vOneNewValueUnit.push_back(strNewValue);  //存放输入的每一组strNewValue	
						}
						vAllKeyValueComment[nCombinationLineNo-1].push_back(vOneNewValueUnit);  //推入一组新的value组
						vOneNewValueUnit.clear();
					}
					vAllKeyValueComment[nCombinationLineNo-1].push_back(vTempComment);  //推入之前保存的注释
					vTempComment.clear();
					printf("===============================================================================================\n");
					printf("您已修改序号为< %d >的数据如下 ：                  \n",nCombinationLineNo);
					printf("===============================================================================================\n");
					//string strAfterAlterKey = vAllKeyValueComment[nCombinationLineNo-1][0][0];
					//printf("%s =  ",strAfterAlterKey.c_str()); //输出修改后的key
					for(unsigned int nAfterAlter1 = 0; nAfterAlter1 < vAllKeyValueComment[nCombinationLineNo-1].size(); nAfterAlter1++)
					{
						for(unsigned int nAfterAlter2 = 0; nAfterAlter2 < vAllKeyValueComment[nCombinationLineNo-1][nAfterAlter1].size(); nAfterAlter2++)
						{
							string strAfterAlterVC = vAllKeyValueComment[nCombinationLineNo-1][nAfterAlter1][nAfterAlter2];
							printf("%s\t",strAfterAlterVC.c_str());  //循环输出修改后的value和注释
						}
					}
					printf("\n");
					printf("===============================================================================================\n");

					mItFR = mFindRepate.find(nCombinationLineNo);  //在mFindRepate中查找之前是否修改过行号为nCombinationLineNo的value
					if(mItFR == mFindRepate.end()) //之前未修改value
					{
						int nAlterValueUnitNum = (vAllKeyValueComment[nCombinationLineNo-1].size()-3);//nAlterValueUnitNum为本次修改的value组的数量
						mFindRepate.insert(make_pair(nCombinationLineNo,nAlterValueUnitNum));//保存<本次修改的行号，value组的数量>到mFindRepate中
						nProduct = nProduct * (vAllKeyValueComment[nCombinationLineNo-1].size()-3);  //此处不统计key，等于号和注释，只统计value组的数量的乘积，故size减3
						vPartKeyValueComment.push_back(vAllKeyValueComment[nCombinationLineNo-1]);  //保存本次修改的数据到vPartKeyValueComment中，用于文件夹命名
					}
					else    //之前已经修改过value
					{
						int nRAQ = (*mItFR).second;//nRAQ即nRepateAlterQualtity,重复修改的数量
						nProduct = nProduct / nRAQ;  //先除掉上次修改的value组的数量
						nProduct = nProduct * (vAllKeyValueComment[nCombinationLineNo-1].size()-3);//再乘以本次修改的value组的数量
						int nRALN = ((*mItFR).first)-1;  //定义nRALN为重复修改的行号

						//在vPartKeyValueComment中查找再次修改的元素，找到后替换为最新的vAllKeyValueComment[nCombinationLineNo-1]
						vector<vector<vector<string> > >::iterator vReplacePartKV;  //vReplacePartKV指向vPartKeyValueComment
						for(vReplacePartKV =vPartKeyValueComment.begin() ; vReplacePartKV!= vPartKeyValueComment.end();vReplacePartKV++)
						{       
							if((*vReplacePartKV)[0][0] == vAllKeyValueComment[nCombinationLineNo-1][0][0])  //key相等即找到
							{
								*vReplacePartKV = vAllKeyValueComment[nCombinationLineNo-1];  //更新vPartKeyValueComment
							}
						}
						int nAlterValueUnitNum = (vAllKeyValueComment[nCombinationLineNo-1].size()-3);//此处不统计key和注释，只统计value组的数量的乘积，故size减2
						(*mItFR).second = nAlterValueUnitNum;  //更新mItFR中行号为nCombinationLineNo的value组的数量
					}
					printf("请继续输入要参与组合的行号(整数,1到%d)：        以‘over’结束！              \n",nDisplayLineNo-1);
					printf("===============================================================================================\n");
					scanf("%s",chCombinationLineNo); 
					printf("===============================================================================================\n");
					nCombinationLineNo = StrToInt(chCombinationLineNo);
					//string strCombinationLineNo(chCombinationLineNo);
				}
				else
				{

					printf("您输入的序号有误，不允许改变！             请继续输入要组合的序号：以‘over’结束              \n");                                                       
					printf("===============================================================================================\n");
					scanf("%s",chCombinationLineNo); 
					printf("===============================================================================================\n");
					nCombinationLineNo = StrToInt(chCombinationLineNo);
					//string strCombinationLineNo(chCombinationLineNo);
				}
			}
			else
			{
				printf("您输入的序号有误，超出范围！         请输入1到%d之间的序号！  以‘over’结束！\n",nDisplayLineNo-1);
				printf("===============================================================================================\n");
				scanf("%s",chCombinationLineNo); 
				printf("===============================================================================================\n");
				nCombinationLineNo = StrToInt(chCombinationLineNo);
				//string strCombinationLineNo(chCombinationLineNo);
			}
			continue;
		}

	}
	else//如果第一次就输入‘0’
	{
		return 0;
	}

	printf("本次数据修改结束！ 您最终修改的数据如下：\n"); 
	printf("===============================================================================================\n");
	//循环输出用户最后修改过的所有数据
	for(unsigned int nLastData = 0;nLastData < vPartKeyValueComment.size();nLastData++)
	{
		//string strLastKey = vPartKeyValueComment[nLastData][0][0];
		//printf("%s    =\t",strLastKey.c_str());
		for(unsigned int nLastA = 0; nLastA < vPartKeyValueComment[nLastData].size(); nLastA++)
		{
			for(unsigned int nLastB = 0; nLastB < vPartKeyValueComment[nLastData][nLastA].size(); nLastB++)
			{
				string strLastValueComment = vPartKeyValueComment[nLastData][nLastA][nLastB];
				printf("%s\t",strLastValueComment.c_str());
			}
		}
		printf("\n");
	}
	printf("===============================================================================================\n");

	char chCopyFilePath[INPUT_STR_LEN] = {0};
	printf("请输入您要复制的文件夹的路径！         输入‘over’可退出本次数据修改！                        \n");
	printf("===============================================================================================\n");
	bool bFindDir;
	scanf("%s",chCopyFilePath);
	if(strcmp(chCopyFilePath,"over"))
	{
		string strPath(chCopyFilePath);
		printf("===============================================================================================\n");
		bFindDir = CheckFolderExist(strPath);
	}
	else
	{
		printf("===============================================================================================\n");
		return 0;
	}
	while(bFindDir == false)
	{
		printf("未找到该目录，请重新输入您要复制的文件夹的路径！               输入‘over’可退出本次数据修改! \n");	
		printf("===============================================================================================\n");
		scanf("%s",chCopyFilePath);	
		printf("===============================================================================================\n");
		if(strcmp(chCopyFilePath,"over")) //如果用户输入over，退出
		{
			string strPath(chCopyFilePath);
			bool bFindDir = CheckFolderExist(strPath);
			if(bFindDir == true)
			{
				break;
			}
		}
		else
		{
			return 0;
		}
	}

	vector<vector<string> >tmp_vec;//存放一种情况
	vector<vector<vector<string> > >tmp_result;//存放所有的情况
	vector<vector<vector<string> > >tmp_result2;//存放所有参与组合的情况，用来生成文件名
	get_result_in_vector(vAllKeyValueComment, 0, tmp_vec, tmp_result);//调用组合函数
	get_result_in_vector(vPartKeyValueComment, 0, tmp_vec, tmp_result2);//调用组合函数
	int nRow = 0;  //定义nRow为tmp_result的行下标，用于向第nFileNo个文件中写入第nRow种组合结果，两者数值相等
	printf("共生成%d种组合情况                                           \n",nProduct);
	printf("===============================================================================================\n");
	for(int nFileNo= 1; nFileNo <= nProduct; nFileNo++)//为所有的情况建文件夹->文件->写入数据
	{
		//定义chFileNo、strFileNameNo保存文件夹的序号，用于文件夹命名时最前面加上序号
		char chFileNo[100];  
		sprintf(chFileNo,"%d",nFileNo);
		string strFileNameNo(chFileNo);

		string strFileName;  //定义strFileName最终的文件名每组数据中的key最后一个小数点后面的名字用于文件夹命名
		string strLastKeyForFileName;  //定义strLastKeyForFileName为每组数据中的key最后一个小数点后面的名字用于文件夹命名
		strFileName = strFileName + strFileNameNo + "-";
		int nFNRow = 0 ;  //nFNRow为生成文件名的行
		vector<vector<vector<string> > >::iterator vFNIter;   //将vIter指向vAllKeyValueComment
		for(vFNIter = vPartKeyValueComment.begin(); vFNIter != vPartKeyValueComment.end(); vFNIter++)
		{
			//截取每组数据中的key最后一个小数点后面的名字用于文件夹命名
			const char *delim = ".";  //delim为截取标志	
			char chFileName[INPUT_STR_LEN];
			strcpy(chFileName, (*vFNIter)[0][0].c_str());
			char *pCut = strtok(chFileName, delim);
			while(pCut)   //通过循环分割原数据
			{
				strLastKeyForFileName = pCut;    
				pCut = strtok(NULL, delim);
			}
			strFileName = strFileName + strLastKeyForFileName;

			for(int nFNColumn = 0;nFNColumn < tmp_result2[nRow][nFNRow].size(); nFNColumn++) //nFNColumn为生成文件名的列
			{
				strFileName = strFileName +"-"+ tmp_result2[nRow][nFNRow][nFNColumn];
			}
			strFileName = strFileName + "_";
			nFNRow++;
		}

		char dirname[2048];//新建文件夹的名字
		strcpy(dirname,strFileName.c_str());
		dirname[strlen(dirname)-1]=0;
		RemoveDirectory(dirname);
		bool flag =CreateDirectory(dirname, NULL);//新建文件夹
		char chCopyFile[INPUT_STR_LEN]={0};
		sprintf(chCopyFile,"xcopy %s %s /s /e",chCopyFilePath,dirname);//s复制子文件夹，e复制空文件夹
		system(chCopyFile); //复制目录
		char buffer[INPUT_STR_LEN];     //新建TXT文件路径
		sprintf(buffer,"%s\\%s",dirname,"StaticSimuConfiguration.txt");
		FILE*ffp;
		if((ffp = fopen(buffer,"w")) == NULL)  
		{
			printf("无法打开或找到文件!\n");
			exit(0);
		}
		printf("%d : 新建%s文件夹成功!\n",nFileNo,dirname);
		printf("===============================================================================================\n");

		//以下为输出组合的结果到新建文件中
		vector<vector<vector<string> > >::iterator vIter;   //将vIter指向vAllKeyValueComment
		int nColumn = 0 ; //定义nColumn为tmp_result的列下标，用于向每次新建的文件里写入组合的第nRow种value结果
		for(vIter = vAllKeyValueComment.begin(); vIter != vAllKeyValueComment.end(); vIter++)
		{
			fprintf(ffp,"%s  ", (*vIter)[0][0].c_str());  //输出每种组合的key

			//string strDYH = (*vIter)[2][0];  //定义strDYH用来判断(*vIter)[1][0]是否是数字，如果是则将‘=’写入文件中
			//IsANumberString(strDYH,bInteger,bFloat);
			if((*vIter)[2][0].size() >= 1)   //)&&(bInteger || bFloat)
			{
				fprintf(ffp,"= "); 
			}

			for(int nPrintValue = 0;nPrintValue < tmp_result[nRow][nColumn].size(); nPrintValue++)
			{
				fprintf(ffp,"%s ",tmp_result[nRow][nColumn][nPrintValue].c_str());//输出每种组合的value组
			}
			nColumn++;

			int nCommentNo = (*vIter).size()-1;  //定义nCommentNo为注释在(*vIter)中的下标
			for(int nPrintComments = 0; nPrintComments < (*vIter)[nCommentNo].size(); nPrintComments++)
			{
				fprintf(ffp,"   %s",(*vIter)[nCommentNo][nPrintComments].c_str());//输出每种组合的注释
			}
			fprintf(ffp,"\n");
		}
		nRow++;
		fclose(ffp);
	}
	return 0 ;
}

void CaculateWeekDay(int nYear, int nMonth, int nDay)
{
	if(nMonth == 1 || nMonth == 2)
	{
		nMonth += 12;//把一月和二月看成是上一年的十三月和十四月
		nYear--;
	}
	//基姆拉尔森计算公式
	int nWeek = (nDay + 2*nMonth + 3*(nMonth + 1)/5 + nYear + nYear/4 - nYear/100 + nYear/400) % 7;
	switch(nWeek)
	{
	case 0: printf("星期一\n");  break;		
	case 1: printf("星期二\n");  break;
	case 2: printf("星期三\n");  break;
	case 3: printf("星期四\n");  break;
	case 4: printf("星期五\n");  break;
	case 5: printf("星期六\n");  break;
	case 6: printf("星期日\n");  break;
	}
}

int main()
{
	combination();
	time_t temp;
	struct tm *t;
	time(&temp);
	t = localtime(&temp);
	printf("当前时间是：    %d年%d月%d日",t->tm_year+1900,t->tm_mon+1,t->tm_mday);
	printf("    %d时%d分%d秒    ",t->tm_hour,t->tm_min,t->tm_sec);
	CaculateWeekDay(t->tm_year+1900,t->tm_mon+1,t->tm_mday);  //调用计算星期的函数
	printf("===============================================================================================\n");
	while(true)
	{
		printf("您确定要退出吗？            Y/N                                       \n");
		printf("===============================================================================================\n");
		char input[2048];
		scanf("%s",input);
		printf("===============================================================================================\n");
		if(input[0]=='Y' || input[0]=='y')
		{
			exit(0);
		}
		combination();
	}
}

