unsigned char* input0[90],input1[90]; // input0 being data from GPS and input2 being from USB
unsigned int input0count=0,input1count=0;
char arrayStorage[3][15][15]; // arrayStorage 0 being to desired point, arrayStorage 1 being the current point, arrayStorage 2 being the Last point
void setup() {
  // put your setup code here, to run once:
  Serial.begin(921600); //serial for comm to PC
  Serial1.begin(9600); // Serial comm for GPS
}

void loop() {
  // put your main code here, to run repeatedly:
  int input1flag=0;
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
  if(input1flag==1){
    dataHandler(1);
  }


  
}
void dataHandler(int s){
  int i=0; // being the position in reading Input0
  int b=0; // being the counter for part eg.: GPGGA 
  int c=0; // character in the current part
  int flag=0;
  unsigned char* tempArray[15][15];
  while(input1[i]!='\0'){ // Creating an array that
    /*
    Serial.println("checking sentence");
    int chlen=0;
    while(input1[chlen]!='\0'){
      Serial.write(input1[chlen]);
      chlen++;
    }
    */
    if(input1[i]==','){ 
      tempArray[b][c]='\0';
      b++;
      c=0;
    } else {
      tempArray[b][c]=input1[i];
      c++;
    }
    if((b==1)&(c==0)){ // after the first part is written check if its a GPGGA sentence
      if(tempArray[0][1]=='G'){
        if(tempArray[0][2]=='P'){
          if(tempArray[0][3]=='G'){
            if(tempArray[0][4]=='G'){
              flag=1;
            }
            
          }
        }
      }
    }
    i++;
  }
  tempArray[b][c]='\0'; // finishing the string
  if(flag==1){
    //Serial.println("got gpgga");
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
    /*
    int g=0;
    while(input1[g]!='\0'){
      Serial.write(input1[g]);
    g++;
    }
    */
    
    
  }
}

