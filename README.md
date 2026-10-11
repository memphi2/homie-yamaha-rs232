> ## This project has a successor
>
> **[ESPHome-Yamaha-Serial](https://github.com/memphi2/ESPHome-Yamaha-Serial)** is
> a complete reimplementation of this idea as a native ESPHome external
> component. If you are setting this up today, start there.
>
> What it does differently:
>
> - **ESPHome instead of Homie/MQTT.** Native entities for Home Assistant:
>   `media_player`, switches, selects, numbers, sensors. No MQTT broker and no
>   property convention in between.
> - **More receivers.** Profiles for the RX-Vx500, RX-Vx600, RX-Vx700 and
>   RX-Vx800 families, with model auto-detection.
> - **A real protocol layer.** Non-blocking parser, command queue with spacing,
>   timeouts and retries, and diagnostics entities that make a failure
>   reportable instead of silent.
> - **The full command surface.** Everything that is not an entity stays
>   reachable through a raw command action with symbolic aliases.
>
> It is GPL-3.0 like this project, and it carries this project's attribution.
>
> **This repository still works** and stays here for anyone running it. It is
> no longer developed; fixes and new receiver support go to the successor.
> Hardware validation reports are what that project needs most, so if you own
> a Yamaha receiver with a DB9 port, an issue there is genuinely useful.

# Homie ESP8266 Yamaha RX-Vxxxx Control (Serial/RS232)
RS232 Control for Yamaha AV-Receivers

This simple code uses
[homie-esp8266](https://github.com/homieiot/homie-esp8266/) to create a wifi
control that will talk to my [openhab](https://openhab.org/) via
MQTT, following the [homie](https://homieiot.github.io/)
convention for IoT.

# Connect ESP8266 with your Receiver

I'm using a simple [max232] RS232 to TTL Serial Port converter Module with DB9 connector. Be sure that CTS line is held high(Shortcut RTS & CTS) by the module, otherwise your receiver won't send any data.
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


