unsigned char* input0[90],input1[90]; // input0 being data from GPS and input2 being from USB
unsigned int input0count=0,input1count=0;
char arrayStorage[3][15][15]; // arrayStorage 0 being to desired point, arrayStorage 1 being the current point, arrayStorage 2 being the Last point
float latitude1,latitude2,longtitude1,longtitude2;
float course_current,course_desired;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(921600); //serial for comm to PC
  Serial1.begin(9600); // Serial comm for GPS
}

void loop() {
  // put your main code here, to run repeatedly:
  int input1flag=0;
  int input0flag=0;
  if(Serial.available()){ // Receiving byte from RPi and then sending it to data handling services.
    unsigned char blabla=Serial.read();
    if(blabla=='\n'){
      input0[input0count]=blabla; // this goes when the EoL character is received
      input0[input0count+1]='\0';
      input0flag=1;
      input0count=0;
    } else {
      input0[input0count]=blabla; // continues to add to an array when the received character is not EoL
      input0count++;
    }
  }
  if(Serial1.available()){ // Receives the byte from GPS and wait for \n to start processin the line
    unsigned char hujesos=Serial1.read();
    if(hujesos=='\n'){
      input1[input1count]=hujesos;
      input1[input1count+1]='\0';
      input1flag=1;
      input1count=0;
    } else {
      input1[input1count]=hujesos;
      input1count++;
    }
  }
  if(input0flag==1){
    dataHandler(0);
  }
  if(input1flag==1){
    dataHandler(1);
  }


  
}
void dataHandler(int s){ // when s=0 it decodes the Desired position, when s=1 it decodes the current GPS position
  int i=0; // being the position in reading Input0
  int b=0; // being the counter for part eg.: GPGGA 
  int c=0; // character in the current part
  int flag=0;
  unsigned char tempArray[15][15];
  if(s==1){ // checking the s
    if(input1[1]=='G'){ // checks if the incoming array of bytes from GPS sensor is GPGGA type.
      if(input1[2]=='P'){
        if(input1[3]=='G'){
          if(input1[4]=='G'){
            if(input1[5]=='A'){
              flag=1; // setting the flag if the conditions are met
            }
          }
        }
      }
    }
    if(flag==1){ // analyzing the 
      while(input1[i]!='\0'){ // Creating an array that would have all the parts of a sentece in separate strings.
        if(input1[i]==','){ 
          tempArray[b][c]='\0';
          b++;
          c=0;
        } else {
          tempArray[b][c]=input1[i];
          c++;
        }
        i++;
      }
      tempArray[b][c]='\0'; // finishing the string
      latitude2=latitude1; // switching the known position to previous position
      longtitude2=longtitude1;
      latitude1=conv_to_deg(atof(tempArray[2])/100.00); // converting the new String of latitude to degrees, as it originally comes in HH:mm:ss
      longtitude1=conv_to_deg(atof(tempArray[4])/100.00);
      course_current=gps_course_to(latitude1,longtitude1,latitude2,longtitude2); // returns the current course from last two positions
      for(int v=0;v<b+1;v++){
        int h=0;
        while(tempArray[v][h]!='\0'){
          unsigned char pidaras=tempArray[v][h];
          Serial.write(pidaras);
          h++;
        }
        if(v<b){
          Serial.write(',');
        }
      }
    }
  } else {
    if(input0[0]=='$'){ // checks if the incoming array of bytes from RPi is NMEA sentence.
    }
  }
}

float conv_to_deg(float dataIn)
{
  int i=0;
  int nein=0;
  while(nein==0){
    if(dataIn-i>1){i++;} else {nein=1;}
  }
  float blet=dataIn-i;
  blet=blet*1.66666+i;
  return blet;
}
float gps_distance_between (float lat1, float long1, float lat2, float long2)
{
  // returns distance in meters between two positions, both specified
  // as signed decimal-degrees latitude and longitude. Uses great-circle
  // distance computation for hypothetical sphere of radius 6372795 meters.
  // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
  // Courtesy of Maarten Lamers
  float delta = ((long1-long2) * 71) / 4068;
  float sdlong = (float)sin(delta);
  float cdlong = (float)cos(delta);
  lat1 = (lat1 * 71) / 4068;
  lat2 = (lat2 * 71) / 4068;
  float slat1 = sin(lat1);
  float clat1 = cos(lat1);
  float slat2 = sin(lat2);
  float clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta = delta*delta;
  delta += (clat2 * sdlong)*(clat2 * sdlong);
  delta = sqrt(delta);
  float denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta = atan2(delta, denom);
  return delta * 6372795;
}
float gps_course_to(float lat1, float long1, float lat2, float long2)
{
  // returns course in degrees (North=0, West=270) from position 1 to position 2,
  // both specified as signed decimal-degrees latitude and longitude.
  // Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
  // Courtesy of Maarten Lamers
  float dlon = ((long2 - long1) * 71) / 4068;
  lat1 = (lat1 * 71) / 4068;
  lat2 = (lat2 * 71) / 4068;
  float a1 = sin(dlon) * cos(lat2);
  float a2 = sin(lat1) * cos(lat2) * cos(dlon);
  a2 = cos(lat1) * sin(lat2) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0)
  {
    a2 += 2*3.14;
  }
  return a2 * (180.0 / 3.141592);
}
