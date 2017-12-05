#include <iostream>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <iomanip>
using namespace std;
#define pi 3.141592
int nmea0183_checksum(char *nmea_data)
{
	int crc = 0;
	int i;

	// the first $ sign and the last two bytes of original CRC + the * sign
	for (i = 1; i < strlen(nmea_data) - 3; i++) {
		crc ^= nmea_data[i];
	}

	return crc;
}
double gps_course_to(double lat1, double long1, double lat2, double long2);
double gps_distance_between(double lat1, double long1, double lat2, double long2);
double latitude(double lat1, double hujne, double heading);
double longtitude(double lon1, double lat1, double heading, double hujne, double templat);
int main(void)
{
	double lat1 = 54.91173263;
	double lon1 = 9.77745652;
	double lat2 = 54.91171413;
	double lon2 = 9.78273511;
	double lat3 = 54.91511219;
	double lon3 = 9.78243470;
	double lat4 = 54.91516770;
	double lon4 = 9.77731705;

	/*
	char *huj;
	int faget = nmea0183_checksum("$GPGGA,013827.428,5454.740,N,00946.813,E,1,12,1.0,0.0,M,0.0,M,,*6f");
	cout <<std::hex<< faget<<endl;*/
	double heading = gps_course_to(lat1, lon1, lat2, lon2);
	int hujne = 40;
	double templat=0.00, templon=0.00;
	int blet = 0;
	ofstream myfile;
	myfile.open("lat.txt");
	myfile.clear();

	myfile << std::setprecision(15);
	myfile << std::setprecision(15) << lat1 << "," << lon1 << endl;
	while (gps_distance_between((templat * 180) / pi, (templon * 180) / pi, lat2, lon2)>40) {
		 templat = latitude(lat1,hujne,heading);
		 templon = longtitude(lon1, lat1, heading, hujne, templat);
		hujne = hujne + 40;
		myfile << (templat * 180) / pi << "," << (templon * 180) / pi << endl;
	}
	myfile <<lat2 << "," << lon2 << endl;
	heading = gps_course_to(lat2, lon2, lat3, lon3);
	templat = 0.00;
	templon = 0.00;
	hujne = 3;
	while (gps_distance_between((templat * 180) / pi, (templon * 180) / pi, lat3, lon3)>3) {
		templat = latitude(lat2, hujne, heading);
		templon = longtitude(lon2, lat2, heading, hujne, templat);
		hujne = hujne + 3;
		myfile << (templat * 180) / pi << "," << (templon * 180) / pi << endl;
	}
	myfile << lat3 << "," << lon3 << endl;
	myfile.close();
}
double latitude(double lat1,double hujne, double heading) {
	double d = hujne / 6378100.00;
	return asin(sin((lat1*pi) / 180.00)*cos(d) + cos((lat1*pi) / 180.00)*sin(d)*cos(heading));

}
double longtitude(double lon1, double lat1, double heading, double hujne, double templat) {
	double d = hujne / 6378100.00;
	return (lon1*pi) / 180.00 + atan2(sin(heading)*sin(d)*cos((lat1*pi) / 180.00), cos(d) - sin((lat1*pi) / 180.00)*sin(templat));
}
double gps_course_to(double lat1, double long1, double lat2, double long2)
{
	// returns course in degrees (North=0, West=270) from position 1 to position 2,
	// both specified as signed decimal-degrees latitude and longitude.
	// Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
	// Courtesy of Maarten Lamers
	double dlon = ((long2 - long1) * 71.00) / 4068.00;
	lat1 = (lat1 * 71.00) / 4068.00;
	lat2 = (lat2 * 71.00) / 4068.00;
	double a1 = sin(dlon) * cos(lat2);
	double a2 = sin(lat1) * cos(lat2) * cos(dlon);
	a2 = cos(lat1) * sin(lat2) - a2;
	a2 = atan2(a1, a2);
	if (a2 < 0.0)
	{
		a2 += 2.00 * pi;
	}
	return a2;
}
double gps_distance_between(double lat1, double long1, double lat2, double long2)
{
	// returns distance in meters between two positions, both specified
	// as signed decimal-degrees latitude and longitude. Uses great-circle
	// distance computation for hypothetical sphere of radius 6372795 meters.
	// Because Earth is no exact sphere, rounding errors may be up to 0.5%.
	// Courtesy of Maarten Lamers
	double delta = ((long1 - long2) * 71.00) / 4068.00;
	double sdlong = (float)sin(delta);
	double cdlong = (float)cos(delta);
	lat1 = (lat1 * 71.00) / 4068.00;
	lat2 = (lat2 * 71.00) / 4068.00;
	double slat1 = sin(lat1);
	double clat1 = cos(lat1);
	double slat2 = sin(lat2);
	double clat2 = cos(lat2);
	delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
	delta = delta*delta;
	delta += (clat2 * sdlong)*(clat2 * sdlong);
	delta = sqrt(delta);
	float denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
	delta = atan2(delta, denom);
	return delta * 6372795.00;
}