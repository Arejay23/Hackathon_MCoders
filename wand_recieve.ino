int i, good, k;
byte data_in;
void setup(){
  attachInterrupt(1,data_incoming,RISING);
  pinMode(3, INPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(115200);
}//setup

void loop(){
  
  
}//loop

void data_incoming(){
    
    
    for(i=0; i<100; i++){
      delayMicroseconds(20);
      good=1;
      if(digitalRead(3)==LOW){
      good=0;
      i=100;
      }
    }//for
      
    if(good==1){
    detachInterrupt(1);
    while(1){
      if(digitalRead(3)==LOW){
        delayMicroseconds(750);

        for(i=0; i<8; i++){
          if(digitalRead(3)==HIGH)
          bitWrite(data_in, i, 1);
          else
          bitWrite(data_in, i, 0);
          delayMicroseconds(1000);
        }//for
        
        if(data_in=='x')
        {
          Serial.println("X=");
          digitalWrite(10,HIGH);
        }
        

        else if(data_in=='a')
        digitalWrite(10,LOW);

        if(data_in=='y')
        {
          Serial.println("Y=");
          digitalWrite(11,HIGH);
        }
        

       else if(data_in=='b')
        digitalWrite(11,LOW);

        if(data_in=='z')
        {
          Serial.println("Z=");
          digitalWrite(11,LOW);
          digitalWrite(10, LOW);
        }
    

       break;//secondtwhile
      }//low kickoff
      
    }//second while
    
    }//good check

  attachInterrupt(1,data_incoming,RISING);
  
  
  
}//routine
