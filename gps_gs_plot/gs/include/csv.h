#ifndef __CSV_H__
#define __CSV_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include<sstream>//istringstream 必须包含这个头文件
using namespace std;

class CSV{
public:
      char infilename[256];//--打开的文件名
      char outfilename[256];//--新建文件名
      vector<string> fields;//--声明一个字符串向量
      string oneline;//--保存一行数据
      ifstream infile;//--输入文件流
      ofstream outfile;//--输出文件流

public:
      void open_csv(char * filename);//--打开原始数据文件
      void creat_new_csv(char * filename);//--创建一个新的csv文件
      void read_oneline_of_csv(void);//--读取一行原始数据  
      void write_csv(int a,int b);//--将需要的数据写入新csv文件中
      void vector_clear(void);//--清空字符串向量
      void infile_close(void);//--关闭所有打开的文件
      void outfile_close(void);//--关闭所有打开的文件
public:
      template<class T>
      void write_to_csv(T data,bool is_need_endl,bool is_need_scientific_notation);//--写入数据模板

};
//--打开csv文件
void CSV::open_csv(char * filename)
{
 strcpy(this->infilename,filename);
 this->infile.open(this->infilename); //--打开文件
 if(!this->infile.is_open())
  cout<<"infilename文件打开失败"<<endl;
}
//--创建一个新的csv文件
void CSV::creat_new_csv(char * filename)
{
 strcpy(this->outfilename,filename);
 this->outfile.open(this->outfilename,ios::out|ios::trunc);
  if(!this->outfile.is_open())
 cout<<"outfilename打开失败"<<endl;
}
//--清空字符串向量
void CSV::vector_clear(void)
{
this->fields.clear();//--清空字符串向量
}
//--读取一行数据并拆解
void CSV::read_oneline_of_csv(void)
{
getline(this->infile,this->oneline);//--读取一行
istringstream tempstream(this->oneline);//将整行字符串line读入到字符串流istringstream中
string field;
 while(getline(tempstream, field, ','))//将字符串流sin中的字符读入到field字符串中，以逗号为分隔符
          {
           this->fields.push_back(field); //将刚刚读取的字符串添加到向量fields中
          }
}
//--向CSV文件内写入数据模板
//--data:写入的数据  is_need_endl:是否需要换行
//--is_need_scientific_notation:如果数据特别大，C++默认以科学计数法显示，可以设置该标志以普通形式显示
template<class T>
void CSV::write_to_csv(T data,bool is_need_endl,bool is_need_scientific_notation)
{
//if(data == "")//--处理csv空值问题
// return ;
if(is_need_scientific_notation == false)
this->outfile<<std::fixed<<data;
else 
this->outfile<<data;

if(is_need_endl == true)//--换行
this->outfile<<endl;    


}
//--a,b 分别为需要保存的数据
void CSV::write_csv(int a,int b)
{
 if(this->fields[a] == "")//--处理csv空值问题
 return ;
this->outfile<<this->fields[a]<<","<<this->fields[b]<<endl;
}

void CSV::infile_close(void)
{
this->infile.close();
}
void CSV::outfile_close(void)
{
this->outfile.close();
}


#endif
