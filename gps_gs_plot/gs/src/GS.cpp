#include "../include/gs.h"
#include "../include/csv.h"
double x1,x2,distancexy;
int first_xy = 1;
int main()
{
CSV csv;
GS gs;

cout<<"输出数据格式为：时间，x，y，定位方式,两点间差值"<<endl;

//cout<<"请输入要打开的文件名"<<endl;
//char in_filename[256];
//char out_filename[256];
//cin>>in_filename;
csv.open_csv((char *)"1.csv");//--打开文件

//cout<<"请输入要新建的文件名"<<endl;
//cin>>out_filename;
csv.creat_new_csv((char *)"output/2.csv");//--创建一个输出文件

while(csv.infile.peek()!=EOF){//--判断文件结束//--此处必须用peek进行预判文件是否结尾，使用别的会重复读取最后一行
      //--.csv文件用“,”作为分隔符
    csv.vector_clear();//--清空字符串向量
    csv.read_oneline_of_csv();//--读取一行文件
	//--度分转为度
    gs.dm_to_d(strdup(csv.fields[1].c_str()),gs.latitude_dd);
    gs.dm_to_d(strdup(csv.fields[2].c_str()),gs.longitude_dd);
    //csv.outfile<<gs.longitude_dd<<","<<gs.latitude_dd<<endl;
    //cout<<std::fixed<<strtod(gs.longitude_dd,NULL)<<"  "<<std::fixed<<strtod(gs.latitude_dd,NULL)<<endl;
    gs.LongLat2XY(strtod(gs.longitude_dd,NULL),strtod(gs.latitude_dd,NULL),&gs.X,&gs.Y);
    //--记录第一次xy坐标
    
	if(first_xy)
	 {
       first_xy = 0;
	    x1 = gs.X;
		x2 = gs.Y;
	 }
    distancexy = gs.twoPointDistance(x1,x2,gs.X,gs.Y);

	csv.write_to_csv(csv.fields[0],false,true);//--写入时间
    csv.write_to_csv(',',false,true);//--写入分隔符
    csv.write_to_csv(gs.X,false,false);//--写入经纬度
    csv.write_to_csv(',',false,true);//--写入分隔符
    csv.write_to_csv(gs.Y,false,false);
    csv.write_to_csv(',',false,true);//--写入分隔符
    csv.write_to_csv(csv.fields[3],false,true);//--写入定位方式
	csv.write_to_csv(',',false,true);//--写入分隔符
	csv.write_to_csv(distancexy,true,false);//--写入每个点距离第一个点的差值
    //csv.outfile<<std::fixed<<gs.X<<","<<std::fixed<<gs.Y<<endl;
     }
csv.infile_close();//--关闭所有打开的文件
csv.outfile_close();//--关闭所有打开的文件

return 0;
}
int GS::dm_to_d(char *ddmm, char *dd)
{
	if (NULL == ddmm || NULL == dd)
	{
		return -1;
	}
	int lenSrc = strlen(ddmm)+1;
	int lenMm = 0;
	int flag = 1;
 
	memcpy(dd,ddmm,lenSrc);
	
	char *pcMm;
	double dMm;
	int iMm;
 
	/* 把pcMm定位到小数点位置 */
	pcMm = strstr(dd,".");
 
	if (pcMm == NULL) /* 不含小数点的情况 */
	{
		pcMm = dd+strlen(dd)-2;
		iMm = atoi(pcMm);
		dMm = iMm /60.0;
	}
	else /* 含有小数点的情况 */
	{	
		/* 有度 */
		if (pcMm - dd > 2)
		{
			pcMm = pcMm - 2;
		}
		else /* 没有度,只有分 */
		{
			pcMm = dd;
			flag = 0;
		}
		/* 将字符串转换为浮点数 */
		dMm = atof(pcMm);
		/* 将分转换为度 */
		dMm /= 60.0;
	}
	/* 把转换后的浮点数转换为字符串 */
	sprintf(pcMm,"%lf",dMm);
	if ( flag )
	{
		/* 去掉小数点前面的0 */
		strcpy(pcMm,pcMm+1);
	}
	/* 保留小数点后6位 */
	pcMm = strstr(dd,".");
	lenMm = strlen(pcMm);
	if ( lenMm > (6+2))
	{
		memset(pcMm+6+2,0,lenMm-6-2);
	}
	
	return 1;

}
//--换算精度为0.001M  单位m
int GS::LongLat2XY(double longitude,double latitude,double *X,double *Y)
{
int ProjNo=0; int ZoneWide; ////带宽 
	double longitude1,latitude1, longitude0,latitude0, X0,Y0, xval,yval;
	double a,f, e2,ee, NN, T,C,A, M, iPI;
	iPI = 0.0174532925199433; ////3.1415926535898/180.0;
	ZoneWide = 3;////3度带宽
	//ZoneWide = 6; ////6度带宽 
	//a=6378245.0; f=1.0/298.3; //54年北京坐标系参数 
	////a=6378140.0; f=1/298.257; //80年西安坐标系参数 
	a = 6378137.0; f = 1.0/298.257223563;//WGS84坐标系参数
	//ProjNo = (int)(longitude / ZoneWide) ;      //6度带
	//longitude0 = ProjNo * ZoneWide + ZoneWide / 2; //6度带
	//ProjNo = (int)(longitude / ZoneWide+0.5) ; 
        ProjNo = (int)(longitude / ZoneWide) ; //--带号
	longitude0 = ProjNo * ZoneWide ; //--中央子午线
	longitude0 = longitude0 * iPI ;//--中央子午线转化未弧度
	latitude0=0; 
	longitude1 = longitude * iPI ; //经度转换为弧度
	latitude1 = latitude * iPI ; //纬度转换为弧度
	e2=2*f-f*f;
	ee=e2*(1.0-e2);
	NN=a/sqrt(1.0-e2*sin(latitude1)*sin(latitude1));
	T=tan(latitude1)*tan(latitude1);
	C=ee*cos(latitude1)*cos(latitude1);
	A=(longitude1-longitude0)*cos(latitude1); 	

	M=a*((1-e2/4-3*e2*e2/64-5*e2*e2*e2/256)*latitude1-(3*e2/8+3*e2*e2/32+45*e2*e2
		*e2/1024)*sin(2*latitude1)
		+(15*e2*e2/256+45*e2*e2*e2/1024)*sin(4*latitude1)-(35*e2*e2*e2/3072)*sin(6*latitude1));
	xval = NN*(A+(1-T+C)*A*A*A/6+(5-18*T+T*T+72*C-58*ee)*A*A*A*A*A/120);
	yval = M+NN*tan(latitude1)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
		+(61-58*T+T*T+600*C-330*ee)*A*A*A*A*A*A/720);
	//X0 = 1000000L*(ProjNo+1)+500000L; //6度带
	X0 = 1000000L*ProjNo+500000L;  //3度带
	Y0 = 0; 
	xval = xval+X0; yval = yval+Y0; 

	*X= xval;
	*Y= yval;
        //printf("%lf   %lf\r\n",xval,yval);
	return 1;

}

int GS::XY2LongLat(double X,double Y,double* longitude,double* latitude)
{
	int ProjNo; int ZoneWide; ////带宽 
	double longitude1,latitude1, longitude0,latitude0, X0,Y0, xval,yval;
	double e1,e2,f,a, ee, NN, T,C, M, D,R,u,fai, iPI;
	iPI = 0.0174532925199433; ////3.1415926535898/180.0; 
	///a = 6378245.0; f = 1.0/298.3; //54年北京坐标系参数 
	////a=6378140.0; f=1/298.257; //80年西安坐标系参数
	a = 6378137.0; f = 1.0/298.257223563;//WGS84坐标系参数
	ProjNo = (int)(X/1000000L) ; //查找带号
	// 	ZoneWide = 6; ////6度带宽 
	// 	longitude0 = (ProjNo-1) * ZoneWide + ZoneWide / 2; //计算每带中央子午线经度
	ZoneWide=3;   ////3度带宽
	longitude0 =  ProjNo * ZoneWide;
	longitude0 = longitude0 * iPI ; //中央经线

	X0 = ProjNo*1000000L+500000L; 
	Y0 = 0; 
	xval = X-X0; yval = Y-Y0; //带内大地坐标
	e2 = 2*f-f*f;
	e1 = (1.0-sqrt(1-e2))/(1.0+sqrt(1-e2));
	ee = e2/(1-e2);
	M = yval;
	u = M/(a*(1-e2/4-3*e2*e2/64-5*e2*e2*e2/256));
	fai = u+(3*e1/2-27*e1*e1*e1/32)*sin(2*u)+(21*e1*e1/16-55*e1*e1*e1*e1/32)*sin(4*u)
		+(151*e1*e1*e1/96)*sin(6*u)+(1097*e1*e1*e1*e1/512)*sin(8*u);
	C = ee*cos(fai)*cos(fai);
	T = tan(fai)*tan(fai);
	NN = a/sqrt(1.0-e2*sin(fai)*sin(fai));// 字串1 
	R = a*(1-e2)/sqrt((1-e2*sin(fai)*sin(fai))*(1-e2*sin(fai)*sin(fai))*(1-e2*sin(fai)*sin(fai)));
	D = xval/NN;
	//计算经度(Longitude) 纬度(Latitude)
	longitude1 = longitude0+(D-(1+2*T+C)*D*D*D/6+(5-2*C+28*T-3*C*C+8*ee+24*T*T)*D*D*D*D*D/120)/cos(fai);
	latitude1 = fai -(NN*tan(fai)/R)*(D*D/2-(5+3*T+10*C-4*C*C-9*ee)*D*D*D*D/24+(61+90*T+298*C+45*T*T-256*ee-3*C*C)*D*D*D*D*D*D/720); 
	int g=0;
	//转换为度 DD
	*longitude = longitude1 / iPI; 
	*latitude = latitude1 / iPI;

	return 1;
}

