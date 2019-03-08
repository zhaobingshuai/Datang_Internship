#include<stdio.h>
#include<iostream>
#include<vector>
#include<map>
#include<iterator>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<cstring>
#include <regex>    //正则表达式

using namespace std;

//通过引用方式显示strVec向量中的所有内容
void DisplayVector(vector<vector<string>>&strVec); 
bool IsANumberString(string str,bool &bInteger, bool &bFloat);
//通过引用方式，分析strVec,将有效数据的key和vector(value、原名、注释)插入到mKeyValue中
void AnalysisVector(vector<vector<string>>&strVec,map<int,string>&mLineNoKey,map<string,vector<string>>&mKeyValue);
//按照source.txt中有效数据的顺序保存key和value到NewData.txt中
void SaveToFile(map<int,string>&mLineNoKey,map<string,vector<string>>&mKeyValue);  
//查询子函数,输入key，输出对应的所有的信息：key，value，原名，注释
void FindKey(char strFind[1000],map<string,vector<string>>&mKeyValue);

int main()         //main函数                             
{ 
	vector<vector<string>>strVec;    //定义二维向量strVec，存放原数据--SplitString-->分割后的内容
	map<int,string>mLineNoKey;      //定义一个map: <行号,mKeyValue.key>
	map<string,vector<string>>mKeyValue; //AnalysisVector之后生成mKeyValue，其中存放key和vector(value、原名、注释)

	FILE*fp;                
	if((fp=fopen("source.txt","r+")) == NULL)  //打开source.txt文件     
	{
		printf("无法打开或找到文件!\n");
		exit(0);
	}

	char strOrigin[1000];      //strOrigin存放截取前的字符串
	while(!feof(fp))           //feof检测文件是否结束       
	{
		fgets(strOrigin, 1000, fp);  //从source.txt中读取数据到strOrigin数组中

		/*******************以下内容起控制字符串长度的作用******************/
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

		vector<string>vline;   //定义一维string型容器vline,存放每一组字符串分割后的所有结果
		printf("原数据： %s\n",strOrigin);   //显示分割前的原数据
		const char *delim =" \t";  //delim为截取标志，可任意添加其他标志，比如tab	
		char *pCut = strtok(strOrigin, delim);//将pCut指向strOrigin中的第一个截取标志前的字符串
		while(pCut)   //通过循环分割原数据
		{
			//printf("%s\t",pCut);//显示每次分割后的结果，调试用
			string strAfter;
			strAfter = pCut;      //分割后的字符串赋给strAfter
			pCut = strtok(NULL, delim);	//将pCut指向strOrigin中的下一个截取标志前的字符串
			vline.push_back(strAfter);  //将strAfter推入vline向量，即一维将pCut指向下一个截取标志前的字符串，若没有则返回NULL结束循环
		}
		strVec.push_back(vline);   //将一维向量vline推入二维向量strVec
	}
	fclose(fp);

	DisplayVector(strVec);    //通过引用方式显示strVec向量中的所有内容
	AnalysisVector(strVec,mLineNoKey,mKeyValue);//分析strVec,将有效数据的key和vector(value、原名、注释)插入到mKeyValue中
	SaveToFile(mLineNoKey,mKeyValue); //按照source.txt中有效数据的顺序保存key和value到NewData.txt中

	printf("*****************************************************************\n");
	printf("请输入您要查找的key:\n");
	char strFindKey[1000];  // find_key数组保存要查找的key值
	scanf("%s", strFindKey);
	FindKey(strFindKey,mKeyValue);  //调用查询函数，输入key，输出所有的信息：key,value，原名，注释

	system("pause");
	return 0;
}	

void DisplayVector(vector<vector<string>>&strVec)  
{
	printf("****************************************************************\n");
	for(unsigned int iDVa = 0; iDVa < strVec.size(); iDVa++)  //显示strVec向量中的所有结果，调试用
	{
		if(strVec[iDVa].size() > 2)   //过滤掉无效数据
		{
			printf("原数据分割后插入vector成功：");
			for(unsigned int iDVb = 0; iDVb < strVec[iDVa].size(); iDVb++)
			{
				string strvecValue = strVec[iDVa][iDVb];
				printf("%s\t",strvecValue.c_str());
			}
			printf("\n"); 
		}
	}
}


//函数功能：分析strVec,将有效数据的key和vector(value、原名、注释)插入到mKeyValue中
//输入：二维vector：strVec
//输出：二维map:mKeyValue
//strVector[1]和strVector[2]均为value，其中strVector[1]为最终的value；strVector[3]存放原名值，
//strVector[4]和strVector[5]均存放注释，其中strVector[4]为最终的注释串
void AnalysisVector(vector<vector<string>>&strVec,map<int,string>&mLineNoKey,map<string,vector<string>>&mKeyValue)
{
	bool bInteger;
	bool bFloat;
	vector<vector<string>>vNewVOC;//vNewVOC存放所有新的Value，Orignname，Comments,用于循环输出到终端，调试用
	vector<string>vNewLine;   //vNewLine存放每组数据的value，原名值，注释等
	int iOriginNo = 1;   //定义iOriginNo为行号
	string strKey;  //存放key
	string strVector[10];   //存放value，原名值，注释等

	for(unsigned int iVec= 0; iVec < strVec.size(); iVec++)  //遍历分析
	{
		if(strVec[iVec].size() > 2)   //1-如果数据完整：至少三个字符串
		{
			vector<string>::iterator vit = strVec[iVec].begin();
			strKey = *vit;   //保存key   
			vit++;  //跳过等于号
			vit++;  //第一个value
			IsANumberString(*vit,bInteger,bFloat);//调用函数判断是否是数字
			if(bInteger || bFloat)  //2-如果第三个是数字，反复加到value里，否则下一步
			{
				strVector[1] = *vit;  //保存第一个value 
				vit++;   //指向下一个字符串变量
				IsANumberString(*vit,bInteger,bFloat);//调用函数判断下一个是否是数字
				while((bInteger || bFloat ) && (vit != strVec[iVec].end())) //如果下一个还是数字，直到不是,并且还不到最后    
				{
					strVector[2] = *vit;   //保存下一个value
					strVector[1] = strVector[1] + "    " + strVector[2];  //加到第一个value后面，两个value之间加上空格  
					vit++;  
					if(vit != strVec[iVec].end())  //2-1如果还没有到结束，再判断
					{
						IsANumberString(*vit,bInteger,bFloat);//调用函数判断是否是数字
					}
					else  //2-1如果已经取完所有value，后面没有数据了，eg:num1 = 12.3 +34.5
					{
					    strVector[3] = "";
						strVector[4] = "";
						continue;
					}
				}	

				if(vit != strVec[iVec].end())  //3-如果value之后还有字符串，可能是原名标志或者注释
				{
					if((*vit == "原名") ||(*vit == "origin")) //4-如果有原名标志
					{
						vit++;   //跳过原名标志
						if (vit != strVec[iVec].end()) //5-有具体的原名值
						{
							strVector[3] = *vit;   //保存具体的原名值 
							vit++;   
							if(vit != strVec[iVec].end())    //6-如果原名之后还有注释
							{
								//不管注释中有多少空格，统一作为注释，直到结束
								strVector[4] = *vit;  //保存第一个注释串  
								vit++;    
								while(vit != strVec[iVec].end())    
								{
									strVector[5] = *vit;   //保存下一个注释串  
									strVector[4] = strVector[4] + "  " + strVector[5];//加到第一个字符串后面,多个注释之间加上空格  
									vit++; 
								}				
							}
							else     //6-原名之后没有注释
							{
								strVector[4] = "";  //注释的空位
							}
						}
						else   //5-有原名标志，但是没有具体的原名值，也没有注释
						{
							 strVector[3] = ""; //原名的空位
							 strVector[4] = ""; //注释的空位
						}
					}
					else    //4-没有原名标志，全算作注释
					{ 
						strVector[3] = ""; //原名的空位
						strVector[4] = *vit;   
					}
				}
				else  //3-value之后没有数据了,此处可能有问题或者多余
				{
					strVector[3] = ""; //原名的空位
					strVector[4] = ""; //注释的空位
				}
			}
			else//2-等于号之后不是数字了，即：没有value但是够3个字符串
			{
				continue;
			}
		}
		else   //1-如果数据不完整：不够3个字符串。
		{
			continue;
		}
		vNewLine.push_back(strVector[1]);  //将所有value推入vNewLine
		vNewLine.push_back(strVector[3]);  //将具体的原名值推入vNewLine 
		vNewLine.push_back(strVector[4]);  //将所有注释推入vNewLine

		mLineNoKey[iOriginNo] = strKey;//原始可用行,将行号和key插入mLineNoKey
		iOriginNo++;  
		mKeyValue.insert(make_pair(strKey,vNewLine)); //将key和vNewLine插入map中
		vNewVOC.push_back(vNewLine);  //将每组vNewLine推入vNewVOC  
		vNewLine.clear();  //清空vNewLine向量，给下一组数据进入
	}  //遍历结束


	printf("*****************************************************************\n");
	for(unsigned int iDVaa = 0; iDVaa < vNewVOC.size(); iDVaa++)//遍历将插入mKeyValue的vector中的内容输出，调试用，可去掉
	{	
		printf("已插入新vector(value+原名+注释)：");
		for(unsigned int iDVbb = 0; iDVbb < vNewVOC[iDVaa].size(); iDVbb++)
		{
			string strVoc = vNewVOC[iDVaa][iDVbb];
			printf("%s\t",strVoc.c_str());
		}
		printf("\n"); 
	}

	map<string,vector<string>>::iterator mIter;   //mIter指向mKeyValue
	printf("*****************************************************************\n");
	for(mIter = mKeyValue.begin(); mIter!=mKeyValue.end(); mIter++) //遍历输出mKeyValue中所有的key和value，调试用，可去掉
	{
		string strFirst = (*mIter).first.c_str();
		string strSecond = (*mIter).second[0];
		printf("插入mKeyValue成功: %s = %s\n",strFirst.c_str(),strSecond.c_str());
	}
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

//函数功能：按照source.txt中有效数据的顺序保存key和value到NewData.txt中
void SaveToFile(map<int,string>&mLineNoKey,map<string,vector<string>>&mKeyValue)  
{
	FILE*fp;
	if((fp = fopen("NewData.txt","w")) == NULL)  //打开NewData.txt文件
	{
		printf("无法打开或找到文件!\n");
		exit(0);
	}

	map<string,vector<string>>::iterator mIter;   //将mIter指向mKeyValue
	for(unsigned int i = 1; i <=  mLineNoKey.size(); i++)
	{
		string skey =  mLineNoKey[i];
		mIter = mKeyValue.find(skey);  //查找mLineNoKey中行号i对应的key
		if(mIter != mKeyValue.end())  //如果找到了
		{
			string svalue = (*mIter).second[0];//将key对应的value赋值给svalue
			fprintf(fp,"%s = %s\n",skey.c_str(),svalue.c_str());//按照source.txt中有效数据的顺序输出key = value到文件中
		}
	}
	fclose(fp);
} 

//查询子函数,输入key，在mKeyValue中查找对应的key，找到则输出所有的信息：key、value、原名、注释；
//找不到则提示用户重新输入要查找的key；不管是否找到，输入0可随时退出查询
//输入：strFind为要查询的key，通过引用传送mKeyValue
void FindKey(char strFind[1000], map<string,vector<string>>&mKeyValue) 
{
	map<string,vector<string>>::iterator mIter;   //mIter指向mKeyValue
	int  nFlag = 0;         //nFlag为结束查询标志
	while(!nFlag)        //反复输入key，直到找到对应的key为止。
	{
		string strFK(strFind);
		mIter = mKeyValue.find(strFK);   //mIter指向找到的数据的key
		if(mIter == mKeyValue.end())   //条件为真表示遍历结束都没找到
		{
			printf("*****没找到,请重新输入要查询的key；如果退出查询请输入‘0’!******\n");
			scanf("%s", strFind);  //输入下一次查询的key
			string strFK(strFind);

			string string0 = "0";
			if(strFK != string0)  //没找到但是不想找了，输入0可结束查询，不是0则继续查询。
			{
				mIter = mKeyValue.find(strFK);
				continue;  //退出本次循环，进行下一次查找
			}
			else
			{
				printf("**********************   本次查询结束！  ************************\n");
				nFlag = 1; //结束查找
			}
		}
		else   //找到的情况
		{
			printf("**********************  恭喜您，找到了!  ***********************\n");
			string second0 = (*mIter).second[0];     
			string second1 = (*mIter).second[1];     
			string second2 = (*mIter).second[2];  
			printf("*   key:       %s\n",(*mIter).first.c_str()); //输出用户查找的数据的key
			printf("*   value：    %s\n",second0.c_str());     //输出用户查找的数据的key对应的value
			printf("*   原名：     %s\n",second1.c_str());     //输出用户查找的数据的key对应的原名
			printf("*   注释：     %s\n",second2.c_str());     //输出用户查找的数据的key对应的注释

			printf("******如果您要继续查询，请输入‘1',如果退出查询请输入‘0’！****\n");
			int nOutOrGoon;     //nOutOrGoon为退出或者继续的标志
			scanf("%d", &nOutOrGoon);
			if(nOutOrGoon == 1)
			{   
				printf("********请重新输入您要查询的key值；如果退出请输入‘0’！********\n"); 
				scanf("%s", strFind);  //输入下一次查询的key
				string strFK(strFind);
				string string0 = "0";
				if(strFK != string0)  //决定继续查询又后悔了，输入0可结束查询，不是0则继续查询。
				{
					mIter = mKeyValue.find(strFK);
					continue;  //退出本次循环，进行下一次查找
				}
				else
				{
					printf("**********************   本次查询结束！  ************************\n");
					nFlag = 1; //结束查找
				}
			}
			else if(nOutOrGoon == 0)
			{
				printf("**********************   本次查询结束！  ************************\n");
				nFlag = 1; //结束查找
			}	
			else
			{
				printf("********************   您输入的指令有误！  **********************\n");
				printf("**********************   本次查询结束！  ************************\n");
				nFlag = 1; //结束查找
			}
		}
	}
}