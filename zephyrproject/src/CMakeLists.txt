# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
set(BOARD arduino_nano_33_ble)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(vision_lead)

# Add the include directory
# target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ble)
zephyr_include_directories(ble)
zephyr_include_directories(gpio)

target_sources(app PRIVATE src/main.c ble/ble.c gpio/gpio.c)
