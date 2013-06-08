# Rat Pack - the internet of things's basic building block

## Abstract

Frank, Sammy & Dean were a great team known as the "Rat Pack". What's true for these fellows is also true for Sinatra, Ruby and Arduino: put them together for instant Internet Of Things awesomeness!

## How to

### Run the rat pack server locally

`$ foreman start` will let you run a local server instance on [localhost:5000](http://localhost:5000)

>Have you tried turning it off and on again?

You can test drive if the server reacts without using the actual remote control with these commands:

`$ curl -X PUT -d "{\"activated\": 1}" localhost:5000/status.json`
`$ curl -X PUT -d "{\"activated\": 0}" localhost:5000/status.json`

### Build the rat pack remote control

See `fritzing/ratpack_xbee_wifly.fzz` for the needed components and the wiring of your remote control. 

![Rat Pack Fritzing Sketch](https://github.com/5v3n/ratpack/blob/master/fritzing/ratpack_xbee_wifly.jpg?raw=true)

If you aren't using Fritzing already you can download it [here](http://fritzing.org/download/).

Find the arduino software in `arduino/ratpack_xbee_wifly`. All you have to do is adjust the `Config.h` file with your wify credentials and the rat pack server's host data:

```c
// Wifi parameters
char passphrase[]  = "YOUR PASSPHRASE";
char ssid[]        = "YOUR SSID";
// ratpack server
char* HOST         = "YOUR SINATRA SERVER";
```

### Run your rat pack server on heroku

Create a [new heroku app](https://devcenter.heroku.com/articles/quickstart) to deploy the whole thing to. Jot down the server's url (something.herokuapp.com) and use it in the `arduino/Config.h` file mentioned above.

### Enjoy your rat pack remote control

If you keep the button on your breadboard pushed it won't take long until you see the reaction in your browser. Also mind the LED is actually tracking the server's response, so it's online too. Voilá, you just entered the Internet Of Things (tm)!