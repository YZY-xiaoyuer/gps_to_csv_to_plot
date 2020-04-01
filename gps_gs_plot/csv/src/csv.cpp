#include "../include/csv.h"
float second=0,minute=0,hour=0;

int main()
{
CSV csv;
cout<<"输出数据格式为：时间，维度，经度，定位方式"<<endl;
cout<<"请输入要打开的文件名"<<endl;

char in_filename[256];
char out_filename[256];
cin>>in_filename;
csv.open_csv(in_filename);//--打开文件

//cout<<"请输入要新建的文件名"<<endl;
csv.creat_new_csv((char *)"output/1.csv");//--创建一个输出文件

while(csv.infile.peek()!=EOF){//--判断文件结束//--此处必须用peek进行预判文件是否结尾，使用别的会重复读取最后一行
      //--.csv文件用“,”作为分隔符
    csv.vector_clear();//--清空字符串向量
    csv.read_oneline_of_csv();//--读取一行文件
  //--计算时间
  second+=0.1;
  //if(second == 60){minute+=1;second=0;}
  //if(minute == 60){hour+=1;minute=0;}
  //-----判断有效性
   if(csv.fields[0] == "$GNRMC")//--包头正确
     {
         if(csv.fields[2] == "A")//--有效定位
           {
             if((csv.fields[12] == "D")||(csv.fields[12] == "F")||(csv.fields[12] == "R"))//--差分定位
               { 
                 csv.write_to_csv(second,false,true);//--写入时间
                 csv.write_to_csv(',',false,true);//--写入分隔符
                 csv.write_to_csv(csv.fields[3],false,true);//--写入经纬度
                 csv.write_to_csv(',',false,true);//--写入分隔符
                 csv.write_to_csv(csv.fields[5],false,true);
                 csv.write_to_csv(',',false,true);//--写入分隔符
                 csv.write_to_csv(csv.fields[12],true,true);//--写入定位方式
                }
           }
         
      }
 
    
     }
csv.infile_close();//--关闭所有打开的文件
csv.outfile_close();//--关闭所有打开的文件
return 0;
}

