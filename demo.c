#include <stdio.h>
#include <math.h>


#define PI 3.1415926535897
static const double EARTH_RADIUS = 6371.004;//6378.137;	//6371.004




double rad(double d)
{
	return d * PI / 180.0;
}

 //[118.77147503233,32.054128923368],[116.3521416286,39.965780080447]
double GetDistance(double lat1, double lng1, double lat2, double lng2)
{
	double radLat1 = (double)rad(lat1);
	double radLat2 = rad(lat2);
	double a = radLat1 - radLat2;
	double b = rad(lng1) - rad(lng2);
	double s = 2 * asin(sqrt(pow(sin(a/2),2) +cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
	//
	s = s * EARTH_RADIUS;
	s = round(s * 10000000) / 10000;
	return s;
} 



int main(char argc ,char **argv)
{

	//118.77147503233,32.054128923368;116.3521416286,39.965780080447;116.28215586757,39.965780080447
	double s;
	s = GetDistance(32.054128923368,118.77147503233,39.965780080447,116.3521416286);
	printf("distance = %lf\n",s);
	s = GetDistance(39.965780080447,116.3521416286,39.965780080447,116.28215586757);
	printf("distance = %lf\n",s);
	//[22.12121,113.1212], [22.59612,113.8413]
	s = GetDistance(22.12121,113.1212,22.59612,113.8413);
	printf("distance = %lf\n",s);
	
}
