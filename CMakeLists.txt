# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
   set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(PICO_SDK_PATH ${USERHOME}/.pico-sdk/sdk/1.5.1)
set(PICO_TOOLCHAIN_PATH ${USERHOME}/.pico-sdk/toolchain/13_2_Rel1)
if(WIN32)
    set(pico-sdk-tools_DIR ${USERHOME}/.pico-sdk/tools/1.5.1)
    include(${pico-sdk-tools_DIR}/pico-sdk-tools-config.cmake)
    include(${pico-sdk-tools_DIR}/pico-sdk-tools-config-version.cmake)
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(archery_clock_firmware C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(archery_clock_firmware archery_clock_firmware.cpp )
target_sources(archery_clock_firmware PRIVATE "archery_clock_firmware.cpp")
target_include_directories(archery_clock_firmware PRIVATE "${PROJECT_SOURCE_DIR}")
add_subdirectory(lib/core)
add_subdirectory(lib/data)
add_subdirectory(lib/io)
add_subdirectory(lib/logging)
add_subdirectory(lib/message)
add_subdirectory(lib/mode)
add_subdirectory(lib/peripheral)

pico_set_program_name(archery_clock_firmware "archery_clock_firmware")
pico_set_program_version(archery_clock_firmware "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(archery_clock_firmware 1)
pico_enable_stdio_usb(archery_clock_firmware 1)

# Add the standard library to the build
target_link_libraries(archery_clock_firmware
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(archery_clock_firmware PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(archery_clock_firmware 
        hardware_timer
        hardware_clocks
        )

pico_add_extra_outputs(archery_clock_firmware)

