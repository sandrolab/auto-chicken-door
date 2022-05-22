#include <Servo.h>

Servo servoDoor;
Servo servoLock; 

const int ledRed = 6;
const int ledGreen = 7;

int luceMin = 0; // minimum light value
int luce[5]; // array for sampling data
int luceMedia = 0;

const int buzzer = 9;

int pos = 0;
int pos2 = 0;
const int posMin = 0; // minumum angle
const int posMax = 125; // maximum angles (0° - 180°)
const int posMax2 = 130;
const int velBlocco = 10; // lock speed
const int velPortaApri = 12; // opening speed
const int velPortaChiudi = 12; // closing speed

int doorState = 0;

void setup() {
  
  servoDoor.attach (3); 
  servoLock.attach (4); 
  pinMode (ledGreen, OUTPUT);
  pinMode (ledRed, OUTPUT);
  pinMode (buzzer, OUTPUT);

  // MOTORS INITIALISATION AND RESET
  servoLock.write (posMax2);  
  servoDoor.write (posMin); 
  delay(2000);
  servoLock.write (pos2); 
  delay(2000);
  tone (buzzer,1500,350);
  delay (500);
  digitalWrite (ledRed, HIGH);
  
}

void loop() {
  
      // ------------- SAMPLING AND AVERAGE CALCULATION -------------
      
      for (int i = 0; i < 5; i++) { 
          luce[i] = analogRead (A0); 
          delay(2000);
      } 
      for (int i = 0; i < 5; i++) {
          luceMedia += luce[i]; 
      }
      luceMedia = luceMedia / 5;

      // ------------- SUNRISE (OPENING) -------------

      if (luceMedia > luceMin && doorState == 0) { 
        
          // 1) Beep 3 times
          for (int x = 0; x < 3; x++) {
              tone (buzzer,3000,200);     
              delay (1000);
          }
          
          // 2) Open the lock
          while (pos2 <= posMax2) {
              for (pos2 = posMin; pos2 <= posMax2; pos2 += 1) {
                  servoLock.write (pos2);
                  delay (velBlocco);
              }
          }
          delay (2000);
          
          // 3) Open the door
          while (pos <= posMax) {
              for (pos = posMin; pos <= posMax; pos += 1) {
                  servoDoor.write (pos);          
                  delay (velPortaApri);
              } 
          }
          
          // 4) Turn on green, turn off red
          digitalWrite(ledGreen, HIGH); 
          digitalWrite(ledRed, LOW); 
          doorState = 1;
      }

      // ------------- SUNSET (CLOSING) -------------
      
      else if (luceMedia <= luceMin && doorState == 1) {
        
          // 1) Alert and wait
          tone (buzzer,2150,200); // detection tone
          digitalWrite(ledRed, HIGH); // turn on the green light
          delay (1200000); // wait between sampling and closing (20m = 1200000)
          
          // 2) Beep 5 times
          for (int x = 0; x < 5; x++) {
              tone (buzzer,3000,200);     
              delay (500);
          }
          
          // 3) Close the door
          while (pos >= posMin) {
              for (pos = posMax; pos >= posMin; pos -= 1) {
                  servoDoor.write (pos);
                  delay (velPortaChiudi); 
              }  
          } 
          delay (2000); 
          
          // 4) Close the lock
          while (pos2 >= posMin) {
              for (pos2 = posMax2; pos2 >= posMin; pos2 -= 1) {
                  servoLock.write (pos2);
                  delay (velBlocco);
              }
          }
          
          // 5) Turn on red, turn off green
          digitalWrite (ledRed, HIGH);
          digitalWrite(ledGreen, LOW); 
          doorState = 0;  
          
      } 
      
delay (10);

}
