
#include "util.h"

#include <chrono>


int main(int argc, char *argv[]){
    LOG << "-------------------------\nStarting music-player-hw-control " << std::endl;
    
    
    LOG << "End World" << std::endl;
    return 0;
}



void gpio_tests(){
    //// tests nes HW ////
    #if 0
    ///relay tests
    RPi_GPIO::exportPin(GPIO_RELAY_0);
    RPi_GPIO::setOutput(GPIO_RELAY_0);
    RPi_GPIO::exportPin(GPIO_RELAY_1);
    RPi_GPIO::setOutput(GPIO_RELAY_1);
    RPi_GPIO::exportPin(GPIO_RELAY_POWER);
    RPi_GPIO::setOutput(GPIO_RELAY_POWER);
    
    RPi_GPIO::exportPin(GPIO_FRONT_LED);
    RPi_GPIO::setOutput(GPIO_FRONT_LED);
    
    //amp power ON
    LOG << "Amp power : on" << endl;
    RPi_GPIO::write(GPIO_RELAY_POWER, 1);
    this_thread::sleep_for(chrono::milliseconds(250));
    
    //input dac
    LOG << "input : DAC" << endl;
    RPi_GPIO::write(GPIO_RELAY_0, 0);
    this_thread::sleep_for(chrono::milliseconds(250));
    RPi_GPIO::write(GPIO_RELAY_1, 1);
    this_thread::sleep_for(chrono::milliseconds(250));
    #endif
    
    #if 0
    //button test
    RPi_GPIO::exportPin(GPIO_AUX_1);
    RPi_GPIO::setInput(GPIO_AUX_1);
    RPi_GPIO::setPulloff(GPIO_AUX_1);
    RPi_GPIO::exportPin(GPIO_AUX_2);
    RPi_GPIO::setInput(GPIO_AUX_2);
    RPi_GPIO::setPulloff(GPIO_AUX_2);
    RPi_GPIO::exportPin(GPIO_ONOFF);
    RPi_GPIO::setInput(GPIO_ONOFF);
    RPi_GPIO::setPullup(GPIO_ONOFF);
    RPi_GPIO::exportPin(GPIO_BUTTON_UP);
    RPi_GPIO::setInput(GPIO_BUTTON_UP);
    RPi_GPIO::setPullup(GPIO_BUTTON_UP);
    RPi_GPIO::exportPin(GPIO_BUTTON_DOWN);
    RPi_GPIO::setInput(GPIO_BUTTON_DOWN);
    RPi_GPIO::setPullup(GPIO_BUTTON_DOWN);
    RPi_GPIO::exportPin(GPIO_SWITCH_ADMIN);
    RPi_GPIO::setInput(GPIO_SWITCH_ADMIN);
    RPi_GPIO::setPullup(GPIO_SWITCH_ADMIN);
    RPi_GPIO::exportPin(GPIO_SWITCH_MODE);
    RPi_GPIO::setInput(GPIO_SWITCH_MODE);
    RPi_GPIO::setPullup(GPIO_SWITCH_MODE);
    #endif
    
    #if 0
    //aux 2
    this_thread::sleep_for(chrono::milliseconds(2500));
    LOG << "input : Aux 2" << endl;
    RPi_GPIO::write(GPIO_RELAY_0, 1);
    this_thread::sleep_for(chrono::milliseconds(250));
    RPi_GPIO::write(GPIO_RELAY_1, 0);
    
    //aux 1
    this_thread::sleep_for(chrono::milliseconds(2500));
    LOG << "input : Aux 1" << endl;
    RPi_GPIO::write(GPIO_RELAY_0, 0);
    this_thread::sleep_for(chrono::milliseconds(250));
    RPi_GPIO::write(GPIO_RELAY_1, 0);
    
    ////screen + led + buttons
    std::shared_ptr<Samsung_20T202DA2JA> screen(new Samsung_20T202DA2JA(0));
    screen->start();
    
    for (int i=0; i<10240; i++){
        //led²
        RPi_GPIO::write(GPIO_FRONT_LED, i%2);
        //RPi_GPIO::write(GPIO_RELAY_POWER, i%2);
        //this_thread::sleep_for(chrono::milliseconds(5000));
        
        screen->on();
        screen->clear();
        screen->cursor_home();
        //this_thread::sleep_for(chrono::milliseconds(2000));
        screen->write("testtttttt");
        this_thread::sleep_for(chrono::milliseconds(500));
        screen->write("    <<------->");
        this_thread::sleep_for(chrono::milliseconds(500));
        
        //buttons
        LOG << "Aux 1 :\t" << RPi_GPIO::read(GPIO_AUX_1) << endl;
        LOG << "Aux 2 :\t" << RPi_GPIO::read(GPIO_AUX_2) << endl;
        LOG << "ON OFF :\t" << RPi_GPIO::read(GPIO_ONOFF) << endl;
        LOG << "UP :\t\t" << RPi_GPIO::read(GPIO_BUTTON_UP) << endl;
        LOG << "Down :\t" << RPi_GPIO::read(GPIO_BUTTON_DOWN) << endl;
        LOG << "Admin :\t" << RPi_GPIO::read(GPIO_SWITCH_ADMIN) << endl;
        LOG << "Mode :\t" << RPi_GPIO::read(GPIO_SWITCH_MODE) << endl;
        LOG << "------------------ " << i << endl;
        
        //screen->scroll_on_2nd_line("1234567890123456789012345");
        //this_thread::sleep_for(chrono::milliseconds(10000));
        screen->clear();
        screen->off();
        this_thread::sleep_for(chrono::milliseconds(10));
    }
    #endif
    
}


