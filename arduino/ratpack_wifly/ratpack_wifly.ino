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
*/

unsigned long lastConnectionTime = 0; // last time you connected to the server, in milliseconds

WiFlyClient client(HOST, 80);

void setup(){
  //------------------------ led setup ------------------------
  pinMode(ledPin, OUTPUT);      
  //------------------------ button setup ------------------------
  pinMode(buttonPin, INPUT);    
  //------------------------ wifly setup ----------------------------------
  Serial.begin(9600);
  //WiFly.setUart(&Serial);
  
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
  // if you're not connected, and the given time has passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > IO_INTERVAL)) {
    //------------------- button part ---------------------------------------
    if(digitalRead(buttonPin) == LOW){
      Serial.println("local arduino says: Button NOT pressed.");
      //--------- send status to server
      connectClient(generateHttpPut(HOST, RESOURCE, '0'));
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
    }
    else if(digitalRead(buttonPin) == HIGH){
      Serial.println("local arduino says: Button IS pressed!");
      //--------- send status to server 
      connectClient(generateHttpPut(HOST, RESOURCE, '1'));
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
    }
  
    //-------------------- signalling led part -------------------------------------
    char postString[255];
    sprintf(postString, "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: close\r\n", RESOURCE, HOST, USERAGENT);
    connectClient(postString);
    // receiving status from server
    if(client.find("\"activated\":")){
      char state[1];
      client.readBytes(state, 1);
      if(atoi(state) == 1){
        Serial.println("rat pack server says: BUTTON PRESSED!");
        digitalWrite(ledPin, HIGH);
      }
      else if(atoi(state) == 0) {
        Serial.println("rat pack server says: button not pressed.");
        digitalWrite(ledPin, LOW);
      }
    }
    client.stop();
    Serial.println("------------- Rat Pack cycle completed -------------------");
    lastConnectionTime = millis();
  }
}

char* generateHttpPut(char* host, char* resource, char occupied){
  char postString[255];
  sprintf(postString, "PUT %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nContent-Length: 16\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n{\"activated\": %c}"
  , resource, host, USERAGENT, occupied);

  return postString;
}

void connectClient(char* http_request){
  if (client.connect()) {
    Serial.println("connected");
    Serial.println(http_request);
    client.print(http_request);
    client.println();
  } else {
    Serial.println("connection failed...");
  }

}
