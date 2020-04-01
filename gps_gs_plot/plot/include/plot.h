/******************************************
 * 版权：小鱼儿飞丫飞
 * 作者：小鱼儿飞丫飞
 * 日期：2020-3-24
 * 描述：基于opencv的C++打点画图程序
 * ***************************************/
#ifndef __PLOT_H__
#define __PLOT_H__

#include <opencv2/opencv.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include <unistd.h> 
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define data_size 0.30//--坐标点尺寸
#define text_size 0.45//--文字尺寸
using namespace cv;
using namespace std;
//--线类型
struct LineType
{
 char type;  //--点类型
 bool is_need_lined;//--两点之间是否需要连线
 Scalar color;//--点颜色
}; 
//--打点画图类
class CPlot
{
public:
     void DrawAxis (Mat image);//--画坐标轴
     void DrawData (Mat image);//--画点
public:
     int window_height;
     int window_width;
                
      vector<vector<Point2d>>dataset;//--点集合(二维数组)
      vector<LineType>linetypeset;//--线类型集合
  
      Scalar backgroud_color;//--图片背景色
      Scalar axis_color;//--坐标轴颜色
      Scalar text_color;//--标签颜色
      Scalar border_color;//--边框颜色
      Mat Figure;//--图片
     //--画图范围
      double y_max;//--y轴最大值
      double y_min;//--y轴最小值
      double y_scale;//--y轴刻度值，两个刻度之间间隔了几个像素点

      double x_max;//--x轴最大值
      double x_min;//--x轴最小值
      double x_scale;//--x轴刻度值

      int border_size;//--边框宽度，两个刻度之间间隔了几个像素点

public:
      //--打点模板
      template<class T>
      void plot(T *y,size_t Cnt,Scalar color,char type = '*',bool is_need_lined = true);//--一组数据打点
      template<typename T>
      void plot(T* x,T* y,size_t Cnt,Scalar color,char type = '*',bool is_need_lined = true);//--一组数据打点
	  template<typename T>
      void plot(T X, T Y, Scalar color, char type,bool is_need_lined);//--单个数据打点

      void xlabel(string xlabel_name,Scalar label_color);//--x轴添加标签
      void ylabel(string ylabel_name,Scalar label_color);//--y轴添加标签
       
      void clear();//--清空图片数据
      void title(string title_name,Scalar title_color);//--给图片添加标题
public:
      CPlot(double window_height,double window_width,int border_size,Scalar backgroud_color,Scalar border_color,Scalar axis_color,Scalar text_color);//--构造函数
      ~CPlot();//--析构函数 

};

//下面所有的函数采用范型设计，因此将实现部分和声明部分放在一个文件中


//--构造函数
/*
*功能：打点画图工作初始化
*参数：window_height：画图区域高度（像素点个数）
*     window_width:画图区域宽度
*     border_size：画图区域边框宽度
*	  backgroud_color：画图区域背景色
*     border_color:边框颜色
*	  axis_color：坐标轴颜色
*	  text_color：坐标刻度数颜色
*返回值：无
*/
CPlot::CPlot(double window_height,double window_width,int border_size,Scalar backgroud_color,Scalar border_color,Scalar axis_color,Scalar text_color)
{
	this->border_size = border_size; //图外围边界
	this->window_height = window_height;//--窗口大小
	this->window_width = window_width;
    this->backgroud_color = backgroud_color; //背景颜色
	this->border_color = border_color;//--边框颜色
	this->axis_color = axis_color;//坐标轴颜色
	this->text_color = text_color; //文字红色
	this->x_min = 0;
	this->x_max = 0;
	this->y_min = 0;
	this->y_max = 0;

	this->Figure = Mat(this->window_height, //--图片高度
                           this->window_width,//--图片宽度
                           CV_8UC3, //--3通道
                           this->border_color//--整个图片颜色为边框色
                           );
	//--修改打印区域像素颜色					   
	for(int i=this->border_size;i<(this->window_height-this->border_size);i++)
	  {
        for(int j=this->border_size;j<(this->window_width-this->border_size);j++)
		    {
            this->Figure.at<Vec3b>(i,j)[0] = this->backgroud_color[0];
			this->Figure.at<Vec3b>(i,j)[1] = this->backgroud_color[1];
			this->Figure.at<Vec3b>(i,j)[2] = this->backgroud_color[2];
			}
		   
	  }
}
//--析构函数
/*
*功能：释放内存
*参数：无
*返回值：无
*/
CPlot::~CPlot()
{
	this->clear();//--删除容器元素
	this->Figure.release();//--释放图片
}
/*
*功能：释放内存
*参数：无
*返回值：无
*/
void CPlot::clear()//--
{ //--clear():从容器中删除所有元素
	this->dataset.clear();
	this->linetypeset.clear();
}
/******************************************************
 * 功能：将传入的坐标点加入到坐标链表里面，并添加对应点的颜色数据到线类型链表里面
 *      然后调用打点程序依次打点
 * 参数：x：x轴坐标数据
 *      y:y轴坐标数据
 *      Cnt:坐标点个数
 *      coler： 打点的颜色
 *      type：点形状：
 *                 l   直线	
 *                 *   星 
 *                 .   点 
 *                 o   圈 
 *                 x   叉 
 *                 +   十字 
 *                 s   方块 
 *      is_need_lined：点与点之间是否需要连线：true or false
 * 
 * 
 ********************************************************/
//范型设计
template<typename T>
void CPlot::plot(T *X, T *Y, size_t Cnt, Scalar color, char type,bool is_need_lined)
{
	//对数据进行存储
	T tempX, tempY;
	vector<Point2d>data;
	for(int i = 0; i < Cnt;i++)
	{
		tempX = X[i];
		tempY = Y[i];
		data.push_back( Point2d((double)tempX, (double)tempY) );
	}
	this->dataset.push_back(data);
	LineType LT;
	LT.type = type;
	LT.color = color;
	LT.is_need_lined = is_need_lined;
	this->linetypeset.push_back(LT);
	
	this->DrawData(this->Figure); //每次都是重新绘制
}
/******************************************************
 * 功能：将传入的坐标点加入到坐标链表里面，并添加对应点的颜色数据到线类型链表里面
 *      然后调用打点程序依次打点
 * 参数：x：无X轴数据传入，默认从0开始向坐标轴正方向+1
 *      y:y轴坐标数据
 *      Cnt:坐标点个数
 *      coler： 打点的颜色
 *      type：点形状：
 *                 l   直线	
 *                 *   星 
 *                 .   点 
 *                 o   圈 
 *                 x   叉 
 *                 +   十字 
 *                 s   方块 
 *      is_need_lined：点与点之间是否需要连线：true or false
 * 
 * 
 ********************************************************/
template<class T>
void CPlot::plot(T *Y, size_t Cnt, Scalar color, char type,bool is_need_lined)
{
	//对数据进行存储
	T tempX, tempY;
	vector<Point2d>data;
	for(int i = 0; i < Cnt;i++)
	{
		tempX = i;
		tempY = Y[i];
		data.push_back(Point2d((double)tempX, (double)tempY));
	}
	this->dataset.push_back(data);
	LineType LT;
	LT.type = type;
	LT.color = color;
	LT.is_need_lined = is_need_lined;
	this->linetypeset.push_back(LT);
	this->DrawData(this->Figure);
}
/******************************************************
 * 功能：传入一个坐标就打一个点，并且把改点进行存储
 * 参数：x：x轴坐标数据
 *      y:y轴坐标数据
 *      coler： 打点的颜色
 *      type：点形状：
 *                 l   直线	
 *                 *   星 
 *                 .   点 
 *                 o   圈 
 *                 x   叉 
 *                 +   十字 
 *                 s   方块 
 *      is_need_lined：点与点之间是否需要连线：true or false
 * 
 * 
 ********************************************************/
//范型设计
template<typename T>
void CPlot::plot(T X, T Y, Scalar color, char type,bool is_need_lined)
{
	//对数据进行存储
	T tempX, tempY;
	vector<Point2d>data;

		tempX = X;
		tempY = Y;
		data.push_back( Point2d((double)tempX, (double)tempY) );

	this->dataset.push_back(data);
	LineType LT;
	LT.type = type;
	LT.color = color;
	LT.is_need_lined = is_need_lined;
	this->linetypeset.push_back(LT);
	
	this->DrawData(this->Figure); //每次都是重新绘制
}
/*
*功能：显示图片标题
*参数：title_name：标题名字
*     title_color：标题颜色
*返回值：无
*/
void CPlot::title(string title_name,Scalar title_color = Scalar(0,0,0))
{
	Mat image = this->Figure;
	int baseline = 0;//--文字最底部y坐标,
	Size textSize = getTextSize(title_name,FONT_HERSHEY_COMPLEX,text_size,1,&baseline);//--获取文字的长和宽像素值
	int x = (this->window_width - 2 * this->border_size ) / 2 + this->border_size - textSize.width/2.0; 
	int y = this->border_size / 2;
        putText (image,
                 title_name.c_str(),
                 Point( x, y),
		 FONT_HERSHEY_COMPLEX,//--字体类型
		 text_size,//--字体缩放因子
		 title_color,//--颜色
		 1,//--线条粗细
		 LINE_8//--线条类型
	);		

}
/*
*功能：显示X轴标签
*参数：xlabel_name：标签名字
*     label_color：标签颜色
*返回值：无
*/
void CPlot::xlabel(string xlabel_name, Scalar label_color = Scalar(0,0,0))
{
	//--允许xy轴穿过0点
	double y_ref = this->y_min;
	if ( (this->y_max > 0 ) && ( this->y_min <= 0 ) )
	{
		y_ref = 0;
	}//--round()四舍五入函数

	int baseline = 0;//--文字最底部y坐标,
	Size textSize = getTextSize(xlabel_name,FONT_HERSHEY_COMPLEX,text_size,1,&baseline);//--获取文字的长和宽像素值

	int x_axis_pos = this->window_height - this->border_size - round((y_ref - this->y_min) * this->y_scale);
	int x = this->window_width - this->border_size - textSize.width;
	int y = x_axis_pos + 25;
	 putText (this->Figure,
                 xlabel_name.c_str(),
                 Point( x, y),
		 FONT_HERSHEY_COMPLEX,//--字体类型
		 text_size,//--字体缩放因子
		 label_color,//--颜色
		 1,//--线条粗细
		 LINE_8//--线条类型
	);		
}
/*
*功能：显示y轴标签
*参数：ylabel_name：标签名字
*     label_color：标签颜色
*返回值：无
*/
void CPlot::ylabel(string ylabel_name, Scalar label_color = Scalar(0,0,0))
{

	int x = this->border_size;
	int y = this->border_size-5;
        putText (this->Figure,
                 ylabel_name.c_str(),
                 Point( x, y),
		 FONT_HERSHEY_COMPLEX,//--字体类型
		 text_size,//--字体缩放因子
		 label_color,//--颜色
		 1,//--线条粗细
		 LINE_8//--线条类型
	);		
}
//--画坐标轴
void CPlot::DrawAxis (Mat image)
{

	//Scalar axis_color = this->axis_color;
	
	int bs = this->border_size;		
	int h = this->window_height;
	int w = this->window_width;

	// 坐标系尺寸
	int gh = h - bs * 2;
	int gw = w - bs * 2;

	// 画水平和竖直轴
	// 如何可能的画，轴可以穿过0点
	double y_ref = this->y_min;//--保存Y轴0点或者最小点（如果Y轴有负半轴的话）
	if ( (this->y_max > 0 ) && ( this->y_min <= 0 ) )//--如果y轴数据越过0点，
	{
		y_ref = 0;//---则y轴参考点为0
	}
       //--计算X轴水平位置，也是y轴0点位置纵坐标
	int x_axis_pos = h - bs - round((y_ref - this->y_min) * this->y_scale);
	//画X 轴
	line(image, 
             Point(bs,x_axis_pos), 		          
             Point(w - bs, x_axis_pos),
            this->axis_color);
	//画Y 轴
	line(image, 
             Point(bs, h - bs), 
             Point(bs, h - bs - gh),
             this->axis_color);


	int chw = 15, chh = 10; 
	char text[16];

	// 显示y轴最大值
	if ( (this->y_max - y_ref) > 0.05 * (this->y_max - this->y_min) )
	{
		snprintf(text, sizeof(text)-1, "%.3f", this->y_max);

	    int baseline = 0;//--文字最底部y坐标,
	    Size textSize = getTextSize(text,FONT_HERSHEY_COMPLEX,data_size,1,&baseline);//--获取文字的长和宽像素值

        putText (image,
                 text,
                 Point(bs-textSize.width, bs-textSize.height),
		 FONT_HERSHEY_COMPLEX,//--字体类型
		 data_size,//--字体缩放因子
		 this->text_color,//--颜色
		 1,//--线条粗细
		 LINE_8//--线条类型
	);		
	}
	// y min
	if ( (y_ref - this->y_min) > 0.05 * (this->y_max - this->y_min) )
	{
		snprintf(text, sizeof(text)-1, "%.3f", this->y_min);
		int baseline = 0;//--文字最底部y坐标,
	    Size textSize = getTextSize(text,FONT_HERSHEY_COMPLEX,data_size,1,&baseline);//--获取文字的长和宽像素值


        putText (image,
                 text,
                 Point(bs-textSize.width, h - bs +textSize.height+5),
		 FONT_HERSHEY_COMPLEX,//--字体类型
		 data_size,//--字体缩放因子
		 this->text_color,//--颜色
		 1,//--线条粗细
		 LINE_8//--线条类型
	);

	}

	//画Y轴的刻度 每隔 scale_pixes 个像素
	//Y正半轴
	double y_scale_pixes = chh * 4;
	for (int i = 0; i < floor( (x_axis_pos - bs) / y_scale_pixes ) + 1; i++)//--ceil 向上取整 floor 向下取整
	{
		snprintf(text, sizeof(text)-1, "%.3f", this->y_min+i * y_scale_pixes / this->y_scale );

		int baseline = 0;//--文字最底部y坐标,
	    Size textSize = getTextSize(text,FONT_HERSHEY_COMPLEX,data_size,1,&baseline);//--获取文字的长和宽像素值

        putText (image,
                 text,
                 Point(bs-textSize.width, x_axis_pos - i * y_scale_pixes+textSize.height/2),
		 FONT_HERSHEY_COMPLEX,//--字体类型
		 data_size,//--字体缩放因子
		 this->axis_color,//--颜色
		 1,//--线条粗细
		 LINE_8//--线条类型
	);
      line(image, Point(bs, x_axis_pos - i * y_scale_pixes), Point(bs+10, x_axis_pos - i * y_scale_pixes), this->axis_color, 1, LINE_AA);//--画刻度线
	}
	//Y负半轴
	for (int i = 1; i < floor(( h - x_axis_pos - bs ) / y_scale_pixes ) + 1; i++)
	{
		snprintf(text, sizeof(text)-1, "%.3f", -i * y_scale_pixes / this->y_scale );

		int baseline = 0;//--文字最底部y坐标,
	    Size textSize = getTextSize(text,FONT_HERSHEY_COMPLEX,data_size,1,&baseline);//--获取文字的长和宽像素值

        putText (image,
                 text,
                 Point(bs-textSize.width, x_axis_pos + i * y_scale_pixes + textSize.height/2),
		 FONT_HERSHEY_COMPLEX,//--字体类型
		 data_size,//--字体缩放因子
		 this->axis_color,//--颜色
		 1,//--线条粗细
		 LINE_8//--线条类型
	);
      line(image, Point(bs, x_axis_pos + i * y_scale_pixes), Point(bs+10, x_axis_pos + i * y_scale_pixes), this->axis_color, 1, LINE_AA);//--画刻度线

	}

	// x_max
	snprintf(text, sizeof(text)-1, "%.3f", this->x_max );

	int baseline_xmax = 0;//--文字最底部y坐标,
	Size textSize_xmax = getTextSize(text,FONT_HERSHEY_COMPLEX,data_size,1,&baseline_xmax);//--获取文字的长和宽像素值

    putText (image,
            text,
            Point(w - bs - textSize_xmax.width/2, x_axis_pos-10),
		 FONT_HERSHEY_COMPLEX,//--字体类型
		 data_size,//--字体缩放因子
		 this->text_color,//--颜色
		 1,//--线条粗细
		 LINE_8//--线条类型
	);

	// x min
	snprintf(text, sizeof(text)-1, "%.3f", this->x_min );

	int baseline_xmin = 0;//--文字最底部y坐标,
	Size textSize_xmin = getTextSize(text,FONT_HERSHEY_COMPLEX,data_size,1,&baseline_xmin);//--获取文字的长和宽像素值

        putText (image,
                 text,
                 Point(bs+5, x_axis_pos-10),
		 FONT_HERSHEY_COMPLEX,//--字体类型
		 data_size,//--字体缩放因子
		 this->text_color,//--颜色
		 1,//--线条粗细
		 LINE_8//--线条类型
	);

	//画X轴的刻度 每隔 scale_pixes 个像素
	//--根据X轴最大的数的长度来自动决定刻度宽度
	 snprintf(text, sizeof(text)-1, "%.3f", this->x_max);
	 int baseline = 0;//--文字最底部y坐标,
	 Size textSize = getTextSize(text,FONT_HERSHEY_COMPLEX,data_size,1,&baseline);//--获取文字的长和宽像素值
	 double x_scale_pixes = textSize.width+10;

	for (int i = 1; i < floor( gw / x_scale_pixes ) + 1; i++)
	{
		snprintf(text, sizeof(text)-1, "%.3f", this->x_min + i * x_scale_pixes / this->x_scale );

	    int baseline = 0;//--文字最底部y坐标,
	    Size textSize = getTextSize(text,FONT_HERSHEY_COMPLEX,data_size,1,&baseline);//--获取文字的长和宽像素值
        
        putText (image,
                 text,
                 Point(bs + i * x_scale_pixes - textSize.width/2, x_axis_pos + textSize.height+5),
		 FONT_HERSHEY_COMPLEX,//--字体类型
		 data_size,//--字体缩放因子
		 this->axis_color,//--颜色
		 1,//--线条粗细
		 LINE_8//--线条类型
	);
 line(image, Point(bs + i * x_scale_pixes ,x_axis_pos), Point(bs + i * x_scale_pixes ,x_axis_pos-10), this->axis_color, 1, LINE_AA);//--画刻度线

	}

}
//添加对线型的支持
//TODO线型未补充完整
//标记		线型
//l          直线	
//*          星 
//.          点 
//o          圈 
//x          叉 
//+          十字 
//s          方块 
void CPlot::DrawData (Mat image)
{
	
	//this->x_min = this->x_max = this->dataset[0][0].x;
	//this->y_min = this->y_max = this->dataset[0][0].y;
	

//--找出最大值和最小值
	for(size_t i = 0; i < this->dataset.size(); i++)//--遍历所有行
	{
		for(size_t j = 0; j < this->dataset[i].size(); j++)//--遍历每行所有列
		{
			if(this->dataset[i][j].x < this->x_min)
			{
				this->x_min = this->dataset[i][j].x;
			}else if(this->dataset[i][j].x > this->x_max)
			{
				this->x_max = this->dataset[i][j].x;
			}
		
			if(this->dataset[i][j].y < this->y_min)
			{
				this->y_min = this->dataset[i][j].y;
			}else if(this->dataset[i][j].y > this->y_max)
			{
				this->y_max = this->dataset[i][j].y;
			}
		}
	}

//--根据最大值自动调整边框的宽度

char text[16];
int x_max_temp,y_max_temp;
snprintf(text, sizeof(text)-1, "%.3f", this->x_max);
int baseline = 0;//--文字最底部y坐标,
Size textSize = getTextSize(text,FONT_HERSHEY_COMPLEX,data_size,1,&baseline);//--获取文字的长和宽像素值
 x_max_temp = textSize.width/2;//--X轴刻度只有一般长度在边框里面
snprintf(text, sizeof(text)-1, "%.3f", this->y_max);
baseline = 0;//--文字最底部y坐标,
textSize = getTextSize(text,FONT_HERSHEY_COMPLEX,data_size,1,&baseline);//--获取文字的长和宽像素值
 y_max_temp = textSize.width;
if( x_max_temp>=y_max_temp)
   this->border_size = x_max_temp+10;
else
   this->border_size = y_max_temp+10;
	int bs = this->border_size;

//--计算x轴和y轴最大值和最小值范围 并计算出刻度大小，两点之间的像素个数
	double x_range = this->x_max - this->x_min;
	double y_range = this->y_max - this->y_min;
	this->x_scale = (image.cols - bs*2)/ x_range;//--cols图片宽度
	this->y_scale = (image.rows- bs*2)/ y_range;//--rows 图片高度
	
	
	//清屏,设置屏幕颜色为灰色
	//memset(image.data, 105, sizeof(unsigned char)*Figure.step*Figure.rows);
		for(int i=0;i<this->window_height;i++)
	  {
        for(int j=0;j<this->window_width;j++)
		    {
            this->Figure.at<Vec3b>(i,j)[0] = this->border_color[0];
			this->Figure.at<Vec3b>(i,j)[1] = this->border_color[1];
			this->Figure.at<Vec3b>(i,j)[2] = this->border_color[2];
			}
		   
	  }
	//--修改打印区域像素颜色					   
	for(int i=this->border_size;i<(this->window_height-this->border_size);i++)
	  {
        for(int j=this->border_size;j<(this->window_width-this->border_size);j++)
		    {
            this->Figure.at<Vec3b>(i,j)[0] = this->backgroud_color[0];
			this->Figure.at<Vec3b>(i,j)[1] = this->backgroud_color[1];
			this->Figure.at<Vec3b>(i,j)[2] = this->backgroud_color[2];
			}
		   
	  }
	this->DrawAxis(image);
	
	//printf("x_range: %f y_range: %f\n", x_range, y_range);
	//绘制点
	double tempX, tempY;
	Point prev_point, current_point;
	int radius = 3;
	int slope_radius = (int)( radius * 1.414 / 2 + 0.5);
	for(size_t i = 0; i < this->dataset.size(); i++)//-遍历每一行数据
	{
		for(size_t j = 0; j < this->dataset[i].size(); j++)//--遍历当前行所有列数据
		{
			tempX = (int)((this->dataset[i][j].x - this->x_min)*this->x_scale);//--计算出改点对应图片上坐标点
			tempY = (int)((this->dataset[i][j].y - this->y_min)*this->y_scale);
			current_point = Point(bs + tempX, image.rows - (tempY + bs));//--考虑边框尺寸，获取实际的坐标点
			
			if(this->linetypeset[i].type == 'l')
			{
				// draw a line between two points
				if (j >= 1)
				{
					line(image, prev_point, current_point, linetypeset[i].color, 1, LINE_AA);
				}		
				prev_point = current_point;
			}else if(this->linetypeset[i].type == '.')
			{
				circle(image, current_point, 1, linetypeset[i].color, -1, 8);
				if (linetypeset[i].is_need_lined == true)
				{
					if (j >= 1)
					{
						line(image, prev_point, current_point, linetypeset[i].color, 1, LINE_AA);
					}		
					prev_point = current_point;
				}
			}else if(this->linetypeset[i].type == '*')
			{
				//画*
				line(image, Point(current_point.x - slope_radius, current_point.y - slope_radius), 
			    Point(current_point.x + slope_radius, current_point.y + slope_radius), linetypeset[i].color, 1, 8);
					   
				line(image, Point(current_point.x - slope_radius, current_point.y + slope_radius), 
			    Point(current_point.x + slope_radius, current_point.y - slope_radius), linetypeset[i].color, 1, 8);

				line(image, Point(current_point.x - radius, current_point.y), 
				Point(current_point.x + radius, current_point.y), linetypeset[i].color, 1, 8);
					   
				line(image,Point(current_point.x, current_point.y - radius), 
			    Point(current_point.x, current_point.y + radius), linetypeset[i].color, 1, 8);	 
				if (linetypeset[i].is_need_lined == true)
				{
					if (j >= 1)
					{
						line(image, prev_point, current_point, linetypeset[i].color, 1, LINE_AA);
					}		
					prev_point = current_point;
				}
				
			}else if(this->linetypeset[i].type == 'o')
			{
				circle(image, current_point, radius, this->text_color, 1, LINE_AA);
				if (linetypeset[i].is_need_lined == true)
				{
					if (j >= 1)
					{
						line(image, prev_point, current_point, linetypeset[i].color, 1, LINE_AA);
					}		
					prev_point = current_point;
				}
			}else if(this->linetypeset[i].type == 'x')
			{
				line(image, Point(current_point.x - slope_radius, current_point.y - slope_radius), 
			    Point(current_point.x + slope_radius, current_point.y + slope_radius), linetypeset[i].color, 1, 8);
					   
				line(image, Point(current_point.x - slope_radius, current_point.y + slope_radius), 
			    Point(current_point.x + slope_radius, current_point.y - slope_radius), linetypeset[i].color, 1, 8);
				if (linetypeset[i].is_need_lined == true)
				{
					if (j >= 1)
					{
						line(image, prev_point, current_point, linetypeset[i].color, 1, LINE_AA);
					}		
					prev_point = current_point;
				}
			}else if(this->linetypeset[i].type == '+')
			{
				line(image, Point(current_point.x - radius, current_point.y), 
				Point(current_point.x + radius, current_point.y), linetypeset[i].color, 1, 8);
					   
				line(image, Point(current_point.x, current_point.y - radius), 
			    Point(current_point.x, current_point.y + radius), linetypeset[i].color, 1, 8);
				if (linetypeset[i].is_need_lined == true)
				{
					if (j >= 1)
					{
						line(image, prev_point, current_point, linetypeset[i].color, 1, LINE_AA);
					}		
					prev_point = current_point;
				}
			}else if(this->linetypeset[i].type == 's')
			{
				rectangle(image, Point(current_point.x - slope_radius, current_point.y - slope_radius), 
			    Point(current_point.x + slope_radius, current_point.y + slope_radius), linetypeset[i].color, 1, 8);
				if (linetypeset[i].is_need_lined == true)
				{
					if (j >= 1)
					{
						line(image, prev_point, current_point, linetypeset[i].color, 1, LINE_AA);
					}		
					prev_point = current_point;
				}
			}
			//--如果想动态显示，可以吧下面两句代码打开
             //imshow("xiaoyu", this->Figure);
            // waitKey(1);
		}
	}	
}

#endif

