unsigned char input0[90];
unsigned char input1[90]; // input0 being data from GPS and input2 being from USB
unsigned int input0count=0,input1count=0;
char* tempArray[]={"$GPGGA","212755.000","5454.07050","Na","00948.54505","E0","100","005","20.71","1.80","M","44.6","M","f","*52","a"};
char arrayStorage[3][15][15]; // arrayStorage 0 being to desired point, arrayStorage 1 being the current point, arrayStorage 2 being the Last point
float latitude1=0.00,latitude2=0.00,longtitude1=0.00,longtitude2=0.00;
float latitude0=54.91196082;
float longtitude0=9.78089243;
float course_current,course_desired;
float templon,templat;
int kunta=0;
float distance_to=0.00;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(921600); //serial for comm to PC
  Serial1.begin(9600); // Serial comm for GPS
  Serial1.write("$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n"); // To only receive $GPGGA sentence from GPS sensor
  while(!Serial1.available()){int kkkk=5;}
  Serial1.write("$PMTK220,1000*1F"); // Sets the update frequency from GPS, the Lat Long is update at 1Hz
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
    } else 
    {
      input0[input0count]=blabla; // continues to add to an array when the received character is not EoL
      input0count++;
    }
  }
  if(Serial1.available()){ // Receives the byte from GPS and wait for \n to start processin the line
    while(Serial1.available()){
    unsigned char hujesos=Serial1.read();
    //Serial.write(hujesos);
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
  }
  if(input0flag==1){
    dataHandler2();
  }
  if(input1flag==1){
    dataHandler();
  }


  
}
void dataHandler2(){
  int flag=0;
  int i=1;
  char temporary1[20]="aa";
  char temporary2[20]="aa";
  if(input1[0]=='#'){
    int kuk=0;
    while(input0[i]!=','){
      temporary1[kuk]=input0[i];
      i++;
      kuk++;
    }
    i++;
    temporary1[kuk+1]='\0';
    kuk=0;
    while(input0[i]!='\n'){
      temporary2[kuk]=input0[i];
      i++;
      kuk++;
    }
    temporary2[kuk+1]='\0';
    latitude0=atof(temporary1);
    longtitude0=atof(temporary2);
  }
}
void dataHandler(){ // when s=0 it decodes the Desired position, when s=1 it decodes the current GPS position
  int i=0; // being the position in reading Input0
  int b=0; // being the counter for part eg.: GPGGA 
  int c=0; // character in the current part
  int flag=0;
  
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
        //Serial.write(tempArray[b]);
        b++;
        c=0;
      } else {
        tempArray[b][c]=input1[i];
        c++;
      }
      i++;
      if(b==5&&c==0){
        Serial.write(tempArray[4]);
        Serial.write("\r\n");
      }
    }
    tempArray[b][c]='\0'; // finishing the string
    /*
    int counterss=0;    
    for(int kuk=0;kuk<15;kuk++){
      Serial.write(tempArray[kuk]);
      if( kuk==14){}else{
        Serial.write(',');
      }
    }
    int llll=0;
    while(input1[llll]!='\0'){
      Serial.write(input1[llll]);
      llll++;
    }
    Serial.write("\r\n");
    */
    if(atof(tempArray[6])>0){
      templat=conv_to_deg(atof(tempArray[2])); // converting the new String of latitude to degrees, as it originally comes in HH:mm:ss;
      templon=conv_to_deg(atof(tempArray[4]));
      distance_to=gps_distance_between(templat,templon,latitude1,longtitude1);
      /*
      Serial.write("lel: \r\n"); // Not being printed, BUG
      printDouble(distance_to,4);
      Serial.write("latitude: ");
      printDouble(conv_to_deg(atof(tempArray[2])),4);
      Serial.write("  longtitude: ");
      printDouble(conv_to_deg(atof(tempArray[4])),4);
      Serial.write("\r\n");
      Serial.write("faf: ");
      printDouble(distance_to,4);
      Serial.write("latitude: ");
      printDouble(latitude1,4);
      Serial.write("  longtitude: ");
      printDouble(longtitude1,4);
      Serial.write("\r\n");
      */
      if(distance_to>20){ // How often should the heading be updated.
        int llll=0;
        while(input1[llll]!='\0'){
          Serial.write(input1[llll]); // Printing the current NMEA sentence
          llll++;
        }
        Serial.write("\r\n");
    /*
        Serial.write("BITCHIN\r\n");
        float course_current=gps_course_to(latitude1,longtitude1,templat,templon);
        Serial.write("\t\t");
        */
        printDouble(course_current,4); // Prints the current course
        Serial.write("\r\n");
        
        latitude1=templat; // updates the previous latitude, longitude with new values
        longtitude1=templon;
      } 
    }else {Serial.println("NO GPS FIX");}
    for(int uu=0;uu<16;uu++){
      tempArray[uu][0]='\0';
    }
   
  }
}
  

float conv_to_deg(float dataIn) // input being 0103.523 meaning 01 is deg and 03 minutes and 52.3seconds
{

  int chen=dataIn/100; // getting the degree part from input
  float temp=(dataIn-chen*100)/60.00;
  return (temp+chen);
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
  float dlon = radians(long2-long1);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  float a1 = sin(dlon) * cos(lat2);
  float a2 = sin(lat1) * cos(lat2) * cos(dlon);
  a2 = cos(lat1) * sin(lat2) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0)
  {
    a2 += TWO_PI;
  }
  return degrees(a2);
}
void printDouble( double val, byte precision){
 // prints val with number of decimal places determine by precision
 // precision is a number from 0 to 6 indicating the desired decimial places
 // example: printDouble( 3.1415, 2); // prints 3.14 (two decimal places)

 Serial.print (int(val));  //prints the int part
 if( precision > 0) {
   Serial.print("."); // print the decimal point
   unsigned long frac;
   unsigned long mult = 1;
   byte padding = precision -1;
   while(precision--)
      mult *=10;
     
   if(val >= 0)
     frac = (val - int(val)) * mult;
   else
     frac = (int(val)- val ) * mult;
   unsigned long frac1 = frac;
   while( frac1 /= 10 )
     padding--;
   while(  padding--)
     Serial.print("0");
   Serial.print(frac,DEC) ;
 }
}
