#ifndef __CONFIG_H__
#define __CONFIG_H__

//comment out to get rid of debug output & the loop waiting for the Serial connection:
#define DEBUG

//io settings
const byte buttonPin            = 2;    // button pin
const byte ledPin               = 9;    // signalling led pin
//ratpack server
const char HOST[]               = "YOU RATPACK SERVER";

const unsigned long IO_INTERVAL = 0.5*1337;  // millisecs delay between updates

#endif
