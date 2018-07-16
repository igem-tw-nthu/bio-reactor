
#define analogInPin0 A0//OD
#define analogInPin1 A1//GFP

int cycle = 1;
int voltage[5];
int value0,value1,value2; 
float outputValue0; //OD
float outputValue1; //GFP
char val;
char mode;
const byte fan = 2;//5v
const byte GFP = 3;//5v
const byte OD = 4;//5v
const byte GREEN = 9;//12v

void setup() {
  Serial.begin(115200);
  Serial.println("Experiment start");
  delay(3000);
  pinMode(fan, OUTPUT); 
  pinMode(OD, OUTPUT);  
  pinMode(GREEN, OUTPUT); 
  pinMode(GFP, OUTPUT); 
}
void loop() {
 if(Serial.available()) {
     val = Serial.read();
 }
       voltage[0] = MODE(0);//O2
       voltage[1] = MODE(1);//OD
       voltage[2] = MODE(2);//GFP
       voltage[3] = cycle;//cycle
    digitalWrite(OD, LOW);
    digitalWrite(GFP,LOW);
    analogWrite(GREEN,0);
    analogWrite(fan,100);
    delay(5000);
    analogWrite(fan,80);
    analogWrite(GREEN,150);//4.05w/m2
    delay(5000);//12 min experiment  unit:ms
   
    voltage[3]=cycle;
    serial_printO2();
    analogWrite(GREEN,0);
    analogWrite(fan,0);
    digitalWrite(OD,HIGH);
    delay(10000);
     voltage[1] = MODE(1);
     outputValue0 = (voltage[1])*5/(float)1024;//OD  
     serial_printOD();
     digitalWrite(OD,LOW); 
     digitalWrite(GFP,HIGH);
     voltage[2] = MODE(2);
     outputValue1 = (voltage[2])*5/(float)1024;//GFP
     serial_printGFP();
     cycle++;
}//main loop
int filter(int k)//
{
int N = 101;
int value_buf[N];
int count,i,j;
int change_step;
for (count=0;count<N;count++){
    value_buf[count] = get_ad(k);
    delay(1);
}
for (j=0;j<N-1;j++){
   for (i=0;i<N-1-j;i++){
        if (value_buf[i] > value_buf[i+1])
        {
            change_step = value_buf[i];
            value_buf[i] = value_buf[i+1];
            value_buf[i+1] = change_step;
        }
    }
}
return value_buf[(N-1)/2];
} 

int get_ad(int k) {
  if(k==0){
  return analogRead(analogInPin0);
  }else if(k==1){
  return analogRead(analogInPin1);
  }
}

void serial_printOD()
{
  Serial.print("  V(OD) = ");
  Serial.print(outputValue0);
}

void serial_printGFP()
{
 Serial.print("   V(GFP) = ");
 Serial.println(outputValue1,4);
}

void serial_printO2()
{
  Serial.print("  cycle: ");
  Serial.print(cycle);
  //Serial.print("  O2 = ");
  //Serial.print(" % ");

}
int MODE(int k)
{
        int stats[20],x,y;
        int mode,count,oldcount,oldmode;
        if(k==0){
          for(x=0;x<20;x++){
          stats[x]=filter(0);
          delay(10);
          }
        }
        if(k==1){
          for(x=0;x<20;x++){
          stats[x]=filter(1);
          delay(10);
          }
        }
        if(k==2){
          for(x=0;x<20;x++){
          stats[x]=filter(2);
          delay(10);
          }
        }
        if(k==3){
          for(x=0;x<20;x++){
          stats[x]=filter(3);
          delay(10);
          }
        }
        if(k==4){
          for(x=0;x<20;x++){
          stats[x]=filter(4);
          delay(10);
          }
        }
        if(k==5){
          for(x=0;x<20;x++){
          stats[x]=filter(5);
          delay(10);
          }
        }
        if(k==6){
          for(x=0;x<20;x++){
          stats[x]=filter(6);
          delay(10);
          }
        }
        if(k==7){
          for(x=0;x<20;x++){
          stats[x]=filter(7);
          delay(10);
          }
        }
        oldcount = 0;
        for(x=0;x<20;x++){
                mode=stats[x];
                 count = 0; 
                for(y=0;y<20;y++){ 
                 if(mode==stats[y]) count++;
                }
  if(count>oldcount){
                        oldmode=mode;
                        oldcount=count;
                }
        }   
             return oldmode;
}

