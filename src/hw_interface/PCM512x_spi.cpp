    #include "hw_interface/PCM512x_spi.h"

using namespace std;

#define PCM512X_SPI_CPOL_CPHA 0b01
#define PCM512X_SPI_SPEED 50000
#define PCM512X_SPI_DELAY 0
//TODO as for screen, it's 2 x 8(WL) rather than 1 x 16(WL) as in data sheet
//? do define with reg num + config ?? #define PCM512X_PLL_D_0000
#define PCM512X_SPI_WL 0

#define REG_WRITE(x) ((x << 1) & 0b11111110)
#define REG_READ(x)  ((x << 1) | 0b00000001)

//register address
#define PCM512X_PLL_REF 0x0d
#define PCM512X_PLL_P 0x14
#define PCM512X_PLL_J 0x15
#define PCM512X_PLL_D 0x17
#define PCM512X_PLL_R 0x18

#define PCM512X_PLL_DDSP 0x1b
#define PCM512X_PLL_DDAC 0x1c
#define PCM512X_PLL_DNCP 0x1d
#define PCM512X_PLL_DOSR 0x1e

#define PCM512X_I2S_FORMAT_LENGTH 0x28

PCM512x_spi::PCM512x_spi(int channel) 
    : SPI_Device(channel, 
                PCM512X_SPI_CPOL_CPHA, 
                PCM512X_SPI_SPEED, 
                PCM512X_SPI_DELAY, 
                PCM512X_SPI_WL)
{
    LOG << "PCM512x_spi" << endl;
    init_SPI();
    char buff[2];
    
    //reg 13, set PLL reference to BCK (1), default SCK (0)
    buff[0] = REG_WRITE(PCM512X_PLL_REF);
    buff[1] = 0b00010000;
    send_buff(buff, 2);
    this_thread::sleep_for(chrono::milliseconds(50));
    
    //P, keep default 0000: P=1
    /*buff[0] = REG_WRITE(PCM512X_PLL_P);
    buff[1] = 0b00010000;
    send_buff(buff, 2);
    this_thread::sleep_for(chrono::milliseconds(50));*/
    
    //J 100000: J=32
    buff[0] = REG_WRITE(PCM512X_PLL_J);
    buff[1] = 0b100000;
    send_buff(buff, 2);
    this_thread::sleep_for(chrono::milliseconds(50));
    
    //D, keep default 0000: D=0
    /*buff[0] = REG_WRITE(PCM512X_PLL_D);
    buff[1] = 0b00010000;
    send_buff(buff, 2);
    this_thread::sleep_for(chrono::milliseconds(50));*/
    
    //R 0001: R=2
    buff[0] = REG_WRITE(PCM512X_PLL_R);
    buff[1] = 0b0001;
    send_buff(buff, 2);
    this_thread::sleep_for(chrono::milliseconds(50));
    
    ////values from pcm512x datasheet - page 28 - 44,1kHz RSCK:32
    //DDSP 2
    buff[0] = REG_WRITE(PCM512X_PLL_DDSP);
    buff[1] = 0b0000001;
    send_buff(buff, 2);
    this_thread::sleep_for(chrono::milliseconds(50));
    
    //DDAC 16 : 
    buff[0] = REG_WRITE(PCM512X_PLL_DDAC);
    buff[1] = 0b0001111;
    send_buff(buff, 2);
    this_thread::sleep_for(chrono::milliseconds(50));
    
    //DNCP 4
    buff[0] = REG_WRITE(PCM512X_PLL_DNCP);
    buff[1] = 0b0000011;
    send_buff(buff, 2);
    this_thread::sleep_for(chrono::milliseconds(50));
    
    //DOSR 8
    buff[0] = REG_WRITE(PCM512X_PLL_DOSR);
    buff[1] = 0b0000111;
    send_buff(buff, 2);
    this_thread::sleep_for(chrono::milliseconds(50));
    
    //i2s format and word length (16bit) 0b00000000
    buff[0] = REG_WRITE(PCM512X_I2S_FORMAT_LENGTH);
    buff[1] = 0b00000000;
    send_buff(buff, 2);
    this_thread::sleep_for(chrono::milliseconds(50));

    LOG << "PCM512x_spi -- end" << endl;
}

PCM512x_spi::~PCM512x_spi(){
    close_SPI();
}
