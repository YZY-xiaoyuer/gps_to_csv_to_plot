#ifndef __GS_H__
#define __GS_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <cstring>
#include <cmath>
#include <cstdlib>

class GS{

public:
       char  latitude_dd[64];//--维度
       char  longitude_dd[64];//--经度
       double X,Y;
 
public:
int LongLat2XY(double longitude,double latitude,double *X,double *Y);//--高斯投影正算
int XY2LongLat(double X,double Y,double* longitude,double* latitude);//--高斯投影反算
int dm_to_d(char *ddmm, char *dd);//--度分格式转度
double twoPointDistance(double x1 ,double y1 ,double x2 ,double y2 );

};


double GS::twoPointDistance(double x1 ,double y1 , double x2 , double y2 )
{
	return  pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
}

#endif
