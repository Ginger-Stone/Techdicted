#include <Keypad.h>
#include <Servo.h>

//Status Pins
#define redLed 11
#define greenLed 12
#define orangeLed 13

//Variables and Pins used on servo
int pos = 0;    // variable to store the servo position
int ServoSignal = 10; //the Servo library supports servos on only two pins: 9 and 10.

String password="#12345"; //Password is set to this value
char pswd[7];
int i = 0;
unsigned long Time;

//Indicate that you are using a 4x4 keypad as the library can be used by multiple types of  keypads
const byte rows=4;
const byte cols=4;

char keys[rows][cols]={
  {'1','2','3','A',},
  {'4','5','6','B',},
  {'7','8','9','C',},
  {'*','0','#','D',},
};

//Pins to connect to on Arduino board
byte rowPins[rows]={5, 4, 3, 2};
byte colPins[cols]={9, 8, 7, 6};

//Create an instance of the Keypad and Stepper classes
Keypad keypad=Keypad(makeKeymap (keys), rowPins, colPins, rows, cols);
Servo myservo;

//Door status functions

  void LockedStatus(){
    digitalWrite(greenLed, HIGH);
    digitalWrite(orangeLed, LOW);
    digitalWrite(redLed, LOW);
    Serial.println("green");
  }

  void UnlockedStatus(){
    digitalWrite(orangeLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, LOW);
  }

  void WrongPasswordStatus(){
    digitalWrite(redLed, HIGH);
    digitalWrite(orangeLed, LOW);
    digitalWrite(greenLed, LOW);
  }

//Get the entered password
int GetPassword(){
  Serial.println("Getting password");
  while(i<6){ //size of all passwords set should be six, the last digit is the null character
    char key=keypad.getKey();
    if (key){
      pswd[i] = key;
      i++;
    }
  }
  pswd[6] = '\0';
  i=0;

  CheckPassword(String(pswd));
  return 0;
}

void CheckPassword(String PSWD){
  Serial.println(PSWD);
  if(PSWD == password){
  Serial.println(PSWD);
  Serial.println("Safe Door Opening!!");
  }else{ //else 
    Serial.println(PSWD);
    Serial.println("Wrong password, Please try Again");
    WrongPasswordStatus();
    GetPassword();
  }
}

void LockDoor(){
  if(pos==180){
    pos-=180;
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15); 
    Serial.print("Servo position on lock ");
    Serial.println(pos);
    LockedStatus();
    
  }else{
    
    Serial.println("Door alreaddy locked");
  }
}

void UnlockDoor(){
  if(pos==0){
    pos+=180;
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15); 
    Serial.print("Servo position on unlock ");
    Serial.println(pos);
    UnlockedStatus();
    
  
  }else{
    
    Serial.print("Door already unlocked: ");
    Serial.println(pos);
  }
  }


void setup() {
  Serial.begin(9600);
  pinMode(orangeLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  myservo.attach(ServoSignal);  // attaches the servo on pin 10 to the servo object
  pos = myservo.read(); // read the current angle of the servo
  if (pos!=0){ //always home the servo on power on. home sets it to locked position
    myservo.write(0); 
    delay(15); 
    
    pos=myservo.read();
    Serial.print("Home at " );
    Serial.println(pos);
  }
  Serial.print("Servo at ");
  Serial.println(pos);

  LockedStatus();
  digitalWrite(greenLed, HIGH);
  delay(1000);
  
}

void loop() {
  int PasswordSuccess = GetPassword();
  while(!keypad.getKey()) //While no key is pressed, wait

//  unlock door
  if(PasswordSuccess == 0){
    Serial.println("Safe Door unlocking...");
    UnlockDoor();
    digitalWrite(orangeLed, HIGH);
    delay(1000);
    PasswordSuccess=1; //Do this so that the door only opens and wait for the lock command. Otherwise it will keep running looping the unlock door function.
  }

//  lock door
  Serial.println("Safe Door locking NOW...");
  LockDoor();

}
