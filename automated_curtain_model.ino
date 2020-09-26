#include "Stepper.h"
#include "IRremote.h"

/*----- Variables, Pins -----*/
#define STEPS  32   // Number of steps per revolution of Internal shaft
int Steps2Take;  // 2048 = 1 Revolution
int receiver = 6; // Signal Pin of IR receiver to Arduino Digital Pin 6

/*-----( Declare objects )-----*/
// Setup of proper sequencing for Motor Driver Pins
// In1, In2, In3, In4 in the sequence 1-3-2-4

Stepper small_stepper(STEPS, 8, 10, 9, 11);
IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;     // create instance of 'decode_results'

//The curtains are closed when the program starts.
bool closed = true;

void CloseCurtains(){
  if(closed){
    Serial.println("curtains already closed"); 
  }else{
    // PREV button pressed
    small_stepper.setSpeed(500);
    Steps2Take  =  -6144;  // Rotate CCW
    small_stepper.step(Steps2Take);
    delay(2000);
    closed=true;
  }
}

void OpenCurtains(){
  if(closed){
    // NEXT button pressed
    small_stepper.setSpeed(500); //Max seems to be 700
    Steps2Take  =  6144;  // Rotate CW
    small_stepper.step(Steps2Take);
    delay(2000); 
    closed=false;
    
  }else{
    Serial.println("curtains already open"); 
  }
}

void setup()
{ 
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop()
{

while(!(irrecv.decode(&results)));// if no button is pressed, wait until code is received
if (irrecv.decode(&results)) // have we received an IR signal?

Serial.println(results.value);
  {
    switch(results.value)

    {

      case 1784160559: CloseCurtains();
                       break;

      case 1361042349: OpenCurtains(); 
                       break;
                      
    }
    
      irrecv.resume(); // receive the next value
  }  


}/* --end main loop -- */
