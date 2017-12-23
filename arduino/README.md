Lesson learned about Arduino

## Environment

The official Arduino IDE is not so good for developers, luckily, there are pretty nice tools out there -- [visualmicro](http://www.visualmicro.com/), see the document for more info, it's pretty nice.

UPDATE: I am not so sure about this, but on my PC, the serial communication in visualmicro is worse than Arduino IDE. Or some [additional configuration needs to be done?](http://www.visualmicro.com/page/User-Guide.aspx?doc=Debugging-With-Different-Ports.html)

### Tips about visualmicro

Use `{@ReportDigital}` action when debug to give a more easy way to show the status of digital pins.

# Arduino communication

[http://www.arduinolibraries.info/categories/communication](http://www.arduinolibraries.info/categories/communication)

## WIFI with ESP8826

[Arduino使用ESP8266通信的实践](http://blog.csdn.net/ydogg/article/details/53337055)

[ekstrand/ESP8266wifi](https://github.com/ekstrand/ESP8266wifi)

## Flash new firmware

[ESP8266-01无线模块兼容Arduino, 刷固件](http://zhongbest.com/2016/09/07/esp8266-01/)

[安信可科技刷固件](http://wiki.ai-thinker.com/esp_download)


## MQTT with WIFI/Serial

[https://iot.stackexchange.com/questions/1190/how-to-use-mqtt-on-arduino-which-uses-serial-com-to-send-at-commands-to-esp8266](https://iot.stackexchange.com/questions/1190/how-to-use-mqtt-on-arduino-which-uses-serial-com-to-send-at-commands-to-esp8266)

-> [MQTT Communication with Arduino using ESP8266 ESP-01](https://sonyarouje.com/2016/03/15/mqtt-communication-with-arduino-using-esp8266-esp-01/)

[https://github.com/RoanBrand/SerialToTCPBridgeProtocol](https://github.com/RoanBrand/SerialToTCPBridgeProtocol)

This is really interesting, serial on embedded hardware <-> tcp on operating system.

## Robotdyn MEGA+WIFI

[https://robotdyn.com/catalog/boards/mega_wifi_r3_atmega2560_esp8266_flash_8gb_usb_ttl_ch340g_micro_usb/](https://robotdyn.com/catalog/boards/mega_wifi_r3_atmega2560_esp8266_flash_8gb_usb_ttl_ch340g_micro_usb/)

[ELECTRONICS - ARDUINO MEGA CONTROLLER WITH ESP8266](http://www.sysengineering.ru/blog/%D1%8D%D0%BB%D0%B5%D0%BA%D1%82%D1%80%D0%BE%D0%BD%D0%B8%D0%BA%D0%B0-%D0%BA%D0%BE%D0%BD%D1%82%D1%80%D0%BE%D0%BB%D0%BB%D0%B5%D1%80-arduino-mega-%D1%81-esp8266/)

[RobotDyn deals a double blow: Mega + ESP8266](https://geektimes.ru/post/287124/)

## Lesson learned

### `long` values

>If doing math with integers, at least one of the numbers must be followed by an L, forcing it to be a `long`.
-- [official document about long](https://www.arduino.cc/en/Reference/Long)

So, unlike ordinary C/Cpp, to declare a long type value, one can just type:

`long l = 1000000000;`

in arduino, we must append `L` after the value, like `long l = 1000000000L;`, this is really annoying. So if we want to do a multiply operation, we should do something like:

`long testLong = 2000 * long(32);`

I really think it is a bug, even the document pointed out it. 