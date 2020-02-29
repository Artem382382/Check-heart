#include <iarduino_OLED.h>
iarduino_OLED Serial;
#include <iarduino_SensorPulse.h>
iarduino_SensorPulse Pulse(A0);
const int signalPin = A1; 
const int bufSize = 16; 
const int hyst = 1;
const int crossSkip = 4; 
int buf[bufSize]; 
int bufPos = 0; 
long int bufSum = 0;
int threshold = 0; 
int skipDetect = 0; 
unsigned long prevTime, time; 
boolean above; 
int pulse, Medpulse, pulseAvg;
boolean pr;
float bsort; 
int sensorValue;
const int xpin = A1;
const int ypin = A2;
const int zpin = A3;
byte p[8] = {
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F
};
float threshold = 6;
float xval[100] = {0};
float yval[100] = {0};
float zval[100] = {0};
float xavg, yavg, zavg;
int steps, flag = 0;

int medFilt(int value){
  const int bufSize = 7; 
  static int buf[bufSize]; 
  static int sortBuf[bufSize]; 
  static int pos = 0;  
  buf[pos] = value;  
  pos++;
  if (pos == bufSize) pos=0;
  for(int i=0; i < bufSize; i++){
    sortBuf[i]=buf[i];
  }
for(int i = 0; i < bufSize-1; i++){
  pr = false; 
   for(int j = 0; j < bufSize-i; j++){
    if(sortBuf[j] > sortBuf[j + 1]){
      bsort = sortBuf[j];
      sortBuf[j] = sortBuf[j + 1];
      sortBuf[j + 1] = bsort;
      pr = true;
      }
    }
    if(pr == false){
      break;
      }
   }
  return sortBuf[(bufSize / 2)];
}

int avg(int value){
  const int bufSize = 16; 
  static int buf[bufSize];
  static int pos;
  static long int sum = 0;
  int avg;
  
  buf[pos] = value;
  sum += value;
  avg = sum / bufSize;
  if ((sum % bufSize) >= (bufSize / 2)) avg++;
  pos++;
  if (pos == bufSize) pos=0;
    
  sum -= buf[pos]; 
  return avg;
}



void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  myOLED.begin();
  myOLED.backlight();
  myOLED.clear();
  calibrate();
  pinMode(10,INPUT);
  pinMode(11,INPUT);
}
void loop()
{
  for (int w = 0; w < 16; w++) {
    myOLED.write(byte(0));
  }
  int acc = 0;
  float totvect[100] = {0};
  float totave[100] = {0};
  float xaccl[100] = {0};
  float yaccl[100] = {0};
  float zaccl[100] = {0};
  for (int a = 0; a < 100; a++)
  {
    xaccl[a] = float(analogRead(xpin) - 345);
    delay(1);
    yaccl[a] = float(analogRead(ypin) - 346);
    delay(1);
    zaccl[a] = float(analogRead(zpin) - 416);
    delay(1);
    totvect[a] = sqrt(((xaccl[a] - xavg) * (xaccl[a] - xavg)) + ((yaccl[a] - yavg) * (yaccl[a] - yavg)) + ((zval[a] - zavg) * (zval[a] - zavg)));
    totave[a] = (totvect[a] + totvect[a - 1]) / 2 ;
    delay(100);
    if (totave[a] > threshold && flag == 0)
    {
      steps = steps + 1;
      flag = 1;
    }
    else if (totave[a] > threshold && flag == 1)
    {
    }
    if (totave[a] < threshold   && flag == 1)
    {
      flag = 0;
    }
    if (steps < 0) {
      steps = 0;
    }
    myOLED.print("Steps: ");
    myOLED.print(steps);
    myOLED.clear();
  }
}
void calibrate()
{
  float sum = 0;
  float sum1 = 0;
  float sum2 = 0;
  for (int i = 0; i < 100; i++) {
    xval[i] = float(analogRead(xpin) - 345);
    sum = xval[i] + sum;
  }
  xavg = sum / 100.0;
  for (int j = 0; j < 100; j++)
  {
    yval[j] = float(analogRead(ypin) - 346);
    sum1 = yval[j] + sum1;
  }
  yavg = sum1 / 100.0;
  for (int q = 0; q < 100; q++)
  {
    zval[q] = float(analogRead(zpin) - 416);
    sum2 = zval[q] + sum2;
  }
  zavg = sum2 / 100.0;
  if((digitalRead(10)==1)||(digitalRead(11)==1)){
      Serial.println("Gagal");
  }
  else{
      Serial.println(analogRead(A1));
  }
}
