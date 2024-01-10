cmake_minimum_required(VERSION 3.12)

# Name the project. It's recomended to use value of ${workspaceFolderBasename} variable, 
# as this is used later on some configurations like launch.json. 
SET(ProjectName pico-rtos)

# Set any variables required for importing libraries
SET(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/FreeRTOS-Kernel)

# Import libraries
include(pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# This can be the same as target name, but can also be different.
# The reason is that there could be multiple targets in the project, e.g. executable and library.
project(${ProjectName})

pico_sdk_init()

# Add subdirectories
add_subdirectory(src)
 