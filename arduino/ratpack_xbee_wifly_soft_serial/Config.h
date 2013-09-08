#ifndef __CONFIG_H__
#define __CONFIG_H__

// Wifi parameters
char passphrase[] = "YOUR PASSPHRASE";
char ssid[] = "YOUR SSID";
// ratpack server
char* HOST         = "YOUR SINATRA SERVER";
char* USERAGENT    = "ratpack-client/xbee-wifly-shield";
char* RESOURCE     = "/status.json";
//io settings
byte buttonPin            = 2;    // button pin 
byte ledPin               = 9;   // led pin
byte SOFT_RX              = 6;   // Software Serial RX/TX
byte SOFT_TX              = 7;


const unsigned long IO_INTERVAL = 0.5*1337;  // delay between updates, in milliseconds

#endif
