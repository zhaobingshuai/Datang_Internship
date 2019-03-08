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
*函数功能：N个keyvalue里各取一个value组，求出所有的组合，并存放到tmp_result
* 原理：利用栈的原理，将每个vec的第一个value组压入tmp，并保存到tmp_result
*弹出末尾value组，再压入最后一组数据的第二个value组，保存后弹出，再压入最后一组数据的第三个value组，保存后弹出，依次循环
**************************************************************************************************************/
void get_result_in_vector(vector<vector<vector<string> > >&vec,unsigned int N,vector<vector<string> >&tmp,vector<vector<vector<string> > >&tmp_result)
{
		for(unsigned int nResultColumn = 1; nResultColumn <(vec[N].size()-1); ++nResultColumn)
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

int main0()
{
		bool bInteger;  //验证是否是一个整数
		bool bFloat;    //验证是否是一个浮点数
		vector<vector<vector<string> > >vAllKeyValueComment; //定义3维vector，存放读取出的所有key、value、注释
		vector<vector<vector<string> > >vPartKeyValueComment; //定义3维vector，存放所有被修改的key、value、注释，用于文件夹命名
		vector<vector<string> >vOneKVC; //定义2维vector，存放每一次读取的vOneKey、vOneValueUnit、vOneCommentUnit
		vector<string>vOneKey; //定义一维vector，存放每一组的key
		vector<string>vOneValueUnit; //定义一维vector，存放每一组的value
		vector<string>vOneCommentUnit; //定义一维vector，存放每一组的注释

		FILE*fp;                
		if((fp=fopen("StaticSimuConfiguration.txt","r+")) == NULL)  //打开文件 StaticSimuConfiguration.txt
		{
				printf("无法打开或找到文件!\n");
				exit(0);
		}
		printf("打开StaticSimuConfiguration.txt文件成功!\n");

		char chReadStringFromFile[INPUT_STR_LEN] = {0};  //chReadStringFromFile存放从文件中读取的字符串
		int nDisplayLineNo = 1;            //nDisplayLineNo为显示读取的字符串的行号
		int dddd=1;
		while(!feof(fp))           //feof检测文件是否结束       
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
						/*******************以下内容起控制字符串长度的作用******************/
						int len=strlen(chReadStringFromFile);
						if(len==0) {continue;}
						if(len>=1) {if((chReadStringFromFile[len-1]==0x0a) || (chReadStringFromFile[len-1]==0x0d)){chReadStringFromFile[len-1]=0;}}
						if(len>=2) {if((chReadStringFromFile[len-2]==0x0a) || (chReadStringFromFile[len-2]==0x0d)){chReadStringFromFile[len-2]=0;}}
						if(len==0) {continue;}
						/*******************以上内容起控制字符串长度的作用*******************/

						printf("%d： %s\n",nDisplayLineNo,chReadStringFromFile);   //显示：<行号：分割前原数据>
						nDisplayLineNo++;  //显示字符串的行号自增

						const char *delim = " =\t";  //delim为截取标志	
						char *pCut = strtok(chReadStringFromFile, delim);//将pCut指向chReadStringFromFile中第一个截取标志前的字符串
						//保存每一行数据中的key到vOneKey中，同时保存到vOneKVC
						string strKey;
						strKey = pCut;     
						vOneKey.push_back(strKey);
						vOneKVC.push_back(vOneKey);  
						vOneKey.clear();

						pCut = strtok(NULL, delim);//将pCut指向chReadStringFromFile中的下一个截取标志前的字符串
						if(pCut)  //有value的情况（不论有无注释） 和没有value但是有两个section名的情况：section1   section2；其中1当做key，2当做注释，vOneValueUnit中推入null
						{
								//通过判断是否是数字区分有无value
								string strCut = pCut;
								IsANumberString(strCut,bInteger,bFloat);
								//有value的情况
								if((bInteger || bFloat)&& pCut) 
								{
										while((bInteger || bFloat)&& pCut)  //通过循环将每一组数据的value推入vOneValueUnit，遇到注释退出循环，进入注释处理模块
										{	
												string strValue;
												strValue = pCut;      
												vOneValueUnit.push_back(strValue); 
												pCut = strtok(NULL, delim);	
												if(pCut) //判断下一个string是否为空，如果不为空，判断是否是数字
												{
														string strCut = pCut;
														IsANumberString(strCut,bInteger,bFloat);
												}
												else//如果下一个string为空，即有value但是没有注释的情况，跳过注释处理模块，退出本次循环
												{continue;}
										}	
										while(pCut)  //通过循环将每一组数据的注释推入vOneCommentUnit
										{			
												string strComment;
												strComment = pCut;  //分割后的字符串赋给strComment
												vOneCommentUnit.push_back(strComment); //将strComment推入vOneCommentUnit
												pCut = strtok(NULL, delim);	
										}
								}
								//没有value的情况：section1   section2；其中1当做key，2当做注释，vOneValueUnit中推入null
								else
								{
										//没有value，跳过vOneValueUnit，推入NULL
										string strValue;    
										vOneValueUnit.push_back(strValue); 

										//section名字视为注释保存
										if(pCut)  
										{			
												string strComment;
												strComment = pCut;      //分割后的注释赋给strComment
												vOneCommentUnit.push_back(strComment); //将strComment推入vOneCommentUnit
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
						else   //没有value和注释的情况：例如section
						{
								string strValue;    
								vOneValueUnit.push_back(strValue); 
								vOneKVC.push_back(vOneValueUnit);
								vOneValueUnit.clear();

								//vOneValueUnit,vOneCommentUnit依次推入null
								string strComment;		
								vOneCommentUnit.push_back(strComment); 
								vOneKVC.push_back(vOneCommentUnit);
								vOneCommentUnit.clear();
						}
						vAllKeyValueComment.push_back(vOneKVC); //将每一组数据的key，value组，注释组推入vAllKeyValueComment
						vOneKVC.clear();
				}
		}
		fclose(fp);


		char chNewValue[INPUT_STR_LEN]={0};   //用户每次输入的要修改的value
		unsigned int nNewValueUnit = 0;   //用户要修改的value组的数量
		unsigned int nCombinationLineNo;    //nCombinationLineNo为用户要参与组合的value组的行号
		unsigned int nProduct = 1;    //nProduct为组合情况的乘积，即组合的总情况数
		map<int,int>mFindRepate;  //定义一个map，保存用户要参与组合的<行号，该行的value组数>，用于查找是否重复修改
		map<int,int>::iterator mItFR;  //mItFR指向mFindRepate

		printf("请输入要组合的行号：以‘0’结束！\n");
		scanf("%d",&nCombinationLineNo);  
		if(nCombinationLineNo != 0)//如果第一次就输入‘0’，退出main0函数，提示用户确认要退出吗？
		{
				while(nCombinationLineNo != 0)    //循环输入key和value组，直到输入的是‘0’为止
				{
						//确保要修改的行号已存在，如果超出范围提醒用户再次输入
						if((nCombinationLineNo > 0) && (nCombinationLineNo < nDisplayLineNo))
						{
								//确保要修改的value不为空，即不是section，如果是则提醒用户不允许修改section名字
								if(!vAllKeyValueComment[nCombinationLineNo-1][1][0].empty() )
								{
										printf("您要参加组合的原数据如下：\n");
										string strBeforeAlterKey = vAllKeyValueComment[nCombinationLineNo-1][0][0]; //输出修改数据前的key
										printf("%s = \t",strBeforeAlterKey.c_str());
										for(unsigned int nBeforeAlter1 = 1;nBeforeAlter1 < vAllKeyValueComment[nCombinationLineNo-1].size();nBeforeAlter1++)
										{
												for(unsigned int nBeforeAlter2 = 0; nBeforeAlter2 < vAllKeyValueComment[nCombinationLineNo-1][nBeforeAlter1].size(); nBeforeAlter2++)
												{
														//循环输出修改数据前的value和注释
														string strBeforeAlterVC = vAllKeyValueComment[nCombinationLineNo-1][nBeforeAlter1][nBeforeAlter2];
														printf("%s\t",strBeforeAlterVC.c_str());
												}
										}
										printf("\n"); 
										int nInputUnitCount = vAllKeyValueComment[nCombinationLineNo-1][1].size();//nInputUnitCount为用户要修改的value组的数量
										//定义nErase为vAllKeyValueComment[nCombinationLineNo-1]最后一组元素的序号，用于删除
										unsigned int nErase = vAllKeyValueComment[nCombinationLineNo-1].size()-1;
										vector<string>vTempComment;//用于临时保存注释组
										if(vAllKeyValueComment[nCombinationLineNo-1][nErase].size()>0)  //vAllKeyValueComment[nCombinationLineNo-1]的最后一组有注释
										{
												//定义strCommentTemp，用于判断最后一组是否是注释
												string strCommentTemp = vAllKeyValueComment[nCombinationLineNo-1][nErase][0];
												IsANumberString(strCommentTemp,bInteger,bFloat);
												if(bInteger || bFloat)//最后一组不是注释，省略保存注释的步骤，直接删除value
												{
														for(;nErase>=1; nErase--)//依次删除value组，直到key截止
														{
																vAllKeyValueComment[nCombinationLineNo-1].erase(vAllKeyValueComment[nCombinationLineNo-1].begin()+nErase);
														}
												}
												else//最后一组是注释，保存注释后再删除value
												{	
														vector<string>vTempComment2(vAllKeyValueComment[nCombinationLineNo-1][nErase]);//临时保存注释组  
														vTempComment.assign(vTempComment2.begin(),vTempComment2.end());
														for(;nErase>=1; nErase--)//依次删除value组，直到key截止
														{
																vAllKeyValueComment[nCombinationLineNo-1].erase(vAllKeyValueComment[nCombinationLineNo-1].begin()+nErase);
														}
												}
										}
										else  //vAllKeyValueComment[nCombinationLineNo-1]的最后一组无注释
										{
												for(;nErase>=1; nErase--)//依次删除value组，直到key截止
												{
														vAllKeyValueComment[nCombinationLineNo-1].erase(vAllKeyValueComment[nCombinationLineNo-1].begin()+nErase);
												}
										}

										printf("您要输入几组数据？\n");
										scanf("%d",&nNewValueUnit);
										if(nNewValueUnit<=1) nNewValueUnit = 1;
										if(nNewValueUnit >100) nNewValueUnit = 100;
										vector<string>vOneNewValueUnit;    //存放一组新输入的value
										printf("---请输入%d组数据，以空格分割：\n",nNewValueUnit);
										for(int nUnitNo = 0; nUnitNo < nNewValueUnit; nUnitNo++)   //循环输入value组
										{
												for(int nValueNo = 0; nValueNo < nInputUnitCount; nValueNo++)//循环输入value
												{
														scanf("%s",chNewValue);
														string strNewValue(chNewValue);
														vOneNewValueUnit.push_back(strNewValue);  //存放输入的每一组strNewValue		
												}
												vAllKeyValueComment[nCombinationLineNo-1].push_back(vOneNewValueUnit);  //推入一组新的value组
												vOneNewValueUnit.clear();
										}
										vAllKeyValueComment[nCombinationLineNo-1].push_back(vTempComment);  //推入之前保存的注释
										vTempComment.clear();
										printf("您已修改为：\n");
										string strAfterAlterKey = vAllKeyValueComment[nCombinationLineNo-1][0][0];
										printf("%s =  ",strAfterAlterKey.c_str()); //输出修改后的key
										for(unsigned int nAfterAlter1 = 1; nAfterAlter1 < vAllKeyValueComment[nCombinationLineNo-1].size(); nAfterAlter1++)
										{
												for(unsigned int nAfterAlter2 = 0; nAfterAlter2 < vAllKeyValueComment[nCombinationLineNo-1][nAfterAlter1].size(); nAfterAlter2++)
												{
														string strAfterAlterVC = vAllKeyValueComment[nCombinationLineNo-1][nAfterAlter1][nAfterAlter2];
														printf("%s\t",strAfterAlterVC.c_str());  //循环输出修改后的value和注释
												}
										}
										printf("\n");

										mItFR = mFindRepate.find(nCombinationLineNo);  //在mFindRepate中查找之前是否修改过行号为nCombinationLineNo的value
										if(mItFR == mFindRepate.end()) //之前未修改value
										{
												int nAlterValueUnitNum = (vAllKeyValueComment[nCombinationLineNo-1].size()-2);//nAlterValueUnitNum为本次修改的value组的数量
												mFindRepate.insert(make_pair(nCombinationLineNo,nAlterValueUnitNum));//保存<本次修改的行号，value组的数量>到mFindRepate中
												nProduct = nProduct * (vAllKeyValueComment[nCombinationLineNo-1].size()-2);  //此处不统计key和注释，只统计value组的数量的乘积，故size减2
												vPartKeyValueComment.push_back(vAllKeyValueComment[nCombinationLineNo-1]);  //保存本次修改的数据到vPartKeyValueComment中，用于文件夹命名
										}
										else    //之前已经修改过value
										{
												int nRAQ = (*mItFR).second;//nRAQ即nRepateAlterQualtity,重复修改的数量
												nProduct = nProduct / nRAQ;  //先除掉上次修改的value组的数量
												nProduct = nProduct * (vAllKeyValueComment[nCombinationLineNo-1].size()-2);//再乘以本次修改的value组的数量
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
												int nAlterValueUnitNum = (vAllKeyValueComment[nCombinationLineNo-1].size()-2);//此处不统计key和注释，只统计value组的数量的乘积，故size减2
												(*mItFR).second = nAlterValueUnitNum;  //更新mItFR中行号为nCombinationLineNo的value组的数量
										}
										printf("请继续输入要组合的序号：以‘0’结束！\n");
										scanf("%d",&nCombinationLineNo);
								}
								else
								{
										printf("您输入的序号有误，无法改变section名字！\n");
										printf("请继续输入要组合的序号：以‘0’结束！\n");
										scanf("%d",&nCombinationLineNo);		
								}
						}
						else
						{
								printf("您输入的序号有误，超出范围！\n");
								printf("请输入1到%d之间的序号！\n",nDisplayLineNo-1);
								scanf("%d",&nCombinationLineNo);
						}
						continue;
				}
		}
		else//如果第一次就输入‘0’
		{
				return 0;
		}

		printf("本次数据修改结束！         您最终修改的数据如下：\n"); 
		//循环输出用户最后修改过的所有数据
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
		printf("请输入您要复制的文件夹的路径！<测试用文件夹叫：test>\n");
		scanf("%s",chCopyFilePath);

		vector<vector<string> >tmp_vec;//存放一种情况
		vector<vector<vector<string> > >tmp_result;//存放所有的情况
		vector<vector<vector<string> > >tmp_result2;//存放所有参与组合的情况，用来生成文件名
		get_result_in_vector(vAllKeyValueComment, 0, tmp_vec, tmp_result);//调用组合函数
		get_result_in_vector(vPartKeyValueComment, 0, tmp_vec, tmp_result2);//调用组合函数
		int nRow = 0;  //定义nRow为tmp_result的行下标，用于向第nFileNo个文件中写入第nRow种组合结果，两者数值相等
		printf("共生成%d种组合情况：\n",nProduct);
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
				system(chCopyFile);
				char buffer[INPUT_STR_LEN];     //新建TXT文件路径
				sprintf(buffer,"%s\\%s",dirname,"StaticSimuConfiguration.txt");
				FILE*ffp;
				if((ffp = fopen(buffer,"w")) == NULL)  
				{
						printf("无法打开或找到文件!\n");
						exit(0);
				}
				printf("%d : 新建%s文件夹成功!\n",nFileNo,dirname);

				//以下为输出组合的结果到新建文件中
				vector<vector<vector<string> > >::iterator vIter;   //将vIter指向vAllKeyValueComment
				int nColumn = 0 ; //定义nColumn为tmp_result的列下标，用于向每次新建的文件里写入组合的第nRow种value结果
				for(vIter = vAllKeyValueComment.begin(); vIter != vAllKeyValueComment.end(); vIter++)
				{
						fprintf(ffp,"%s  ", (*vIter)[0][0].c_str());  //输出每种组合的key
						string strDYH = (*vIter)[1][0];  //定义strDYH用来判断(*vIter)[1][0]是否是数字，如果是则将‘=’写入文件中
						IsANumberString(strDYH,bInteger,bFloat);
						if(((*vIter)[1].size() >= 1)&&(bInteger || bFloat))
						{
								fprintf(ffp,"=  "); 
						}

						for(int nPrintValue = 0;nPrintValue < tmp_result[nRow][nColumn].size(); nPrintValue++)
						{
								fprintf(ffp,"%s    ",tmp_result[nRow][nColumn][nPrintValue].c_str());//输出每种组合的value组
						}
						nColumn++;

						int nCommentNo = (*vIter).size()-1;  //定义nCommentNo为注释在(*vIter)中的下标
						for(int nPrintComments = 0; nPrintComments < (*vIter)[nCommentNo].size(); nPrintComments++)
						{
								fprintf(ffp,"%s    ",(*vIter)[nCommentNo][nPrintComments].c_str());//输出每种组合的注释
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
				printf("***************您确定要退出吗？     Y/N?******************\n");
				char input[2048];
				scanf("%s",input);
				if(input[0]=='Y' || input[0]== 'y')
				{
						exit(0);
				}
				main0();
		}
}