#include <Wire.h>

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

int c=0,f=0;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;

void setup(){
   Serial.begin(115200);
  Wire.begin();
  setupMPU();
  pinMode(2, OUTPUT);
}

void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processAccelData();
}

void processAccelData(){
  gForceX = accelX / 16384.0;
  gForceY = accelY / 16384.0; 
  gForceZ = accelZ / 16384.0;
}

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processGyroData();
}

void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0; 
  rotZ = gyroZ / 131.0;
}

void loop(){
  recordAccelRegisters();
  recordGyroRegisters();
   printData();

  if(gForceX<1 && gForceZ>0)//&&(gForceY>=0 && gForceY<=0.1) && (gForceZ>=0.9 && gForceZ<=1.1))
  {
    Serial.println("x change");
    rf_send('x');
     //delay(2000);
    // c++;
  }
   else if(gForceX<1 && gForceY<0)//&&(gForceY>=0 && gForceY<=0.1) && (gForceZ>=0.9 && gForceZ<=1.1))
  {
    Serial.println("y change");
    rf_send('y');
    // delay(2000);
    //f++;
  }
   else   if(gForceX<1 && gForceY>1&&gForceZ<0)
   {
    Serial.println("z change");
    rf_send('z');
   }

   //---------------------------
/*
   if(c==1)
   {
        if(gForceX<1 && gForceZ>0)//&&(gForceY>=0 && gForceY<=0.1) && (gForceZ>=0.9 && gForceZ<=1.1))
        {
          Serial.println("second x change");
            rf_send('a');
            c=0;
        }
  }

  if(f==1)
  {
         if(gForceX<1 && gForceY<0)//&&(gForceY>=0 && gForceY<=0.1) && (gForceZ>=0.9 && gForceZ<=1.1))
        {
          Serial.println(" second y change");
          rf_send('b');
          f=0;
        }
  }
*/
  
 /* else
  {
  rf_send('a');
  rf_send('b');
  rf_send('c');
  }*/
  
  /*else if((gForceX>=-0.20 && gForceX<=0.1 )&&(gForceY>0.1) && (gForceZ>=0.9 && gForceZ<=1.1))
  {
    Serial.println("y change");
    rf_send('y');
  }
  else   if((gForceX>=-0.20 && gForceX<=0.1 )&&(gForceY>=0 && gForceY<=0.1) && (gForceZ>1.1))
   {
    Serial.println("z change");
    rf_send('z');
  }*/
  /*rf_send(gForceX);
  rf_send(gForceY);
  rf_send(gForceZ);
  rf_send(rotX);
  rf_send(rotY);
  rf_send(rotZ);*/
  //  rf_send('#');
  delay(3000);
  //v++;
}

void rf_send(byte input){
  int i;
  
  for(i=0; i<20; i++){
  digitalWrite(3, HIGH);
  delayMicroseconds(500);
  digitalWrite(3, LOW);
  delayMicroseconds(500);
}

  digitalWrite(3, HIGH);
  delayMicroseconds(3000);
  digitalWrite(3, LOW);
  delayMicroseconds(500);
  
    
  for(i=0; i<8; i++){
  if(bitRead(input,i)==1)
  digitalWrite(3, HIGH);
  else
  digitalWrite(3, LOW);
  delayMicroseconds(500);
  
  if(bitRead(input,i)==1)
  digitalWrite(3, LOW);
  else
  digitalWrite(3, HIGH);
  delayMicroseconds(500);
}//i

  
  digitalWrite(3, LOW);
}

void printData() {
  Serial.print("Gyro (deg)");
  Serial.print(" X=");
  Serial.print(rotX);
  Serial.print(" Y=");
  Serial.print(rotY);
  Serial.print(" Z=");
  Serial.print(rotZ);
  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);
}

