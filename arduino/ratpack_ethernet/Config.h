#ifndef __CONFIG_H__
#define __CONFIG_H__

// lan parameters
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte MAC[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// ratpack server
char* HOST         = "YOUR RAT PACK SERVER";
char* USERAGENT    = "ratpack-client/ethernet-shield";
char* RESOURCE     = "/status.json";
//io settings
byte buttonPin            = 2;    // button pin 
byte ledPin               = 9;   // led pin

const unsigned long IO_INTERVAL = 0.5*1337;  // delay between updates, in milliseconds

#endif
