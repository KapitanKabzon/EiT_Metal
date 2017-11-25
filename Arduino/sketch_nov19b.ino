unsigned long start_time;
unsigned long stop_time;
unsigned long values[1000];
uint16_t buf[4][256];   // 4 buffers of 256 readings


 #define SYS_BOARD_OSCOUNT   (CKGR_MOR_MOSCXTST(0x8UL))
#define SYS_BOARD_PLLAR     (CKGR_PLLAR_ONE \
                            | CKGR_PLLAR_MULA(0xfUL) \
                             | CKGR_PLLAR_PLLACOUNT(0x3fUL) \
                            | CKGR_PLLAR_DIVA(0x1UL))
 #define SYS_BOARD_MCKR      (PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK)
 
void setup() {        
  Serial.begin(115200);  
  adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST);
  ADC->ADC_MR |= 0x80;  //set free running mode on ADC
  ADC->ADC_CHER = 0x80; //enable ADC on pin A0
}

void loop() {
  unsigned int i;
    
  start_time = micros();
  for(i=0;i<1000;i++){
    while((ADC->ADC_ISR & 0x80)==0); // wait for conversion
    values[i]=ADC->ADC_CDR[7]; //get values
  }
  stop_time = micros();

  Serial.print("Total time: ");
  Serial.println(stop_time-start_time); 
  Serial.print("Average time per conversion: ");
  Serial.println((float)(stop_time-start_time)/1000);

  Serial.println("Values: ");
  /*for(i=0;i<1000;i++) {
    Serial.println(values[i]);
  }*/
  
  delay(2000);
}
