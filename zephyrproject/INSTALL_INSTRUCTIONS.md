## VisionLead embedded application

This folder contains all code for both ZephyrOS and our application code. Application code is contained inside the *src* folder.

The following sections describe how to install all required dependancies, and build the application.

### Installing dependancies

This is based on the following guide from the Zephyr Project [here](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#).
Follow the section for installing dependancies that can be found on that page for your respective operating system, and then install python3 virtual enviroment.

#### seting up west
once you have installed those dependancies and installed python3 / python3-venv (virtual enviroment), perform the following steps from a cli that is in the zephyrproject folder.

1. python3 -m venv .venv

2. source .venv/bin/activate

3. pip install west

4. west init 

5. west update 

3. west zephyr-export

4. pip install -r ~/zephyrproject/zephyr/scripts/requirements.txt

These steps setup west, which is the helper tool used to build zephyros projects, as well as install the zephyrOS code base.

#### setting up zephyr SDK

from the same cli session as in the previous steps, cd into the *zephyr* folder, and run the following command

1. west sdk install

this will install the ZephyrOS SDK for your system. 

Once this is complete, you should be ready to build the projects

### Building the application

to build the application, open a cli to the zephyrproject folder, and run the following command

1. west build -p always -b arduino_nano_33_ble src

this will create a *build* folder, which contains the build of the application. 

### Flashing the application to device

This still needs to be tested, but we need a different version of *bossac* than what is used by zephyrOS by default. To get the arduino specific version, all we need to do is follow the steps outlined [here](https://docs.zephyrproject.org/latest/boards/arduino/nano_33_ble/doc/index.html) under programming and debugging, and then run the flash command using the path to your specific bossac file. In most linux systems, the path is $HOME/.arduino15/packages/arduino/tools/bossac/1.9.1-arduino2/bossac, so the flash command will look something like this

1. west flash --bossac=$HOME/.arduino15/packages/arduino/tools/bossac/1.9.1-arduino2/bossac

