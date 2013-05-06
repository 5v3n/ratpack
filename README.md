# Rat Pack - the internet of things's basic building block

## Abstract

yadda yadda yadda...

## Run it locally

`$ foreman start` will let you run a local server instance on [localhost:5000](http://localhost:5000)

>Have you tried turning it off and on again?

`$ curl -X PUT -d "{\"activated\": 1}" localhost:5000/status.json`
`$ curl -X PUT -d "{\"activated\": 0}" localhost:5000/status.json`