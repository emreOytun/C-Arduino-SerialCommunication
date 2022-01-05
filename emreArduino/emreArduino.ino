const int buttonPin = 9;      
const int Delay = 50;   
const int ledPin = 2;

// Variables will change:
int lastState = LOW;       // the previous steady state from the input pin
int lastUndeterminedState = LOW;  // the previous undetermined state from the input pin
int currentState;                // the current state from the input pin

// time variable is unsigned long because it'll get bigger fast
unsigned long lastTriggertime = 0;  // the last time the output pin was triggered
int ctr=0;


void setup() {
  pinMode(ledPin,OUTPUT);
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
}



void turnOn(){
  digitalWrite(ledPin, HIGH);
}
void turnOff(){
  digitalWrite(ledPin, LOW);
}
void flashLed(){
  int i;
  int firstState = digitalRead(ledPin);
  for(i=0;i<3;i++){
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  }
  digitalWrite(ledPin, firstState); // Led'll be open if it was open before this command.
}

int square(){
    long int num = Serial.parseInt(); // converts coming input to integer.
    long int result = num*num;
    
    char EOFch='f'; 
    int trigger = 1;   
    Serial.println(trigger); // Send C to trigger reading from Arduino
    
    Serial.println(result); // Send the result  
    Serial.println(EOFch); // Send E0F character 
    Serial.flush();
    return 1; // done
}

int counter(){ 

  while(1){
  if(Serial.available()>0){
      char check = Serial.read();
      if(check == 'd'){
        char EOFch='f'; 
        Serial.println(ctr);
        Serial.println(EOFch);
      }
      if(check == 'q'){
        ctr = 0;
        return 0;
      }
    }
    
  // read the state of the switch/button:
  currentState = digitalRead(buttonPin);

  // If the switch/button changed/triggered : 
  if (currentState != lastUndeterminedState) {
    
    lastTriggertime = millis();           // save the current time 
    lastUndeterminedState = currentState; // save last undetermined state, which we're not sure
  }

  if ((millis() - lastTriggertime) > Delay) {
    // if the button state has changed:
    if (lastState == HIGH && currentState == LOW){
      ctr++;
    }
    // save the the actual last state
    lastState = currentState;
  }
  
  }
  
}

char UserInput;
void loop() {
  while(Serial.available()>0){ // Waits until user enters an input
    Serial.flush();
    UserInput = Serial.read(); // Gets the input
    if(UserInput == 'a'){
      turnOn();
    }
    else if(UserInput == 'b'){
      turnOff(); 
    }
    else if(UserInput == 'c'){
      flashLed();
    }
    else if(UserInput == 'd'){
       square();
    }
    else if(UserInput == 'e'){
      counter();
    }
  } 
} 
  
