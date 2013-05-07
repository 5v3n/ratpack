#include "WiFly.h"
#include "SPI.h"
#include "Config.h"

/*
* rat pack remote control
*
* sven kraeuter for making things happen.
*
* http://makingthingshappen.de
*
* Find the WiFly library here: https://github.com/sparkfun/WiFly-Shield
*/

WiFlyClient client(HOST, 80);

void setup(){
  //------------------------ led setup ------------------------
  pinMode(ledPin, OUTPUT);      
  //------------------------ button setup ------------------------
  pinMode(buttonPin, INPUT);    
  //------------------------ wifly setup ----------------------------------
  Serial.begin(9600);
  
  WiFly.begin();
  
  if (WiFly.join(ssid, passphrase)) {
    Serial.println("associted with wifly network.");
  } 
  else {
    Serial.println("Association failed.");
    while (1) {
      // Hang on failure.
    }
  }  
}

void loop(){
  //------------------- button part ---------------------------------------
  if(digitalRead(buttonPin) == LOW){
    Serial.println("Button NOT pressed.");
    //--------- send status to server
    connectClient(generateHttpPut(HOST, RESOURCE, '0'));
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  else if(digitalRead(buttonPin) == HIGH){
    Serial.println("Button IS pressed!");
    //--------- send status to server 
    connectClient(generateHttpPut(HOST, RESOURCE, '1'));
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  
  //-------------------- signalling led part -------------------------------------
  char postString[255];
  sprintf(postString, "GET http://%s%s HTTP/1.0\r\n", HOST, RESOURCE);
  connectClient(postString);
  
  if (client.available()) {
    // receiving status from server
    if(client.find("\"activated\": ")){
      char state[1];
      client.readBytes(state, 1);
      if(atoi(state) == 1){
        Serial.println("BUTTON PRESSED!");
        digitalWrite(ledPin, HIGH);
      }
      else if(atoi(state) == 0) {
        Serial.println("button not pressed.");
        digitalWrite(ledPin, LOW);
      }
    } 
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
    }
    
  }
  delay(pause); 
}

char* generateHttpPut(char* host, char* resource, char occupied){
  char postString[255];
  sprintf(postString, "PUT %s HTTP/1.1\r\nUser-Agent: %s\r\nHost: %s\r\nContent-Length: 16\r\nContent-Type: application/json\r\n\r\n{\"activated\": %c}"
  , resource, USERAGENT, host, occupied);

  return postString;
}

void connectClient(char* http_request){
  if (client.connect()) {
    Serial.println("connected");
    Serial.println(http_request);
    client.print(http_request);
    client.println();
  } else {
    Serial.println("connection failed");
  }
  
  Serial.print("Waiting for the response...");
  while(!client.available()){
    Serial.print('.');
  }
  Serial.println();
}
