# VisionLead Embedded Application

This folder contains all code for both ZephyrOS and our application code. Application code is contained inside the *src* folder.

The following sections describe how to install all required dependencies, and build the application.

# Installing Dependencies

This is based on the following guide from the Zephyr Project [here](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#).
Follow the section for installing dependencies that can be found on that page for your respective operating system, and then install python3 virtual environment.

# Set Up Virtual Environment
It is recommended to use a virtual environment to isolate your project's dependencies and avoid conflicts with other Python projects on your system.

To setup and activate virtual environment with python, run the following script in a bash terminal.

```sh
python3 -m venv .venv 
source .venv/bin/activate #(or .venv~/Scripts/activate)
```

## Set Up West
Once all dependencies has been installed, run the following command line to set up West, which is the helper tool used to build zephyros projects, as well as install the zephyrOS code base.

```sh
pip install west
west init 
west update 
west zephyr-export
pip install -r /zephyr/scripts/requirements.txt #(or west packages pip --install)
```

## Set Up Zephyr SDK

From the same command line interface session as in the previous steps, cd into the *zephyr* folder, and run the following command

```sh
west sdk install
```

This will install the ZephyrOS SDK for your system. 

Once this is complete, you should be ready to build the projects

# Build and Program Application

to build the application, open a command line interface  to the *zephyrproject* folder, and run the following command

```sh
west build -S cdc-acm-console -p always -b arduino_nano_33_ble src
```

this will create a *build* folder, which contains the build of the application. 

## Flashing the application to device

To flash the Arduino board, a variant of *bossac* is needed. Please see details here.

  - For Windows systems, a compiled version of *bossac* has been added to the repository under *zephyrproject/*.

Ensure the Arduino board is connected to the system via USB.

 - For Windows system, you need to find the COM port the board is plugged into. This is OS depended and you can find it through many ways, for example, using PowerShell in Windows:

    ```sh
    Get-WmiObject Win32_SerialPort | Select-Object DeviceID, Description
    ```

Double click the RESET button on the arduino board (the only physical button on the devboard) then run the following command:

```sh
west flash --bossac="<path to the arduino version of bossac>"
```
  - For Windows system, the COM port is also required as an input   argument, for example
    ```sh
    west flash --bossac="<path to the arduino version of bossac>" --bossac-port="<COM port>"
    
    # Example, use the compiled bossac in the repo and COM port 6
    west flash --bossac=\bossac.exe --bossac-port="COM6"
    ```
  - For linux, this com port is not required
  ```sh
    west flash --bossac=$HOME/.arduino15/packages/arduino/tools/bossac/1.9.1-arduino2/bossac
  ```


## Monitoring console output

 - Logging messages are available over the usb connection using the same COM port used for programming. These messages can be viewed on the host computer using any uart console. 

 - On linux, this can be done simply using the minicom program

 ```sh
    minicom -D /dev/ttyACM0
 ```
 - Once in Minicom windows, press CTRL-A to open menu, and press X to exit Minicom