unsigned char input0[90],input1[90]; // input0 being data from GPS and input2 being from USB
unsigned int input0count=0,input1count=0;
char arrayStorage[3][15][15]; // array 0 being to desired point, array 1 being the current point, array 2 being the Last point
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9216000; //serial for comm to PC
  Serial1.begin(9600); // Serial comm for GPS
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial1.available()){ // Receives the byte from GPS and wait for \n to start processin the line
    input0[input0count]=Serial1.read();
    if(input0[input0count]=='\n'){
      input0flag=1;
      input0count=0;
    } else {
      input0count++;
    }
  }


  
}
void dataHandler(int s){
  int i=0; // being the position in reading Input0
  int b=0; // being the counter for part eg.: GPGGA 
  int c=0; // character in the current part
  char tempArray[15][15];
  while(input0[i]!='\0'){ // Creating an array that
    if(input0[i]==','){ 
      tempArray[b][c]='\0';
      b++;
      c=0;
    } else {
      tempArray[b][c]=input0[i];
      c++;
    }
    if((b==1)&(c==0)){ // after the first part is written check if its a GPGGA sentence
      if(tempArray[0]=="GPGGA"){
        flag=1;
      }
    }
    i++;
  }
  tempArray[b][c]='\0'; // finishing the string
  if(flag==1){
    arrayStorage[2]=arrayStorage[1]; // pushing down the previous current position to Last position
    arrayStorage[1]=tempArray; 
  }
}

