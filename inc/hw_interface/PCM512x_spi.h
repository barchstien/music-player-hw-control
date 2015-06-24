#ifndef _PCM512X_SPI_
#define _PCM512X_SPI_

#include <string>
#include "hw_interface/SPI_Device.h"

#include "module.h"

/**
SPI control of pcm512x
Mostly set PLL dividers, which is mandatory in sw mode
*/

class PCM512x_spi : public SPI_Device {
public:
    /** Constructor
    @param channel SPI channel (0 & 1 for raspberry pi) */
    PCM512x_spi(int channel);
    ~PCM512x_spi();
    
private:
    

};
#endif
