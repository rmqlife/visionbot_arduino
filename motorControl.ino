const int wheel[4]={11,10,9,8};
const int velocityPin=12;
const int MAXINT=32760;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
 for (int i=0;i<4;i++){
   pinMode(wheel[i],OUTPUT);
 }
 pinMode(velocityPin,INPUT);
}

void driveSimple(int pinA,int pinB, int ahead)
{
  if (ahead>0)
  {
    digitalWrite(pinA,HIGH);
    digitalWrite(pinB,LOW);
   }
   else if (ahead==0)
   {
     digitalWrite(pinA,LOW);
     digitalWrite(pinB,LOW);
   }
   else {
     digitalWrite(pinA,LOW);
     digitalWrite(pinB,HIGH);
   }
}

void driveSingle(int right,int ahead)
{
  if (right>0)
    driveSimple(wheel[0],wheel[1],ahead);
  else
    driveSimple(wheel[2],wheel[3],ahead);
}



void driveBoth(int ahead1,int ahead2)
{
  driveSingle(-1,ahead1);
  driveSingle(1,ahead2);
}

int currentOrder = 4;
int vLastStatus=0;
int vInteval=0;

int vIntevalOrder=0;
long vIntevalSeqSum=0;
const int vIntevalCOUNT=50;
int vIntevalSeq[50];


unsigned long time,lastTime;


void loop() {
  // send data only when you receive data:
   if (Serial.available() > 0) {
                // read the incoming byte:
                int order = Serial.read()-48;
                // say what you got:
                Serial.print("I received: ");
                Serial.print(order,DEC);
                Serial.print("   ");

                if (order>-1 && order<9)
                   currentOrder=order;
                Serial.print(currentOrder/3-1);
                Serial.print("   ");
                Serial.print(currentOrder%3-1);
                driveBoth(currentOrder / 3-1, currentOrder % 381);
           
                Serial.println();

  }
  
  ///count velocity
  int vStatus=digitalRead(velocityPin);
  if (vStatus!=vLastStatus){
    vIntevalOrder=(vIntevalOrder+1) % vIntevalCOUNT; 
    vIntevalSeqSum=vIntevalSeqSum+vInteval-vIntevalSeq[vIntevalOrder];    
    vIntevalSeq[vIntevalOrder]=vInteval;
    //Serial.println(vIntevalSeqSum/vIntevalCOUNT);
    
    
    
    vInteval=0;
    vLastStatus=vStatus;
  }else if (vInteval<MAXINT)
    vInteval++;
  
  
  ///time test
  if (millis()-lastTime==100){
    Serial.println(millis());
    lastTime=millis();
  }
}
