# Homie ESP8266 Yamaha RX-Vxxxx Control (Serial/RS232)
RS232 Control for Yamaha AV-Receivers

This simple code uses
[homie-esp8266](https://github.com/homieiot/homie-esp8266/tree/develop-v3) to create a wifi
control that will talk to my [openhab](https://openhab.org/) via
MQTT, following the [homie](https://homieiot.github.io/)
convention for IoT.

# Connect ESP8266 with your Receiver

I'm using a simple [max232](https://www.amazon.de/FITYLE-MAX232-Konverter-Modul-USB-Seriell-Konversion-Mehrfarbig/dp/B079RGQTLG/ref=sr_1_11?ie=UTF8&qid=1552064756&sr=8-11) RS232 to TTL Serial Port converter Module with DB9 connector. Be sure that CTS line is held high(Shortcut RTS & CTS) by the module, otherwise your receiver won't send any data.
Just connect :

TxD Pin ESP8266 <---> RxD Pin converter module

RxD Pin ESP8266 <---> TxD Pin converter module

GND Pin ESP8266 <---> GND Pin converter module

5V  Pin ESP8266 <---> VCC Pin converter module

# openhab Integration
See [here](https://community.openhab.org/t/integration-of-older-yamaha-rx-v-av-receivers-rs-232-serial/65204)


# Notes

- To power on your receiver you have to turn on "WakeON-RS232"
- Disconnect RX/TX while flashing your ESP


