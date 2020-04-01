#include "../include/plot.h"
#include "../include/csv.h"

int main(int argc,char ** argv)
{
int Cnt = 0;
vector<double> x;
vector<double> y;
//--初始化图片
CPlot plot(800,800,80,Scalar(0,0,0),Scalar(205,116,24),Scalar(255,255,255),Scalar(255,255,255));
CSV csv;


cout<<"开始打印坐标点"<<endl;

//cout<<"请输入要打开的文件名"<<endl;
//char in_filename[256];
//cin>>in_filename;
csv.open_csv("2.csv");//--打开文件


while(csv.infile.peek()!=EOF){//--判断文件结束//--此处必须用peek进行预判文件是否结尾，使用别的会重复读取最后一行
      //--.csv文件用“,”作为分隔符
    csv.vector_clear();//--清空字符串向量
    csv.read_oneline_of_csv();//--读取一行文件
   x.push_back(strtod(csv.fields[0].c_str(),NULL));//--c_str():将char*转为string。strtod（）：将string转为double
   y.push_back(strtod(csv.fields[4].c_str(),NULL));
   
   ++Cnt;
     }
  csv.infile.close();//--关闭所有打开的文件

        namedWindow("xiaoyu",1);//--创建一个窗口
	plot.x_max = x[0]; //可以设定横纵坐标的最大，最小值
	plot.x_min = x[0];
	plot.y_max = y[0];
	plot.y_min = y[0];
	//plot.axis_color = Scalar(0,255,0);
	//plot.text_color = Scalar(255,0,255);

	plot.plot(&x[0],&y[0], Cnt, Scalar(0, 0, 255),'s'); //可以只传入Y值 X默认从0开始 //--vector转数组 只需要传入首位数据地址就可以
	plot.title("xiaoyuer"); //可以设定标题 只能是英文 中文会乱码 有解决方案，但是很麻烦
	plot.xlabel("this X",Scalar(255,255,0));
	plot.ylabel("this Y",Scalar(255,255,0));
	imshow("xiaoyu", plot.Figure);
	waitKey(0);
    imwrite("./picture/1.jpg",plot.Figure);


	plot.clear();//清出前面的存储的数据

return 0;
}

