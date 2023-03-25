# Info
Basic blinky project for NRF52840 on dongle board (PCA10059) with no mbr or bootloader. 
NO SOFTDEVICE EITHER!

# How to start
Edit `GNU_INSTALL_ROOT` `GNU_VERSION` and `arm-none-eabi` to match your current needs
after that execute `make` to buid hex file into `build` dir.

# How to program
As I dont plan on spending ~500$ on jlink, clones are flaky and stlink converted to jlink can talk to st devices (why would somebody do that) I didn't have much choice but to use trusty RPi as programmer.
Simply boot and rpios and compile openocd on that bad boy

```
git clone http://openocd.zylin.com/openocd
cd openocd/
./bootstrap
sudo apt install libtool #probably some deps i forgot
./configure --enable-sysfsgpio --enable-bcm2835gpio --disable-dependency-tracking #about this dep tracking i dont really know why i needed that
sudo make install
```

Now that we have openocd working time to wire.

```
GPIO 11 (pin 23) - SWCLK
GPIO  8 (pin 24) - SWDIO
GPIO 26 (pin 37) - TRST - NRST Didnt used it
GPIO 18 (pin 12) - SRST - NRST Didnt used it
GND - GND
```

# OpenOCD
On raspberry execute

```
/usr/local/bin/openocd -c "bindto 0.0.0.0" -f ./interface/raspberrypi-native.cfg -c "transport select swd" -c "adapter speed 1000" -f ./target/nrf52.cfg
```  

on local machine telnet to programmer. port 4444


# Sources and various info 

When to use what sdk: https://devzone.nordicsemi.com/nordic/nordic-blog/b/blog/posts/nrf-connect-sdk-and-nrf5-sdk-statement

nRF52840 documentation: https://infocenter.nordicsemi.com/index.jsp?topic=/ps_nrf52840