# Info
Basic TWI/I2C example. It's written in polling mode.
pin 0,13 as SCL  
and 0,15 as SDA

⚠ If you arent connecting rx pull it up to vcc as otherwise you will be met with break interrupt and hange there.

# How to start
Edit `GNU_INSTALL_ROOT` `GNU_VERSION` and `arm-none-eabi` to match your current needs
after that execute `make` to buid hex file into `build` dir.

# How to program
As I dont plan on spending ~500$ on jlink, clones are flaky and stlink converted to jlink can talk to st devices (why would somebody do that) I didn't have much choice but to use trusty RPi as programmer.
Simply boot and rpios and compile openocd on that bad boy

```
sudo apt automake install wget git autoconf libtool make pkg-config libusb-1.0-0 libusb-1.0-0-dev libhidapi-dev libftdi-dev libhidapi-dev telnet
git clone https://github.com/openocd-org/openocd.git
cd openocd/
./bootstrap
sudo apt install libtool #probably some deps i forgot
./configure --enable-sysfsgpio --enable-bcm2835gpio --enable-cmsis-dap
make -j4
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

# Serial over IP
```
sudo apt install socat
sudo socat file:/dev/ttyAMA0,raw,echo=0,b115200 tcp-l:3016
```

On local machine in vs code install `Serial Monitor (ms-vscode.vscode-serial-monitor)` and add your serial monitor IP

# Things to be mindful of
Sometimes you include source in makefile and it doesnt compile :/ 
Why? NRF added safeguard to prevent excessive flash usage when you import whole lib folder just add `{MODULE}_ENABLED` explained in `nordic_common.h`

# Sources and various info 

When to use what sdk: https://devzone.nordicsemi.com/nordic/nordic-blog/b/blog/posts/nrf-connect-sdk-and-nrf5-sdk-statement  
nRF52840 documentation: https://infocenter.nordicsemi.com/index.jsp?topic=/ps_nrf52840