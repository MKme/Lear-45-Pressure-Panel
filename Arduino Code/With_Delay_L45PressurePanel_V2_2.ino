/*
Youtube : http://www.youtube.com/user/Shadow5549
Learjet 45 series home cockpit build videos can be found on my channel
This is my version of Nokia 5110 LCD displaying FSX Data
Final result to be used for Learjet 45 Home Cockpit Cabin Pressure Panel
The nokia 5110 display will have backlight turned off and contrast set to nil if avionics master switch not turned on
Cabin Rate- This is actual clim/descend unless above the limits above the actual Lear systems
Delta Pressure- Calculated by altitude linear extrapolation
Cabin Alt- Actual altitude unless above limits of actual Lear systems
Landing Alt- This is set by the user and has no effect on FSX
Manual Cabin Rate- this is set by the user and has no effect on FSX- This could be linked to the pushbutton later to show "M" and
alter the cabin rate display to show the manual selection
Delay of 5 seconds after manual adjustments as per the real Lear 45

     Original comments from example sketch:
    This code is in the public domain
    For use with "Link2fs_Multi_beta5d" or later"
    Jimspage.co.nz
    My thanks to the Guys that gave me snippets of code. 
    
    
    Encoder Readme:
    This sets the Arduino Mega card for 3 rotary encoders and "keys" input except pin 13.
    Attach a switch to any pin from pin 8 upwards (except 13) and program that pin in "Multi"
    Everything to do with "Keys"  starts with a "K" in this code.
    Everything to do with rotary encoders starts with a "R" in this code.
    The Quadrature2 library is available from my site Jimspage.co.nz. (Special thanks to Bill Holland)
    It's here ,, http://www.jimspage.co.nz/encoders2.htm (Near the bottom of the page)
    Rotary encoder 1 is on pins 2 and 3
    Rotary encoder 2 is on pins 4 and 5
    Rotary encoder 3 is on pins 6 and 7
    The "push" switch on each one is connected to a "Keys" input pin and just seen as a switch.
    
 Aug 2015 V2.2 Added contrast int
*/
#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"
#include "math.h" //For Encoders
#include "Quadrature.h"  //For Encoders
Quadrature quad1(9, 8); //Set Encoder Pins
Quadrature quad2(12, 10); //Set Encoder Pins
//Quadrature quad3(6, 7);  //commented out because encoder 3 not yet used
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);//4 and 3 are reversed on the cheap eBay models

//FROM LINK2FS-----------------------------------------
int CodeIn;// used on all serial reads
int KpinNo; 
int Koutpin;
String KoldpinStateSTR, KpinStateSTR, Kstringnewstate,Kstringoldstate;
//FROM LINK2FS----------------------------------------

int Contrast = 35;//  Set your contrast here!----------------------------------------------
int LED = 225; // Set the brightness of your backlight LED's ------------------------------
//Max value is 255 and min is 0.  Around 220 is about normal for mine.  Warning- full current flow can be beyond the capability of the Arduino pin!

String alt;// Added this for altitude calc & display E
String vs;// Added this for vs calc & display E
String avm;// Added this for Avionics Master- Ability to turn off LCD display if avionics not on
String alt2;//
String cabinrate;
int landalt =7;  // For encoder landing alt
int la;  // For encoder landing alt
int manualrate =1;  //for encoder manual pressure rate
int mr;  //for encoder manual pressure rate

//encoders variables
int R;// a variable
int Rold;// the old reading
int Rdif;// the difference since last loop
int R2;// a second test variable
int R3;// a second test variable
int Rold2;// a second loop old reading
int Rdif2; // the second test difference
int Rold3;// a second loop old reading
int Rdif3; // the second test difference
//Time setups for screen switch delay functionality
unsigned long time;
unsigned long time1;


void setup() {
  
  Serial.begin(115200); // need to enable serial com
   analogWrite(11,LED);// PWM of LCD backlight but ebay unit is backwards- 
  //must go high # + cycle to dim (255 limit)
  //Very Dim=230
  
  display.begin();//Display code
 
  display.setContrast(Contrast);//Nokia 5110 works best at 50- no more flicker
  delay(1000);
  display.clearDisplay();     // clears the screen and buffer
  display.setTextSize(1);     // set text size
  display.setTextColor(BLACK);
  //delay(1000);
  
  //Splash Screen----------------------------------------------------------------------------------------------
   // Splash screen for initial startup
  display.setTextSize(1);// Set text size
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println(" Bombardier");
  display.println(" Learjet 45");
  display.println("Cabin Pressure");
  display.println("Initializing..");
  display.println("");
  display.display();
  
  //Removed Delay and clearscreen here- Now splash screen will remain untill Link2fs sends a value for speed or ALT.
  //Sometimes Link2fs can have a fairly large delay when on ground.  Set card to cycle time 200 M/s and refresh of 10 seconds seems to work
  //Splash Screen----------------------------------------------------------------------------------------------



}

void loop() {
  
time = millis(); // Sets "time" to current system time count
{ENCODER();} //Check the Rotary Encoders
  if (Serial.available()) {
    CodeIn = getChar();
    if (CodeIn == '=') {EQUALS();} // The first identifier is "="
    if (CodeIn == '<') {LESSTHAN();}// The first identifier is "<"
    if (CodeIn == '?') {QUESTION();}// The first identifier is "?"
    if (CodeIn == '/') {SLASH();}// The first identifier is "/" (Annunciators)
  }
 
}

char getChar()// Get a character from the serial buffer
{
  while(Serial.available() == 0);// wait for data
  return((char)Serial.read());// Thanks Doug
}

void EQUALS(){      // The first identifier was "="
 CodeIn = getChar(); // Get another character
  switch(CodeIn) {// Now lets find what to do with it
    case 'A'://Found the second identifier
       //Do something
    break;
     
    case 'B':
       //Do something
    break;
     
    case 'C':
       //Do something
    break;
    
         //etc etc etc
     }

}

  void LESSTHAN(){    // The first identifier was "<"
CodeIn = getChar(); // Get another character
  switch(CodeIn) {// Now lets find what to do with it
  
    //-----------------------Functions to turn off Cabin Pressure if Avioincs is off and initialise on startup--------------------
    case 'g'://Avionics Master Switch
     {
   avm = "";
     avm += getChar();
     int avm1 = avm.toInt(); // convert it to an integer (Thanks Phill)
     if (avm1 < 1)
     {
     display.setContrast(1);   //Avionics is off- Turn off LCD by setting contrast to nil
     analogWrite(11,255);      //Avionics is off- Turn off LCD Backlight
        }
   else if (avm1 > 0)
   {
     display.setContrast(Contrast);//Avionics is ON- turn on LCD by setting contrast up to visible
      analogWrite(11,LED);   //Avionics is ON- turn on LCD Backlight
             
   }
   
   //-----------------------Functions to turn off Cabin Pressure if Avioincs is off and initialise on startup  (END)--------------------
     
    }
     
    break;
     
    case 'B':
       //Do something
    break;
     
// Display Vertical Speed------------------------------------------------------------------------------------------------ 

   case 'L':
  { 
    {
    vs = "";
    vs += getChar();
    vs += getChar();
    vs += getChar();
    vs += getChar();
    vs += getChar();
    vs += getChar();
    int vs1 = vs.toInt(); // convert it to an integer (Thanks Phill)
     
 display.clearDisplay();   // clears the screen and buffer
 display.setCursor(0,0);
 display.setTextSize(2);  // Set text size
 display.print (" "); // Adds some spaces to move text right
 
 if (vs1 >0)               // Display up arrow of positive rate of climb
{
display.print ("^ ");       // Display up arrow of positive rate of climb
}
else 
{display.print (" ");        // Offsets one space right to maintain alignment
 }
 
 if (vs1 > 600) 
 {
   display.println ("600");// These are actual values from the Lear 45
 }
 else if (vs1 < -375) 
 {
   display.println ("-375");// These are actual values from the Lear 45
 }
 
 else
display.println(vs1);
  }
  }
   break;
    // Display Vertical Speed End------------------------------------------------------------------------------------------------   
    
    
    case 'D':
    {
     alt = "";  // Get altitude
     alt += getChar();
     alt += getChar();
     alt += getChar();
     alt += getChar();
     alt += getChar();
  unsigned int alt1 = alt.toInt(); // convert it to an integer NOTE USE UNSIGNED INT to double int capacity for only + numbers
  float cabindelta = (alt1 * 0.00018); // This calculates tha cabin delta (for use on Line 2 below) from max at aircraft ceiling- used float for big#
 
 
 // Display Cabin delta on LCD Line 2-------------------------------------------------------------------------------------------
  // need to add weight on wheels function to zero this value otherwie it will show delta > 0 when on ground *****
  display.print("  ");// Adds some spaces to move text right
  display.println (cabindelta, 2);// Shows the cabin delta with 2 decimal places
 // Display Cabin delta on LCD Line 2-------------------------------------------------------------------------------------------
   
     
// Display Cabin Alt to LCD but Cap at 8000-------------------------------------------------------------------------------------
     display.print ("  ");// Adds some spaces to move text right
     {
       if (alt1 < 8000) 
      { 
       display.println (alt1);
       display.display();
       // delay(100);  Removed to troubleshoot encoder delay problems
      }
        
     else if (alt1 > 8000)
     {
      
      display.println ("8000");
      display.display();
       // delay(100);  Removed to troubleshoot encoder delay problems
     }
        
     }
     }
  break;
// Display Cabin Alt to LCD but Cap at 8000 END ------------------------------------------------------------------------------------- 

   
}
  }
void QUESTION(){    // The first identifier was "?"
CodeIn = getChar(); // Get another character
  switch(CodeIn) {// Now lets find what to do with it
    case 'A'://Found the second identifier
       //Do something
    break;
     
    case 'B':
       //Do something
    break;
     
    case 'C':
       //Do something
    break;
       
         
     }
}
void SLASH(){    // The first identifier was "/" (Annunciator)
  //Do something
  
} 
  
  void ENCODER(){
 ENCODER:
 time = millis();
  R =(quad1.position()/1); //The /2 is to suit the encoder(See my website)
  if (R != Rold) { // checks to see if it different
   time1 = millis(); //Resets Time1 each time there is an encoder change- for screen timer
   (Rdif = (R-Rold));// finds out the difference
   if (Rdif == 1) (la= (landalt++)*25);  //updates landing alt by multiples- change this if user likes a different interval
   if (Rdif == -1) (la= (landalt--)*25);    //updates landing alt by multiples- change this if user likes a different interval
  // Serial.println (la); //send to serial for troubleshooting
   
// Display Landing Altitiude   
 display.clearDisplay();   // clears the screen and buffer
 display.setCursor(0,0);
 display.setTextSize(1);  // Set text size
 display.println (" Set Ldg Alt");
  display.println ("  ");
 display.setTextSize(2);  // Set text size
 display.print ("  ");
 display.println (la);// Used for troubleshooting only- sends to serial
 display.println ("");
 display.display();
 // End Display Landing Altitude   
   
la=landalt; // overwrites the old reading with the new one EG
Rold = R; // overwrites the old reading with the new one.

  }
//manual rate adjust  
 R2 =(quad2.position());
  if (R2 != Rold2){
   time1 = millis(); //Resets Time1 each time there is an encoder change- for screen timer
   (Rdif2 = (R2-Rold2));
   if (Rdif2 == 1) (mr= (manualrate ++)*100);  //Real Lear Adjust in 100ft/min increments
   if (Rdif2 == -1) (mr= (manualrate --)*100);  //Real Lear Adjust in 100ft/min increments
   //Serial.println (mr); //send to serial for troubleshooting
 
 // Manual Rate  display 
 display.clearDisplay();   // clears the screen and buffer
 display.setCursor(0,0);
 display.setTextSize(1);  // Set text size
 display.println ("Set Man. Rate");
  display.println ("  ");
 display.setTextSize(2);  // Set text size
 display.print ("  "); //Space text to the right
 display.println (mr);
 display.println ("   ");
 display.display();
 // End Display Manual Rate 
 
mr = manualrate; // overwrites the old reading with the new one EG
Rold2 = R2;  // overwrites the old reading with the new one.
  
 }

if ((millis() - time1) < 5000) goto ENCODER; //This causes the encoder section to repeat untill 5 seconds since last encoder change!
  }





//  R3 =(quad3.position());
//  if (R3 != Rold3) {
 //   (Rdif3 = (R3-Rold3));
 //   if (Rdif3 == 1) Serial.println ("A55"); // VOR bug down
 //   if (Rdif3 == -1) Serial.println ("A56"); //VOR bug up
 //   Rold3 = R3;
  //}





   

  


 

   
   
