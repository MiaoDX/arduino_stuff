As I have said, the communication part is one of the most crucial ones for hardware programming. Here are some notes about how to communicate with the Arduino.

# Protocols

## stackoverflow questions

* [how-do-you-design-a-serial-command-protocol-for-an-embedded-system](https://stackoverflow.com/questions/1445387/how-do-you-design-a-serial-command-protocol-for-an-embedded-system/)
* [simple-serial-point-to-point-communication-protocol](https://stackoverflow.com/questions/815758/simple-serial-point-to-point-communication-protocol)

## min

[min-protocol/min](https://github.com/min-protocol/min)

>MIN is a simple point-to-point frame based protocol designed to connect a microcontroller to a PC (or other microcontroller).

It's neat, beautiful, excellent, do what I want, but the official example use Arduino with `SerialUSB` support which is not so ideal. And I failed to compile it even with the boards do support `SerialUSB` (needs some re-diagnose).

## firmata

[firmata](https://github.com/firmata) 

>Firmata is a protocol for communicating with microcontrollers from software on a computer (or smartphone/tablet, etc).

It's a big player, and extensive libraries rely on this, say, [johnny-five](https://github.com/rwldrn/johnny-five) for an example.

## The official website

[InterfacingWithSoftware](http://playground.arduino.cc/Main/InterfacingWithSoftware)

Arduino official website is far from perfect, really, I mean it, each and every time I want to find something, I have to google and maybe latter a link will bring me back to the official documents, but never get luck when using the search form in the official site.

And, note that, [playground](http://playground.arduino.cc/) seems like a good place to play around.

## CmdMessenger

[playground CmdMessenger](http://playground.arduino.cc/Code/CmdMessenger)

This is one interesting library.

There are four versions, and the [newest version](https://github.com/thijse/Arduino-CmdMessenger) is still in maintain, which seems nice, however, there are lots of forks and these forks vary here and there, but notice that, there are some forks come from the version 3. -.-

# Serial

## serial

[Cross-platform, Serial Port library written in C++](https://github.com/wjwwood/serial)

> This is a cross-platform library for interfacing with rs-232 serial like ports written in C++. It provides a modern C++ interface with a workflow designed to look and feel like PySerial, but with the speed and control provided by C++.

It dose seems promising, and can have a try on it.


# Networking

## uNetworking

[uNetworking](https://github.com/uNetworking/), it seems amazing, for one person can really lead such a huge project:

[uWebSockets](https://github.com/uNetworking/uWebSockets), [uTT](https://github.com/uNetworking/uTT) and [uSockets](https://github.com/uNetworking/uSockets).

# frnetlib

[frnetlib](https://github.com/Cloaked9000/frnetlib), it's small and neat and cross-platform.