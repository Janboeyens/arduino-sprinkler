/*
  Physical Pixel

 An example of using the Arduino board to receive data from the
 computer.  In this case, the Arduino boards turns on an LED when
 it receives the character 'H', and turns off the LED when it
 receives the character 'L'.
 http://www.arduino.cc/en/Tutorial/PhysicalPixel

 The BEE comm uses this serial port to communicate (in transparent mode) 

 
 
 */

const int Green_led= 8;
const int Green_button=9;   // these are wired 5v-> 3.3Kohm ->led -> pin

const int Blue_led=10;
const int Blue_button=11;
const int Red_led=12;
const int Red_button=7;
const int Yellow_led=6;
boolean change;
int mode;  // 1 on 2 off 3 auto

boolean pump;

const int songle= 4;

const int ledPin = 13; // the pin that the LED is attached to
int incomingByte;      // a variable to read incoming serial data into

void setup() {
  // initialize serial communication:
  Serial.begin(9600);  // BEE set to transparent mode will just pick it up.
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);   // this is the built-oin arduino led..  part of the original demo.
  pinMode(Green_button,INPUT);
  pinMode(Green_led,OUTPUT); 

  pinMode(Blue_button,INPUT);
  pinMode(Blue_led,OUTPUT); 
  pinMode(Red_button,INPUT);
  pinMode(Red_led,OUTPUT); 
  pinMode (songle,OUTPUT);
  
pinMode(Yellow_led,OUTPUT);
pump=false;
  
  mode=3;
  change=false;
}

void loop() {

    read_xbee();

    check_buttons();

    show_led();

   do_pump();
    
 if(change==true){ delay(500); change=false;} // so the button doesn't keep on getting pressed. 
    
}



void read_xbee(){
    
  // see if there's incoming serial data:
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'H') {
      if(mode==3) pump=true;
      digitalWrite(ledPin, HIGH);
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == 'L') {
      if(mode==3) pump=false;
      digitalWrite(ledPin, LOW);
    }
  }
}

void check_buttons(){

int check;
check=digitalRead(Green_button);
 //    Serial.println(check);
    if(check==HIGH){ // it has  been pressed
              mode=2; pump=true;
              change=true;
    }
check=digitalRead(Blue_button);
 //    Serial.println(check);
    if(check==HIGH){ // it has  been pressed
              mode=3;
              change=true;
    }
check=digitalRead(Red_button);
 //    Serial.println(check);
    if(check==HIGH){ // it has  been pressed
      pump=false;
              mode=1;
              change=true;
    }


}


void show_led(){
   switch(mode){

       case 1:
            digitalWrite(Red_led,LOW);
            digitalWrite(Green_led,HIGH);
            digitalWrite(Blue_led,HIGH);
         break;

       case 2:
            digitalWrite(Red_led,HIGH);
            digitalWrite(Green_led,LOW);
            digitalWrite(Blue_led,HIGH);
       
           break;

        case 3:
            digitalWrite(Red_led,HIGH);
            digitalWrite(Green_led,HIGH);
            digitalWrite(Blue_led,LOW);

            break;
   }

   // also check if the power is on:
   if(pump==true)digitalWrite(Yellow_led,LOW);
   else digitalWrite(Yellow_led,HIGH);
   
   }

void do_pump(){
  // switch the relay 
  if(pump==true) digitalWrite(songle,LOW);
  else digitalWrite(songle,HIGH);
  
    //  Serial.println(pump);
}



