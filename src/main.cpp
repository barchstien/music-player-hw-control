
#include "util.h"

#include "hw_interface/RPi_GPIO.h"
#include "hw_interface/SPI_Device.h"

#include <chrono>
#include <vector>
#include <thread>

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

/** pin map */
#define GPIO_UNMUTE 26


using namespace std;

#define PCM512X_SPI_CHAN 0
#define PCM512X_SPI_CPOL_CPHA 0b01
#define PCM512X_SPI_SPEED 50000
#define PCM512X_SPI_DELAY 0
#define PCM512X_SPI_WL 0

#define PCM512X_PLL_REF 0x0d
#define PCM512X
#define PCM512X
#define PCM512X
#define PCM512X

int main(int argc, char *argv[]){
    LOG << "-------------------------\nStarting music-player-hw-control " << endl;
    
    ///relay tests
    RPi_GPIO::exportPin(GPIO_UNMUTE);
    RPi_GPIO::setOutput(GPIO_UNMUTE);
    
    //unmute
    LOG << "input : DAC" << endl;
    RPi_GPIO::write(GPIO_UNMUTE, 0);
    this_thread::sleep_for(chrono::milliseconds(500));
    RPi_GPIO::write(GPIO_UNMUTE, 1);
    this_thread::sleep_for(chrono::milliseconds(500));
    
    shared_ptr<SPI_Device> spi_0;
    spi_0 = shared_ptr<SPI_Device>(
        new SPI_Device(PCM512X_SPI_CHAN, PCM512X_SPI_CPOL_CPHA, PCM512X_SPI_SPEED, PCM512X_SPI_DELAY, PCM512X_SPI_WL));
    
    //reg 13, set PLL reference to BCK (1), default SCK (0)

    while (true){
        //RPi_GPIO::write(GPIO_UNMUTE, 1);
        this_thread::sleep_for(chrono::milliseconds(2000));
        //RPi_GPIO::write(GPIO_UNMUTE, 0);
        //this_thread::sleep_for(chrono::milliseconds(2000));
    }
    
    LOG << "End World" << endl;
    return 0;
}




