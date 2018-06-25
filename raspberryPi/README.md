Raspberry stuff.

## Installation

[The official set up](https://www.raspberrypi.org/documentation/installation/installing-images/windows.md) is pretty good.

[RaspberryPi 3的安装配置](https://robocoderhan.github.io/2016/12/13/Raspberry%20Pi%203%E7%9A%84%E5%AE%89%E8%A3%85%E8%AE%BE%E7%BD%AE/)

It introduced a wonderful remote access tool for linux like system (MobaXterm), it can replace MTPUTTY with ease.

## Save image to/copy from disk 

[制作树苺派SD卡备份镜像——树苺派系统备份与还原指南](http://blog.lxx1.com/1450)

## VIM mouse right click as copy

[Disable vim automatic visual mode using mouse](http://www.varesano.net/blog/fabio/disable%20vim%20automatic%20visual%20mode%20using%20mouse)

Insert `set mouse-=a` into your ~/.vimrc file (maybe also for the root user).

## LOGIN via ssh

The default behavior of raspberry system changes A LOT!

> As of the November 2016 release, Raspbian has the SSH server disabled by default. It can be enabled manually from the desktop:

``` vi
* Enter sudo raspi-config in a terminal window
* Select Interfacing Options
* Navigate to and select SSH (it can varies, maybe in the `Interfacing Options`)
* Choose Yes
* Select Ok
* Choose Finish
```


### [And login via root](https://raspberrypi.stackexchange.com/questions/48056/login-as-root-not-possible):

``` vi
* Login, and edit this file: sudo nano /etc/ssh/sshd_config
* Find this line: PermitRootLogin without-password
* Edit: PermitRootLogin yes
* Close and save file
* reboot or restart sshd service using: /etc/init.d/ssh restart
* Set a root password if there isn't one already: sudo passwd root
```

## Some configuration for hardware

I2C

## Static IP

```
# vi /etc/network/interfaces

# this raspberry is for new generation number 1's platform
# ip:130

auto lo

iface lo inet loopback
iface eth0 inet dhcp

auto wlan0
iface wlan0 inet static
address 192.168.0.130
gateway 192.168.0.1
netmask 255.255.255.0
wpa-ssid pi2
wpa-psk wenwubaohu
# wpa-roam /etc/wpa_supplicant/wpa_supplicant.conf
iface default inet dhcp

```

The newer version has some specific configuration, which are not so necessary, we can stick to our old knowledge as above.

> NOTE(this is not repeatable): I failed to connect to the wireless network directly after the IP is set, I recovered the `/etc/network/interfaces` and use configuration below to connect to the network first, and then set the static IP makes the trick. So, maybe: connect to wireless (without static IP) -> set static IP -> ~~delete the wireless configuration in `/etc/wpa_supplicant/wpa_supplicant.conf`~~.

Newer one (2017), [https://www.raspberrypi.org/documentation/configuration/wireless/wireless-cli.md](https://www.raspberrypi.org/documentation/configuration/wireless/wireless-cli.md) to add wireless password and ssid.

And [https://www.raspberrypi.org/documentation/configuration/wireless/access-point.md](https://www.raspberrypi.org/documentation/configuration/wireless/access-point.md) to add wireless static ip.

## WebIOPi

[WebIOPi with patch](https://github.com/doublebind/raspi)

``` vi
WebIOPi successfully installed
* To start WebIOPi foreground   : sudo webiopi [-h] [-c config] [-l log] [-s script] [-d] [port]

* To start WebIOPi background   : sudo /etc/init.d/webiopi start
* To start WebIOPi at boot      : sudo update-rc.d webiopi defaults

* Weaved IoT Kit installed, log on http://developer.weaved.com to access your device

* Look in /home/pi/webioPi/WebIOPi-0.7.1/examples for Python library usage examples
```

To see it, `ip:8000`, usrname: `webiopi`, password: `raspberry`.

## Interfaces

``` vi
pi@raspberrypi:~/miaodx/webiopi $ gpio readall
 +-----+-----+---------+------+---+--B Plus--+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 | ALT0 | 1 |  3 || 4  |   |      | 5v      |     |     |
 |   3 |   9 |   SCL.1 | ALT0 | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 1 | ALT0 | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 1 | ALT0 | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |   IN | 0 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI |   IN | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO |   IN | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
 |  11 |  14 |    SCLK |   IN | 0 | 23 || 24 | 1 | IN   | CE0     | 10  | 8   |
 |     |     |      0v |      |   | 25 || 26 | 1 | IN   | CE1     | 11  | 7   |
 |   0 |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   |
 |   5 |  21 | GPIO.21 |   IN | 1 | 29 || 30 |   |      | 0v      |     |     |
 |   6 |  22 | GPIO.22 |   IN | 1 | 31 || 32 | 0 | IN   | GPIO.26 | 26  | 12  |
 |  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     |
 |  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 0 | IN   | GPIO.27 | 27  | 16  |
 |  26 |  25 | GPIO.25 |   IN | 0 | 37 || 38 | 0 | IN   | GPIO.28 | 28  | 20  |
 |     |     |      0v |      |   | 39 || 40 | 1 | IN   | GPIO.29 | 29  | 21  |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+--B Plus--+---+------+---------+-----+-----+

```