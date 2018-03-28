
int red_led=13;
int external_power=A0;
int button=11;

boolean button_on,power_on;
boolean change;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(red_led,OUTPUT);
  pinMode(external_power,INPUT);
  pinMode(button,INPUT);

  button_on=false;
  power_on=false;
  change=true;
}

void loop() {
  // put your main code here, to run repeatedly:

read_button();
check_power();
/*
    Serial.print("H");
    digitalWrite(red_led,LOW);  //on
    delay(6000);
    Serial.print('L');
    digitalWrite(red_led,HIGH);  /off
    delay(3000);
*/


if(change==true){
  if(button_on==true || power_on==true){
     Serial.print("H");
     digitalWrite(red_led,LOW);  //on
    }else{
     Serial.print("L");
     digitalWrite(red_led,HIGH);  //on

    }
    delay(500);
  }
  change=false;
}

void read_button(){

int check;
  check=digitalRead(button);
//  Serial.println(check);
 if(check==1){                 //we have a press:
   change=true;               // regardless - we want to send a xbee message I suspect.
    if(power_on==false) { // we only respond if the sprinkler is not running
         if(button_on==true) button_on=false;
         else button_on=true;  
    }
       
    }
 }
  

void check_power(){
 int check;
  check=analogRead(external_power);  /*
                                       Wired external + to 8.5Kohm to pin to 2kohm to Gnd
                                       Thus giving a value of up to 5 volts represented as a value between 0 and 1024
                                       i.e 204 per volt
                                       12 Volts will return 2/10.2 Times 12 =  0.1960   *12 = 2.352 volt
                                       value should therefore be 204*2.2352  plus minus 450
*/
 // Serial.println(check);

    if(check>200){

        if(power_on==false){
          change=true;
          power_on=true;
         }
    }else{
      if(power_on==true){
           change=true;
           power_on=false;
      }


    }



}




