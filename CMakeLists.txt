set(CMAKE_VERBOSE_MAKEFILE ON)

#music-player-hw-control
cmake_minimum_required(VERSION 2.8)
project(music-player-hw-control)
add_definitions(-std=c++11)

add_executable(mp-hw-control
    src/main.cpp
    src/module.cpp
    src/util.cpp
    src/mpc_control.cpp
    src/hw_interface/SPI_Device.cpp
    src/hw_interface/Samsung_20T202DA2JA.cpp
    src/hw_interface/RPi_GPIO.cpp
    src/hw_interface/Button.cpp
    src/hw_interface/PushButton.cpp
    src/hw_interface/SwitchButton.cpp
    src/hw_interface/LedControl.cpp
    src/hw_interface/PCM512x_spi.cpp)

include_directories(inc)
#for some reason the pcm512x.h are empty
#pacman -S linux-raspberrypi-headers
#EXECUTE_PROCESS( COMMAND uname -r COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
#include_directories(/usr/lib/modules/${ARCHITECTURE}/build/include/config/snd/soc)
#message (${ARCHITECTURE})
include_directories(/usr/include)
include_directories(/root/dev/linux/sound/soc/codecs)
include_directories(/root/dev/linux/include)

#curl
#include_directories(/usr/local/include)
#TARGET_LINK_LIBRARIES(wifisoundsystem curl)

#include_directories(/usr/include/gstreamer-1.0)
#include_directories(/usr/include/glib-2.0)
#include_directories(/usr/lib/arm-linux-gnueabihf/glib-2.0/include)
#include_directories(/usr/include/libxml2)
##### g_value_get_boolean not define
#message (STATUS ${CMAKE_THREAD_LIBS_INIT})
#TARGET_LINK_LIBRARIES(wifisoundsystem gobject-2.0)
# for g_source_remove
#TARGET_LINK_LIBRARIES(wifisoundsystem glib-2.0)
#TARGET_LINK_LIBRARIES(wifisoundsystem gstreamer-1.0)

# if no link to pthread, runtime error when starting thread
find_package(Threads REQUIRED)
TARGET_LINK_LIBRARIES(mp-hw-control ${CMAKE_THREAD_LIBS_INIT})

#boost
#find_package(Boost 1.50.0 EXACT COMPONENTS program_options REQUIRED )
#find_package(Boost 1.50.0 EXACT REQUIRED )
#INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
#useless so far coz property_tree is header only, but keep for future if needed
#TARGET_LINK_LIBRARIES(wifisoundsystem ${Boost_LIBRARIES} )

#debug
#cmake -DCMAKE_BUILD_TYPE=Debug ..
