#include <iostream>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <iomanip>

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
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
	double lat1 = 54.91214739;
	double lon1 = 9.78054911;
	double lat2 = 54.91227613;
	double lon2 = 9.78086561;
	double lat3 = 54.91251742;
	double lon3 = 9.78061348;
	double lat4 = 54.91236709;
	double lon4 = 9.78027016;

	/*
	char *huj;
	int faget = nmea0183_checksum("$GPGGA,013827.428,5454.740,N,00946.813,E,1,12,1.0,0.0,M,0.0,M,,*6f");
	cout <<std::hex<< faget<<endl;*/
	double heading = gps_course_to(lat1, lon1, lat2, lon2);
	
	double templat=0.00, templon=0.00;
	int hujne;
	int countA = 1, countB = 1;
	double arrayA[999][2];
	double arrayB[999][2];
	arrayA[0][0] = (lat1*pi) / 180.00;
	arrayA[0][1] = (lon1*pi) / 180.00;
	arrayB[0][0] = (lat2*pi) / 180.00;
	arrayB[0][1] = (lon2*pi) / 180.00;
	ofstream myfile;
	myfile.open("lat.txt");
	myfile.clear();

	myfile << std::setprecision(15);

	myfile << std::setprecision(15) << lat1 << "," << lon1 << endl;
	cout << std::setprecision(15);
	/*
	hujne = 10;

	while (gps_distance_between((templat * 180) / pi, (templon * 180) / pi, lat2, lon2)>10) {
		 templat = latitude(lat1,hujne,heading);
		 templon = longtitude(lon1, lat1, heading, hujne, templat);
		hujne = hujne + 10;
		myfile << (templat * 180) / pi << "," << (templon * 180) / pi << endl;
	}
	myfile <<lat2 << "," << lon2 << endl;
	*/

	heading = gps_course_to(lat1, lon1, lat4, lon4);
	templat = 0.00;
	templon = 0.00;
	hujne = 3;
	while (gps_distance_between((templat * 180) / pi, (templon * 180) / pi, lat4, lon4)>5) {
		templat = latitude(lat1, hujne, heading);
		templon = longtitude(lon1, lat1, heading, hujne, templat);
		hujne = hujne + 3;
		//myfile << (templat * 180) / pi << "," << (templon * 180) / pi << endl;
		arrayA[countA][0] = templat;
		arrayA[countA][1] = templon;
		countA++;
	}
	//myfile << lat4 << "," << lon4 << endl;


	heading = gps_course_to(lat2, lon2, lat3, lon3);
	templat = 0.00;
	templon = 0.00;
	hujne = 3;
	while (gps_distance_between((templat * 180) / pi, (templon * 180) / pi, lat3, lon3)>5) {
		templat = latitude(lat2, hujne, heading);
		templon = longtitude(lon2, lat2, heading, hujne, templat);
		hujne = hujne + 3;
		//myfile << (templat * 180) / pi << "," << (templon * 180) / pi << endl;
		arrayB[countB][0] = templat;
		arrayB[countB][1] = templon;
		countB++;
	}
	//myfile << lat3 << "," << lon3 << endl;
	
	cout << countA << '\t' << countB << endl;
	for (int i = 0;i < countA;i++) {
		myfile << (arrayA[i][0] * 180) / pi << "," << (arrayA[i][1] * 180) / pi << endl;
	}
	for (int i = 0;i < countB;i++) {
		myfile << (arrayB[i][0] * 180) / pi << "," << (arrayB[i][1] * 180) / pi << endl;
	}
	myfile.close();
	int linecount = 0,linestodo=0;
	if (countA > countB) { linestodo = countA; }
	else { linestodo = countB; }
	cout << linestodo << endl;
	ofstream myfile2;
	myfile2.open("snake.txt");
	myfile2.clear();
	myfile2 << std::setprecision(15);
	templat = 0.00;
	templon = 0.00;
	for (int i = 0;i < linestodo;i++) {
		heading = gps_course_to((arrayA[i][0] * 180) / pi, (arrayA[i][1] * 180) / pi, (arrayB[i][0] * 180) / pi, (arrayB[i][1] * 180) / pi);
		templat = 0.00;
		templon = 0.00;
		hujne = 8;
		myfile2 << (arrayA[i][0] * 180) / pi << "," << (arrayA[i][1] * 180) / pi << endl;
		while (gps_distance_between((templat * 180) / pi, (templon * 180) / pi, (arrayB[i][0]*180)/pi, (arrayB[i][1]*180)/pi)>6) {
			templat = latitude((arrayA[i][0]*180)/pi, hujne, heading);
			templon = longtitude((arrayA[i][1]*180)/pi, (arrayA[i][0]*180)/pi, heading, hujne, templat);
			hujne = hujne + 8;
			myfile2 << (templat * 180) / pi << "," << (templon * 180) / pi << endl;
			cout << (arrayA[i][0] * 180) / pi << "," << (arrayA[i][1] * 180) / pi << endl;
			cout << (templat * 180) / pi << "," << (templon * 180) / pi << endl;
			cout << gps_distance_between((templat * 180) / pi, (templon * 180) / pi, (arrayB[i][0] * 180) / pi, (arrayB[i][1] * 180) / pi) << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(7));
		}
		myfile2 << (arrayB[i][0] * 180) / pi << "," << (arrayB[i][1] * 180) / pi << endl;
		cout << "\t\t"<<"new shit"<<i << endl;
	}
	
	//myfile2 << lat2 << "," << lon2 << endl;
	myfile2.close();

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
	double denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
	delta = atan2(delta, denom);
	return delta * 6372795.00;
}