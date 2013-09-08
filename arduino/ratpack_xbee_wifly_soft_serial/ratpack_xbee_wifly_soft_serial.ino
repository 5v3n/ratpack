#include "WiFly.h"
#include "SPI.h"
#include <SoftwareSerial.h>
#include "Config.h"

/*
* rat pack remote control. software serial port version.
* 
* it's slower than hardware rx/tx but great for debugging since you can print on the serial console.
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

WiFlyClient    client(HOST, 80);
SoftwareSerial mySerial(SOFT_RX, SOFT_TX);

void setup(){
  //Serial comm
  Serial.begin(9600);
  mySerial.begin(9600);
  //------------------------ led setup ------------------------
  pinMode(ledPin, OUTPUT);      
  //------------------------ button setup ------------------------
  pinMode(buttonPin, INPUT);
  //use internal 10k
  digitalWrite(buttonPin, HIGH);
  //------------------------ wifly setup ----------------------------------
  
  Serial.println("Setting UART mode...");
  WiFly.setUart(&mySerial);
  
  Serial.println("Setting up wifly...");
  WiFly.begin();
  
  Serial.println("Joining network....");
  if (WiFly.join(ssid, passphrase)) {
    Serial.print("Joined wifi: ");
    Serial.println(ssid);
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
    if(digitalRead(buttonPin) == HIGH){
      //--------- send status to server
      Serial.println("Local: button pressed 0");
      connectClient(generateHttpPut(HOST, RESOURCE, '0'));
      client.stop();
    }
    else if(digitalRead(buttonPin) == LOW){
      //--------- send status to server
      Serial.println("Local: button pressed: 1");
      connectClient(generateHttpPut(HOST, RESOURCE, '1'));
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
        Serial.println("Remote: button pressed: 1");
        digitalWrite(ledPin, HIGH);
      }
      else if(atoi(state) == 0) {
        Serial.println("Remote: button pressed: 0");
        digitalWrite(ledPin, LOW);
      }
    }
    client.stop();
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
    client.print(http_request);
    client.println();
  } else {
    Serial.println("connection failed...");
  }

}
