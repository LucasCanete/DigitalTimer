/*On this project I create a digital Clock that displays the current date on a lcd device.
  Additionally there is a joystick that when pressed for the firt time, takes us to a menu
  in the lcd where the user decides whether to change the current date or to create an event.
  Using the joystick the user selects an option, in the first case changing the date and time to 
  be displayed and in the second case (create event) creating an alarm that will trigger PIN 13 
  at a given time for a specific time frame. As an example, the user can programm the device to trigger
  PIN 13 at 15:00 until 16:30 to, lets say, automatically irrigating his garden at that time.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
LiquidCrystal_I2C lcd(0x27,16,2); //for UNO and Nano 0x27 

RTC_DS3231 rtc;

const uint8_t button = 7;
const uint8_t y_key = A1;
const uint8_t x_key = A2;
uint8_t menu = 1;

int start_hour = 0;
int start_minute = 0;

int end_hour = 0;
int end_minute = 0;

int updt_hour = 0;
int updt_minute = 0;
int updt_day = 1;
int updt_month = 1;
int updt_year = 2019;

void setup() {
  
  lcd.init();
  lcd.backlight();
  pinMode(button,INPUT_PULLUP);
  pinMode(13,OUTPUT);
  
  if (! rtc.begin()) {
    lcd.print("Couldn't find RTC");
    
  }
  Serial.begin(9600);
}

void loop() {

  display_time();
  
  check();
  int button_state = digitalRead(button);

  if (button_state == LOW){
      enter_menu();
    }
 }

void enter_menu(){
  
      lcd.clear();
      update_menu();
      
      while(1){
        
          int y_val = analogRead(y_key);
          int x_val = analogRead(x_key);
          int map_y = map(y_val,0,1023,-512,512);
          int map_x = map(x_val,0,1020,-512,512);
          
          if(map_y <= -410){
              menu++;
              update_menu();
              delay(400);
    
          }
         else if (map_y >= 450)
              menu--;
              update_menu();
              delay(400);
         }
        else if (map_x>= 450){
            if (menu == 5){
                menu = 1;
                break;
              }
              
              update_submenu();
              
              delay(400);
              menu = 1; 
              break;
          
          }
       else if (map_x <= -410){
              
              update_menu();
              delay(400);
              
        }

         }  
  }


void update_menu(){
  
  switch(menu){

      case 0:
          menu = 1;
          break;
    
      case 1:
      
          lcd.clear();
          lcd.print(">Begin Event ");
          lcd.setCursor(0,1);
          lcd.print(" Finish Event");
          break;


      case 2:
      
          lcd.clear();
          lcd.print(" Begin Event");
          lcd.setCursor(0,1);
          lcd.print(">Finish Event");
          break;

      case 3:
      
          lcd.clear();
          lcd.print(" Finish Event");
          lcd.setCursor(0,1);
          lcd.print(">Delete Event");
          break;

       
      case 4:
      
          lcd.clear();
          lcd.print(" Delete Event");
          lcd.setCursor(0,1);
          lcd.print(">Update Time");
          break;
          
      case 5:
          lcd.clear();
          lcd.print(" Update Time");
          lcd.setCursor(0,1);
          lcd.print(">Exit");
          
          break;

      case 6: 
          
          menu = 5;
          break;
    
    }
  
  
}

void display_time(){
  
     DateTime now = rtc.now();
     lcd.setCursor(0,0);
     lcd.print("Date:");
     lcd.print(now.day());
     lcd.print("/");
     lcd.print(now.month());
     lcd.print("/");
     lcd.print(now.year());
     lcd.setCursor(0,1);
     lcd.print("Time: ");
     lcd.print(now.hour());
     lcd.print(":");
     if(now.minute()<10){
        lcd.print("0");
        lcd.print(now.minute());
      }
     else{
        
        lcd.print(now.minute());
        
        }
    lcd.print(":");
    if (now.second()<10){
          lcd.print("0");
      }
    lcd.print(now.second());
  
  }

void update_submenu(){

  switch(menu){
    
        case 1:
            lcd.clear();
            
            begin_event();
            break;

        case 2:
        
          lcd.clear();
          end_event();
          break;

        case 3:
        
          lcd.clear();
          start_hour = 0;
          start_minute = 0;
          end_hour = 0;
          end_minute = 0;
          
          break;

        case 4:
          lcd.clear();
          update_time();
          break;             
    
    }
  
 }

void begin_event(){

     lcd.print("Begin Time");
     int y_val;
     int map_y;
     int but_state;
    while(1){
 
       y_val = analogRead(y_key);
       map_y = map(y_val,0,1023,-512,512);
       but_state = digitalRead(button);

    
       lcd.setCursor(0,1);
       
       lcd.print("Hour: ");
        if (start_hour < 10){
          
            lcd.print("0");
          }
     
       lcd.print(start_hour);
      
       set_hour(map_y,start_hour);
       
      if (but_state == LOW){ 
     
          delay(400); //cleans input
          break;
        
        }
      
    }
      
    
   
  while(1){
    
       y_val = analogRead(y_key);
       map_y = map(y_val,0,1023,-512,512);
       but_state = digitalRead(button);
       

       
       lcd.setCursor(0,1);

       lcd.print("Minute: ");
        if (start_minute < 10){
          
            lcd.print("0");
          }
     
       lcd.print(start_minute);
       set_minute(map_y,start_minute);
       if (but_state == LOW){ 
         
          break;
        
        }
    
    }
  
  }

void end_event(){
  
     int y_val;
     int map_y;
     int but_state;
    
    lcd.print("End Time");
    while(1){
 
       y_val = analogRead(y_key);
       map_y = map(y_val,0,1023,-512,512);
       but_state = digitalRead(button);
   
       
       lcd.setCursor(0,1);

       lcd.print("Hour: ");
        if (end_hour < 10){
          
            lcd.print("0");
          }
     
       lcd.print(end_hour);
       set_hour(map_y,end_hour);
      if (but_state == LOW){ 
     
          delay(400); //cleans input
          break;
        
        }
      
    }
      
    
   
  while(1){
    
       y_val = analogRead(y_key);
       map_y = map(y_val,0,1023,-512,512);
       but_state = digitalRead(button);
      
       
       lcd.setCursor(0,1);

       lcd.print("Minute: ");
        if (end_minute < 10){
          
            lcd.print("0");
          }
     
       lcd.print(end_minute);
       set_minute(map_y,end_minute);
       if (but_state == LOW){ 
         
          break;
        
        }
    
    }
  
  }
void update_time(){

  lcd.print("Update Time");
  int y_val;
  int map_y;
  int but_state;
  while(1){
    
       y_val = analogRead(y_key);
       map_y = map(y_val,0,1023,-512,512);
       but_state = digitalRead(button);
   
       lcd.setCursor(0,1);

       lcd.print("Hour: ");
        if (updt_hour < 10){
          
            lcd.print("0");
          }
     
       lcd.print(updt_hour);
       set_hour(map_y,updt_hour);
      if (but_state == LOW){ 
     
          delay(400); //cleans input
          break;
        
        }
   
    }

   while(1){
    
       y_val = analogRead(y_key);
       map_y = map(y_val,0,1023,-512,512);
       but_state = digitalRead(button);
   
       lcd.setCursor(0,1);

       lcd.print("Minute: ");
        if (updt_minute < 10){
          
            lcd.print("0");
          }
     
       lcd.print(updt_minute);
       set_minute(map_y,updt_minute);
      if (but_state == LOW){ 
     
          delay(400); //cleans input
          break;
        
        }
    }
    lcd.clear();
    while(1){
      
       y_val = analogRead(y_key);
       map_y = map(y_val,0,1023,-512,512);
       but_state = digitalRead(button);
       lcd.setCursor(0,0);
       lcd.print("Update Time");
       lcd.setCursor(0,1);
       lcd.print("Date: ");
       if (updt_day < 10){
            lcd.print("0");
          }
       lcd.print(updt_day);
       set_day(map_y,updt_day);
        if (but_state == LOW){ 
     
          delay(400); //cleans input
          break;
        
        }
      
      }

    while(1){
        y_val = analogRead(y_key);
        map_y = map(y_val,0,1023,-512,512);
        but_state = digitalRead(button);

       lcd.setCursor(0,1);
       lcd.print("Month: ");
       if (updt_month < 10){
          
            lcd.print("0");
          }
       lcd.print(updt_month);
       set_month(map_y,updt_month);
        if (but_state == LOW){ 
     
          delay(400); //cleans input
          break;
        
        }
      }
    while(1){
       y_val = analogRead(y_key);
       map_y = map(y_val,0,1023,-512,512);
       but_state = digitalRead(button);

       lcd.setCursor(0,1);
       lcd.print("Year: ");
       
       lcd.print(updt_year);
       set_year(map_y,updt_year);
        if (but_state == LOW){ 
     
          delay(400); //cleans input
          break;
        
        }
      
      }
 rtc.adjust(DateTime(updt_year,updt_month,updt_day,updt_hour,updt_minute, 0));
  
  
  
  
  }
 void set_hour(int stick_x,int& event_hour){
  
  if(stick_x >= 450) {
    delay(300);
    event_hour++;
  }
    
  else if(stick_x <= -410){
    delay(300);
    event_hour--;
    }
    
  if (event_hour == 24){
    
    event_hour = 0;}
    
  else if (event_hour < 0)event_hour = 23;

  
    
     }
void set_minute(int stick_x,int& event_minute){
  
  if(stick_x >= 450){
    delay(300);
    event_minute++;}
  else if(stick_x <= -410){
    delay(300);
    event_minute--;}
  if(event_minute == 60)event_minute = 0;
  else if(event_minute < 0)event_minute = 59;
  
  
  }
void set_year(int stick_x,int& the_year){  
  if(stick_x >= 450){
    delay(300);
    the_year++;}
  else if(stick_x <= -410){
    delay(300);
    the_year--;}
  if (the_year < 2019)the_year = 2060;
  else if(the_year > 2060)the_year = 2019;
   
  }
  void set_month(int stick_x,int& the_month){
    
    if(stick_x>=450){
      delay(300);
      the_month++;}
    else if(stick_x<=-410){
      delay(300);
      the_month--;}
    if (the_month<1)the_month = 12;
    else if(the_month>12)the_month = 1;
    
    
    }
  void set_day(int stick_x,int& the_day){
    
    if(stick_x >= 450){
      delay(300);
      the_day++;}
    else if(stick_x <= -410){
      delay(300);
      the_day--;}
    if (the_day < 1)the_day = 31;
    else if(the_day > 31)the_day = 1;
    
    }

  void check(){
  //checks whether out current time matches the time of the event
  DateTime now = rtc.now();
  if (now.hour() == start_hour && now.minute() == start_minute) {
    Serial.println("ON");
    digitalWrite(13,HIGH);
  }
  else if (now.hour() == end_hour && now.minute() == end_minute){ 
    digitalWrite(13,LOW);
    Serial.println("OFF");
    }
  }



    
  
