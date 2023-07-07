#define getBit(value, bitn) ((((value)&(1<<(bitn)))>>(bitn))?HIGH:LOW)
int CLK  = 4;
int DIO = 2;
int STB   = 3;
int v1=1;
int v2=2;
int v3=3;
int v4=4;
byte LED[12] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D,0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x63 };

void setup() {
   pinMode(DIO, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(STB, OUTPUT);
  digitalWrite(DIO, HIGH);
  digitalWrite(CLK, HIGH);
  digitalWrite(STB, HIGH);
  initET6202();

}

void loop() {
  printLED(1,1,2,1,2);
  }
 void initET6202(){
    order2();
    order3_init();
    order1();
    order4();
  }
  void printLED(char d1, char d2, char d3, char d4, char d5){
 
  order2();
      int data[14] = {LED[d1], 0, LED[d2], 0, LED[d3], 0, LED[d4], 0, LED[d5], 0,};
    sendCmd(0xC0, data, 14, false);
    order1();  
    order4();
    }
    unsigned long sendCmd(int cmd, int * data, int size, int nocshigh ){
    int i=0;
    int j=0;
    //noInterrupts();
    digitalWrite(STB, LOW);    

    for (i=0; i<8; i++){
      digitalWrite(CLK, LOW);
      digitalWrite(DIO, getBit(cmd, i));
      digitalWrite(CLK, HIGH);
      
    }

    if(data != NULL){
        for(j=0; j<size; j++){
            for (i=0; i<8; i++){
                digitalWrite(CLK, LOW);
                digitalWrite(DIO, getBit(data[j], i));
                digitalWrite(CLK, HIGH);
               
            }     
        }
    }

    if(nocshigh == false){
      digitalWrite(STB, HIGH);
      digitalWrite(DIO, HIGH);
      digitalWrite(CLK, HIGH);
    }

}

void order3_init(){
    int cmd = 0xC0;
    int data[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    sendCmd(cmd, data, 14, false);
}


void order2(){
    int cmd = 0x40; //0100 0000  normal mode, auto increment, write mode
    sendCmd(cmd, NULL, 0, false);
}
void order3(){
    int cmd = 0xC0;
    int data[14] = {LED[8], 0, LED[9], 0, LED[6], 0, LED[7], 0 ,LED[5], 0 };
    sendCmd(cmd, data, 14, false);  
}

void order1(){
    int cmd = 0x03; //11 paragraph x 6 bit
    sendCmd(cmd, NULL, 0, false);
}

void order4(){
    int cmd = 0x8F; // dimming 0x88 ~ 0x8F
    sendCmd(cmd, NULL, 0, false);
}
