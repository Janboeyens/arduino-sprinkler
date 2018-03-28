
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // use the sketch_may12a to find the address ;-)



int water_delay;  //the time each component remains on  

// zone plugs 2 - 6

int transformer_songle=12;
int pump_songle=13;

int screen_button=15;
boolean screen_on,screen_changed;

int start_now_button=16;   // A2
int skip_button=17;       // A3



int date_set_button=9;
int auto_set_button=8;

int auto_set_led=10;
boolean auto_on;


int date_set_led=11;
boolean date_set;


boolean started;
int zones;
int current_zone;
int zone_next_hour,zone_next_minute;

int Next_hour,Next_minute;
boolean change;

int old_min;
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}
void setup()
{
  int a;
  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  // setDS3231time(30,18,21,2,4,9,17);

      lcd.begin(20,4); //Start the lcd and define how many rows and characters it has. 20 characters and 4 rows.

    old_min=0;
    lcd.backlight();//Start the backlight

    water_delay=7;
    zones=6; 
     pinMode(auto_set_button, INPUT);
     pinMode(auto_set_led,OUTPUT);
     pinMode(date_set_button,INPUT);
     pinMode(date_set_led,OUTPUT);

     pinMode(screen_button,INPUT);
     pinMode(skip_button,INPUT);
     pinMode(start_now_button,INPUT);
      
     
     pinMode(pump_songle,OUTPUT);
     pinMode(transformer_songle,OUTPUT);
     for(a=0;a<zones;a++){
       pinMode(2+a,OUTPUT);
     }
     
     auto_on=false;
     change=true;
     date_set=false;

          started=false;

         Next_hour=18;
         Next_minute=1;

         screen_on=false;
          lcd.setBacklight(LOW);
}

/**
 * This is only run when you want to change the set time  Do this once for the program when you setup a new timer board.
 */
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
/** This function displays it onthe serial terminal
void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}

*/
void loop()
{
  //displayTime(); // display the real-time clock data on the Serial Monitor,

  check_buttons();

  check_start_end();
  
  show_leds();
  switch_songles();
  
  if(auto_on==true){
      read_delay_potentio();
    }
  if(date_set==true){
      read_date_potentio();
    }

 
  
 // delay(1000); // every second
 
  show_time_lcd();
  if(screen_changed==true){ delay(500); screen_changed=false;}
  if(change==true){ delay(500); change=false;}
}



void show_time_lcd(){

   byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);


  if(screen_changed==true){
     if(screen_on==true)
       lcd.setBacklight(HIGH);
     else
       lcd.setBacklight(LOW);

    
  }



  if(minute!=old_min){
                        lcd.clear();
                          lcd.setCursor(0,0);
                          lcd.print(Day(dayOfWeek));
                          lcd.print(" ");
                          if(hour<10)
                            lcd.print("0");
                          lcd.print(hour,DEC);
                          lcd.print(":");
                          if(minute<10)
                             lcd.print("0");
                          lcd.print(minute,DEC);
    //   if(auto_on==true)          auto_on=false; else auto_on=true;
   old_min=minute;
   }
               lcd.setCursor(0,1);
               lcd.print("Duration: ");
               lcd.print(water_delay);  lcd.print("  ");

               lcd.setCursor(0,2);
               lcd.print("Next: ");
               if(Next_hour<10)
               lcd.print("0");
               lcd.print(Next_hour);
               
               lcd.print(":");
               if(Next_minute<10)
               lcd.print("0");
               lcd.print(Next_minute);
               lcd.print("  ");

      lcd.setCursor(0,3);
    
if(started==true){
          lcd.print("Zone ");
          lcd.print(current_zone);
          lcd.print(" Until: ");
          if(zone_next_hour<10)            lcd.print("0");
          lcd.print(zone_next_hour);
          lcd.print(":");
          if(zone_next_minute<10) lcd.print("0");
          lcd.print(zone_next_minute);
}else{
      lcd.print("Off                 "); 
}


}


void switch_songles(){


int a;

    if(started==true){
           // make sure pump is on and transformer too
           
                  digitalWrite(pump_songle,LOW);
                  digitalWrite(transformer_songle,LOW);

                  digitalWrite(current_zone+1,LOW);

                 for(a=0;a<zones;a++){  //make sure the rest remain off.
                  if(!( (current_zone-1)==a)){
                  digitalWrite(a+2,HIGH);
                 }
                 }

    }else{

                  digitalWrite(pump_songle,HIGH);
                  digitalWrite(transformer_songle,HIGH);
                 for(a=0;a<zones;a++){
                  digitalWrite(a+2,HIGH);
                 }



    }
           

      
    }
  




String Day(int day){
switch(day){
  case 1:
    return "Sunday";
    break;
   
  case 2:
    return"Monday";
    break;
  case 3:
    return"Tuesday";
    break;
  case 4:
    return"Wednesday";
    break;
  case 5:
    return "Thursday";
    break;
  case 6:
    return "Friday";
    break;
  case 7:
    return "Saturday";
    break;
  }


return "Error";
}

/**
 * See if anything has been pushed
 */
void check_buttons(){

int check;
     check=digitalRead(auto_set_button);
   //  Serial.println(check);
    if(check==HIGH){ // it has  been pressed
        change=true;
        if(auto_on==true){ auto_on=false;} else{ auto_on=true; date_set=false;}
    }

   check=digitalRead(date_set_button);
  //   Serial.println(check);
    if(check==HIGH){ // it has  been pressed
        change=true;
        if(date_set==true){ date_set=false;} else{ date_set=true; auto_on=false;}
    }


  check=digitalRead(screen_button);
    if(check==HIGH){
      if(screen_on==true){ screen_on=false;} else{ screen_on=true;}
       screen_changed=true;
    }
  

  check=digitalRead(skip_button);
    if(check==HIGH){
      if(started==true){
        next_zone();
      //  old_min-1;  // force a refresh.
      change=true;
    }
}
  check=digitalRead(start_now_button);
    if(check==HIGH){
      if(started==false){
        start_cycle();
      change=true;
    }
}


}


void show_leds(){
  if(auto_on==true) {
     digitalWrite(auto_set_led,LOW);
  }else{
     digitalWrite(auto_set_led,HIGH);
  }

if(date_set==true){
  digitalWrite(date_set_led,LOW);
}else{
  digitalWrite(date_set_led,HIGH);
  }


}



void read_delay_potentio(){
  int check;

  check=analogRead(A0);

  //   Serial.println(check);
  water_delay=check/18;

if(water_delay<1) water_delay=1;

}
void read_date_potentio(){
  int check;
  int a,b,c;

  
  check=analogRead(A0);
  check*=2;
  for(c=0,a=0;c<check && a<24;a++){
    for(b=0;c<check&& b<60;b++,c++){
    
    }
    }
  Next_minute=b;
  if(Next_minute>59) Next_minute=59;
  Next_hour=a;
    if(Next_hour==24) Next_hour=0;
  }
 

void check_start_end(){
   byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);

  if(started==false){
     if(Next_minute==minute && Next_hour==hour){
        start_cycle();
     }
    
  }else{ // its busy 
   if(zone_next_hour==hour && zone_next_minute==minute){
     next_zone();
   }
  
  }

  
}

void start_cycle(){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);

  
  started=true;
      current_zone=1;
      add_zone_time(hour,minute);
 
}

void next_zone(){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);


  
  current_zone++;
    if(current_zone>zones){
      started=false;
      }else{
        add_zone_time(hour,minute);
      }
  
  
}

void add_zone_time(int hour, int minute){
  int a;
  zone_next_hour=hour;
  zone_next_minute=minute;
  
  for(a=0;a<water_delay;a++){
    zone_next_minute++;
    if(zone_next_minute==60){
      zone_next_hour++;
      zone_next_minute=0;
      if(zone_next_hour==24){
        zone_next_hour=0;}
    }
  }
}
