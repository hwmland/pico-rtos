# pico-rtos

Very simple demo of FreeRTOS on RPi pico

## Hardware

Any board with RP2040 should do. I tested it on official RPi Pico. You can buy it from official distibutor - button `Buy` on [RPi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/) for about $4, on [Amazon](https://www.amazon.com/s?k=Raspberry+Pi+Pico) or wherever awailable for you. Chines boards from [Aliexpress](https://www.aliexpress.com/w/wholesale-Raspberry-Pi-Pico.html) will do as well. Only point to be carefull is that some bords (SeedStudio's XIAO, WaveShare) don't support debugging (pins SWCLK and SWDIO missing)
Demo use just one led (connected on pin 25) and serial port (115k, pins TX pin 1, RX pin 2) for some trace messges. None of them is mandatory however.

## Environment setup

First of all you need working envirnment to build and debug. It's described detailed in [Getting Started](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) document. I'll put here just simplified summary.

<details>
  <summary>Raspberry Pi</summary>

```bash
wget https://raw.githubusercontent.com/raspberrypi/pico-setup/master/pico_setup.sh
chmod +x pico_setup.sh
./pico_setup.sh
```

</details>

<details>
  <summary>Linux</summary>
  It's possible that Raspberry Pi setup script will do on 'normal' linux as well. Otherwise here is summary from documentation.

```bash
# get SDK
git clone https://github.com/raspberrypi/pico-sdk.git --branch master
cd pico-sdk
git submodule update --init
# set where to find SDK, e.g.
export PICO_SDK_PATH=../../pico-sdk
```

```bash
# install toolchain
sudo apt update
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential
```

Native gcc and g++ are needed to compile pioasm and elf2uf2. Ubuntu and Debian users might additionally need to also install `libstdc++-arm-none-eabi-newlib`.

</details>

<details>
  <summary>MacOS</summary>

```bash
# get SDK
git clone https://github.com/raspberrypi/pico-sdk.git --branch master
cd pico-sdk
git submodule update --init

# set where to find SDK, e.g.
export PICO_SDK_PATH=../../pico-sdk
```

```bash
# install homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

```bash
# install toolchain
brew install cmake
brew tap ArmMbed/homebrew-formulae
brew install arm-none-eabi-gcc
```

```bash
# If you are running on an Apple M1-based Mac you will need to install Rosetta 2 as the Arm compiler is still only compiled for x86 processors and does not have an Arm native version.
/usr/sbin/softwareupdate --install-rosetta --agree-to-license
```

</details>

# Download project

```bash
git clone https://github.com/hwmland/pico-rtos.git
cd ./pico-rtos
git submodule update --init
```

It should clone FreeRTOS sources into project tree.

# Building

Check `./vscode/settings.json`. It's currently configured to use Microsoft's nmake. Commenting out line

```json
//  "cmake.generator": "NMake Makefiles",
```

should do the trick on other systems. Than using cmake and build should work just fine from VS Code.

# Debugging

This depends very much on debug probe and and environment. Check `settings.json` and `launch.json`. I only have experience with using picoprobe with custom openocd on windows. Any current knowledge and debug hardware for debugging ARM MCUs should be valid for RP2040 as well.

# Code description

Code is just for demonstration purpose for SMP rtos-view extension. It does nothing usefull. Just several threads that cyclically sleep or fully use MCU core. Threads are very simple and documented.

## SMP / single core

The same code could be used to check buth SMP and non-SMP scenarios. Just adjust `./src/FreeRTOSConfig.h`

```c
#if 1 /* 0 or 1*/
// SMP => 2 cores
#define configNUMBER_OF_CORES 2
#define configUSE_CORE_AFFINITY 1
#else
// No SMP => 1 core
#define configNUMBER_OF_CORES 1
#define configUSE_CORE_AFFINITY 0
#endif
```
