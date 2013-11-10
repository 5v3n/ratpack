/* Yun Pack - the Rat Pack using the new possibilities of the Arduino Yun.
*
*  Check out the repository & participate on github:
*
*  http://github.com/5v3n/ratpack
*
*  Copyright (c) 2013 Sven Kraeuter sven@makingthingshappen.de MIT License
*/

#include <Process.h>
#include "Config.h"

char commandBuffer[255]; //it's crude - feel free to handle the commandBuffer length more elegant

void setup() {
  // Initialize Bridge
  Bridge.begin();
  
  //------------------------ button setup ------------------------
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH); //use internal 10k
  //------------------------ led setup ---------------------------
  pinMode(ledPin, OUTPUT);
  
#ifdef DEBUG
  // Initialize Serial
  Serial.begin(9600);
  // Wait until a Serial Monitor is connected.
  while (!Serial){
    ;
  }
#endif
  
}

void loop() {
  //transfer button state to server
  if(digitalRead(buttonPin) == HIGH){
    sendState('0');
  }
  else if(digitalRead(buttonPin) == LOW){
    sendState('1');
  }
  //read button state from server & use the LED accordingly
  receiveState();
  //the yun is FAST - give the server a break ;-)
  delay(IO_INTERVAL);
}

void receiveState() {
  commandBuffer[0] = '\0';
  sprintf(commandBuffer, "curl %s/status.json", HOST);
  
  Process p;
  p.runShellCommand(commandBuffer);
  
#ifdef DEBUG
  Serial.println(commandBuffer);
#endif

  while(p.running());  //wait
  
  char state[2];
  state[1]='\0';
  
  if(p.find("\"activated\":")){
    p.readBytes(state, 1);
    if(atoi(state) == 1){
      digitalWrite(ledPin, HIGH);
    }
    else if(atoi(state) == 0) {
      digitalWrite(ledPin, LOW);
    }
  }
#ifdef DEBUG
  Serial.print("According to the server, the button state is: ");
  Serial.println(state);
  Serial.println();
#endif
  
  Serial.flush();
}

void sendState(byte state) {
 #ifdef DEBUG
  Serial.print("sending update, state will be: ");
  Serial.println((char)state);
 #endif
 
  commandBuffer[0] = '\0';
  sprintf(commandBuffer, "curl -X PUT -d \"{\\\"activated\\\": %c}\" http://%s/status.json", state, HOST);
  
#ifdef DEBUG
  Serial.println(commandBuffer);
#endif
  
  Process p;
  p.runShellCommand(commandBuffer);
  
 
 #ifdef DEBUG
  while(p.running());  //wait

  Serial.print("Response from PUT: ");
  while (p.available()>0) {
    char c = p.read();
    Serial.print(c);
  }
  Serial.println();
#endif
  
  // Ensure the last bit of data is sent.
  Serial.flush();
}

