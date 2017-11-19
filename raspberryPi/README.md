Raspberry stuff.

## Installation

[The official set up](https://www.raspberrypi.org/documentation/installation/installing-images/windows.md) is pretty good.

[RaspberryPi 3的安装配置](https://robocoderhan.github.io/2016/12/13/Raspberry%20Pi%203%E7%9A%84%E5%AE%89%E8%A3%85%E8%AE%BE%E7%BD%AE/)

It introduced a wonderful remote access tool for linux like system (MobaXterm), it can replace MTPUTTY with ease.

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
