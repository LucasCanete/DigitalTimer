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

int x_key = A1; // Pin to read movement from the joystick

int button = 7;  // Joystick button works as ok button in this project

int x_val;


int hora=0;
int minuto=0;


int end_hora=0;
int end_minuto=0;


 
void setup() {

  lcd.init(); //turn on lcd
  lcd.backlight(); //turn on backlight
  pinMode(13,OUTPUT); 
  pinMode(button,INPUT_PULLUP);
  if (! rtc.begin()) {
    lcd.print("Couldn't find RTC");
    
  }
  //rtc.adjust(DateTime(2020, 05, 17, 13, 54, 0));
 
}

void loop() {
  
 //User sees date and time until he presses the joystick button to enter the menu 
  
  if(digitalRead(button) == LOW){
      delay(900);   
      menu();
    }
   check(); //check whether the current time is equal to the time user chose to make event
   actual_time(); // displays current date and time
  delay(200);
}
void actual_time(){
      //displays current time and date
      lcd.clear();
    
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
     lcd.print(now.minute());
      delay(500);
    }
void check(){
  //checks whether out current time matches the time of the event
  DateTime now = rtc.now();
  if (now.hour() == hora && now.minute() == minuto) digitalWrite(13,HIGH);
  else if (now.hour() == end_hora && now.minute() == end_minuto) digitalWrite(13,LOW);
  
} 
  void menu(){
      lcd.clear();
      lcd.print("     *MENU*");
      lcd.setCursor(0,1);
      lcd.print("   OK TO EXIT"); //if user presses joystick button in the first 3 seconds he goes out of the menu
      delay(3000);
      
      while(1){
        lcd.clear();
        lcd.print("Adjust Time");
        lcd.setCursor(0,1);
        lcd.print("Make Event");
        if (digitalRead(button) == LOW){
          delay(600);
          break;
          }
        x_val = analogRead(x_key);
        int map_x = map(x_val,0,1023,-512,512);
        if(map_x <= -410){
          lcd.clear();
          lcd.print("Adjust Time");
          lcd.setCursor(0,1);
          lcd.print("Make Event   *");
          delay(1500);
          make_event(); //set time to begin the automation
          end_event(); //set time to finish automation
          break;
          }
         else if(map_x >= 490){
            lcd.clear();
            lcd.print("Adjust Time  *");
            lcd.setCursor(0,1);
            lcd.print("Make Event  ");
            delay(1500);
            adjust_time(); // adjusts current time
            break;
          }
      
        
       delay(400); }
    
    }


    
//the following functions are for when user chooses to make an event for the automation

void make_event(){
    
    
    while(1){
      //We first set the hour the event will occur
        lcd.clear(); 
        lcd.print("  *MAKE EVENT*  ");
        
        if(digitalRead(button) == LOW){
            delay(900);
            break;}
            
        x_val = analogRead(x_key);
        int map_x = map(x_val,0,1023,-512,512);
        set_hour(map_x,hora); 
       
        lcd.setCursor(0,1);
        lcd.print("Set hour:");
        lcd.print(hora);
                                                                                  
        delay(300);
        
      }
   
    while(1){
      //We then set at what minute will this event occur
        lcd.clear();
        lcd.print("  *MAKE EVENT*  ");
        if(digitalRead(button) == LOW){
          
          delay(900);
          break;}
        x_val = analogRead(x_key);
        int map_x = map(x_val,0,1023,-512,512);
        set_minute(map_x,minuto);
        lcd.setCursor(0,1);
        lcd.print("Set minute:"); 
        lcd.print(minuto);
        delay(300);
      }
    lcd.clear();
    lcd.print("Saving...");
    delay(1000);
    lcd.clear();
    
    }

 void end_event(){
    //Set the time at which the event will end
    
    while(1){
      //Set the hout the event will end
        lcd.clear(); 
        lcd.print("  *END EVENT*  ");
        
        if(digitalRead(button) == LOW){
            delay(900);
            break;}
            
        x_val = analogRead(x_key);
        int map_x = map(x_val,0,1023,-512,512);
        set_hour(map_x,end_hora); 
       
        lcd.setCursor(0,1);
        lcd.print("Set hour:");
        lcd.print(end_hora);
                                                                                  
        delay(300);
        
      }
   
    while(1){
      //Set the minute the event will end
        lcd.clear();
        lcd.print("  *END EVENT*  ");
        if(digitalRead(button) == LOW){
          
          delay(900);
          break;}
        x_val = analogRead(x_key);
        int map_x = map(x_val,0,1023,-512,512);
        set_minute(map_x,end_minuto);
        lcd.setCursor(0,1);
        lcd.print("Set minute:"); 
        lcd.print(end_minuto);
        delay(300);
      }
    lcd.clear();
    lcd.print("Saving...");
    delay(1000);
    lcd.clear();
    
    }

//Following functions are for setting the time for example at which an event will occur and then finish

void set_hour(int stick_x,int& event_hour){
  
  if(stick_x >= 490) event_hour++;
    
  else if(stick_x <= -410) event_hour--;
    
  if (event_hour == 24)event_hour = 0;
    
  else if (event_hour < 0)event_hour = 23;
    
     }
void set_minute(int stick_x,int& event_minute){
  
  if(stick_x >= 490)event_minute++;
  else if(stick_x <= -410)event_minute--;
  if(event_minute == 60)event_minute=0;
  else if(event_minute < 0)event_minute=59;
  
  
  }

 void adjust_time(){
    //adjusts the current date and time
    // Current date that will be displayed on lcd. Values can be changed
    int the_year = 2019;  
    int the_month = 1;
    int the_day = 1;
    int the_hour = 1;
    int the_minute = 1;
    while(1){

      if(digitalRead(button) == LOW){
          delay(900);
          break;
        }

      lcd.clear();
      lcd.print(" *ADJUST TIME* ");
      
      x_val = analogRead(x_key);
      int map_x = map(x_val,0,1023,-512,512);
      updt_day(map_x,the_day);
      
      lcd.setCursor(0,1);
      lcd.print("day:");
      lcd.print(the_day);
     
      
      delay(300);
      }

      while(1){

          if(digitalRead(button) == LOW){
          delay(900);
          break;
            }
        
          lcd.clear();
          lcd.print(" *ADJUST TIME* ");

          lcd.setCursor(0,1);
          lcd.print("month:");
          lcd.print(the_month);

          x_val = analogRead(x_key);
          int map_x = map(x_val,0,1023,-512,512);
          updt_month(map_x,the_month);
        
        
        
          delay(300);
        }

      while(1){
          if(digitalRead(button) == LOW){
              delay(900);
              break;
            }
      
         lcd.clear();
         lcd.print(" *ADJUST TIME* ");

         lcd.setCursor(0,1);
         lcd.print("year:");
         lcd.print(the_year);

         x_val = analogRead(x_key);
         int map_x = map(x_val,0,1023,-512,512);
         updt_year(map_x,the_year);
        
        
         delay(300);
        }


      while(1){
        
          if(digitalRead(button)==LOW){
              delay(900);
              break;
            }
      
         lcd.clear();
         lcd.print("**Adjust Time**");

         lcd.setCursor(0,1);
         lcd.print("hour:");
         lcd.print(the_hour);

         x_val = analogRead(x_key);
         int map_x = map(x_val,0,1023,-512,512);
         set_hour(map_x,the_hour);
        
         delay(300);
        
        
        }

     while(1){
      
         if(digitalRead(button) == LOW){
             delay(900);
             break;
            }
      
         lcd.clear();
         lcd.print("**Adjust Time**");

         lcd.setCursor(0,1);
         lcd.print("minute:");
         lcd.print(the_minute);

         x_val = analogRead(x_key);
         int map_x = map(x_val,0,1023,-512,512);
         set_minute(map_x,the_minute);
        
         delay(300);
      
      
      }
    lcd.clear();
    rtc.adjust(DateTime(the_year,the_month,the_day,the_hour,the_minute, 0));
    lcd.print("OK***");
    delay(1000);
    
    }


//functions to update date
  
  void updt_year(int stick_x,int& the_year){
  
  if(stick_x >= 490)the_year++;
  else if(stick_x <= -410)the_year--;
  if (the_year < 2019)the_year = 2060;
  else if(the_year > 2060)the_year = 2019;
   
  }
  void updt_month(int stick_x,int& the_month){
    
    if(stick_x>=490)the_month++;
    else if(stick_x<=-410)the_month--;
    if (the_month<1)the_month = 12;
    else if(the_month>12)the_month = 1;
    
    
    }
  void updt_day(int stick_x,int& the_day){
    
    if(stick_x >= 490)the_day++;
    else if(stick_x <= -410)the_day--;
    if (the_day < 1)the_day = 31;
    else if(the_day > 31)the_day = 1;
    
    }




    
  
