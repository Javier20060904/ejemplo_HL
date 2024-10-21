#  Copyright (c) Microsoft Corporation. All rights reserved.
#  Licensed under the MIT License.

cmake_minimum_required (VERSION 3.20)

project (Ejemplo_Mejorado C)

# Create executable
add_executable (${PROJECT_NAME} main.c modules/handlers.c modules/gpio.c modules/uart.c modules/adc.c eventloop_timer_utilities.c)
target_link_libraries (${PROJECT_NAME} applibs pthread gcc_s c)

azsphere_target_hardware_definition(${PROJECT_NAME} TARGET_DEFINITION "mt3620_rdb.json")
azsphere_target_add_image_package(${PROJECT_NAME})
