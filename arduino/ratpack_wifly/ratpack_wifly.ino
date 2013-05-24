#include "WiFly.h"
#include "SPI.h"
#include "Config.h"

/*
* rat pack remote control
*
* sven kraeuter for making things happen.
*
* http://github.com/5v3n/ratpack
*
* http://makingthingshappen.de
*
* Find the WiFly library here: https://github.com/sparkfun/WiFly-Shield
*
* Connect/Disconnect logic heavily inspired by Tom Igoe' 
* repeating web client example (public domain):
* 
* http://arduino.cc/en/Tutorial/WebClientRepeating
*
*
*/

unsigned long lastConnectionTime = 0; // last time you connected to the server, in milliseconds
boolean lastConnected = false;        // state of the connection last time through the main loop

WiFlyClient clientLed(HOST, 80);
WiFlyClient clientButton(HOST, 80);

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

void loop() { 
  if (clientLed.available()) {
    // receiving status from server
    if(clientLed.find("\"activated\":")){
      char state[1];
      clientLed.readBytes(state, 1);
      if(atoi(state) == 1){
        Serial.println("response says: BUTTON PRESSED!");
        digitalWrite(ledPin, HIGH);
      }
      else if(atoi(state) == 0) {
        Serial.println("response says: button not pressed.");
        digitalWrite(ledPin, LOW);
      }
    } 

  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!clientLed.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    clientLed.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!clientLed.connected() && (millis() - lastConnectionTime > IO_INTERVAL)) {
    //------------------- button part ---------------------------------------
    if(digitalRead(buttonPin) == LOW){
      Serial.println("arduino says: Button NOT pressed.");
      //--------- send status to server
      connectClient(clientButton, generateHttpPut(HOST, RESOURCE, '0'));
      clientButton.stop();
    }
    else if(digitalRead(buttonPin) == HIGH){
      Serial.println("arduino says: Button IS pressed!");
      //--------- send status to server 
      connectClient(clientButton, generateHttpPut(HOST, RESOURCE, '1'));
      clientButton.stop();
    }
  
    //-------------------- signalling led part -------------------------------------
    char postString[255];
    sprintf(postString, "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: close\r\n", RESOURCE, HOST, USERAGENT);
    connectClient(clientLed, postString);
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = clientLed.connected();
}

char* generateHttpPut(char* host, char* resource, char occupied){
  char postString[255];
  sprintf(postString, "PUT %s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\nContent-Length: 16\r\nContent-Type: application/json\r\n\r\n{\"activated\": %c}"
  , resource, host, USERAGENT, occupied);

  return postString;
}

void connectClient(WiFlyClient client, char* http_request){
  if (client.connect()) {
    Serial.println("connected");
    Serial.println(http_request);
    client.print(http_request);
    client.println();
  } else {
    Serial.println("connection failed...");
  }

}