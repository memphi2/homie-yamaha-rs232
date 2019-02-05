# Homie ESP8266 Yamaha RX-Vxxxx Control (Serial/RS232)
RS232 Control for Yamaha AV-Receivers

This simple code uses
[homie-esp8266](https://github.com/marvinroger/homie-esp8266) to create a wifi
control that will talk to my [openhab](https://openhab.org/) via
MQTT, following the [homie](https://github.com/marvinroger/homie/tree/master)
convention for IoT.

# Connect ESP8266 with your Receiver

I'm using a simple [max232](https://www.aliexpress.com/item/MAX3232-RS232-COM-Serial-to-TTL-Converter-Module-Board-Have-LED/696400942.html?spm=2114.search0603.3.58.33631514tGi5Na&ws_ab_test=searchweb0_0,searchweb201602_2_10320_10065_10068_10890_10547_319_10546_317_10548_10696_453_10084_454_10083_433_10618_431_10304_10307_10820_537_536_10843_10059_10884_10887_100031_321_322_10103,searchweb201603_45,ppcSwitch_0&algo_expid=8c74e0aa-2c1b-4dd8-bf1b-61308df757bf-8&algo_pvid=8c74e0aa-2c1b-4dd8-bf1b-61308df757bf&transAbTest=ae803_4) RS232 to TTL Serial Port converter Module with DB9 connector 
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


