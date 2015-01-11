
#include "util.h"
#include "hw_interface/PushButton.h"
#include "hw_interface/SwitchButton.h"

#include "hw_interface/RPi_GPIO.h"

#include <chrono>
#include <vector>
#include <thread>

/** pin map */
//many are useless, and will be remove with upcoming HW layout
#define GPIO_RELAY_0 17
#define GPIO_RELAY_1 27
#define GPIO_RELAY_POWER 22
#define GPIO_FRONT_LED 12

#define GPIO_AUX_1 24
#define GPIO_AUX_2 23
#define GPIO_ONOFF 4
#define GPIO_BUTTON_UP 25
#define GPIO_BUTTON_DOWN 16
#define GPIO_SWITCH_NET_MODE 5
#define GPIO_SWITCH_MODE 6

using namespace std;
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


int main(int argc, char *argv[]){
    LOG << "-------------------------\nStarting music-player-hw-control " << std::endl;
    
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
    
    //gpio_tests();
    #if 1
    //button array, used pause/resume sample and save CPU cycles/energy
    std::shared_ptr<std::vector<std::shared_ptr<Button> > > button_vector(new std::vector<std::shared_ptr<Button> >());
    
    std::shared_ptr<ThreadSafeQ<std::shared_ptr<Message> > > bus(new ThreadSafeQ<std::shared_ptr<Message> >());
    
    ////buttons
    //pull up
    std::shared_ptr<PushButton> button_up(new PushButton(GPIO_BUTTON_UP, bus, 1, 1));
    button_vector->push_back(button_up);
    //pull up
    std::shared_ptr<PushButton> button_down(new PushButton(GPIO_BUTTON_DOWN, bus, 1, 1));
    button_vector->push_back(button_down);
    //no pull
    std::shared_ptr<SwitchButton> switch_aux_small(new SwitchButton(GPIO_AUX_1, bus));
    button_vector->push_back(switch_aux_small);
    //pull up
    std::shared_ptr<SwitchButton> switch_onoff(new SwitchButton(GPIO_ONOFF, bus, 1));
    button_vector->push_back(switch_onoff);
    //pull up
    std::shared_ptr<SwitchButton> switch_mode(new SwitchButton(GPIO_SWITCH_MODE, bus, 1));
    button_vector->push_back(switch_mode);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    LOG "----------------------" << std::endl;
    
    std::shared_ptr<Message> m = 0;
    while (true){
        m = bus->deque();
        if (0 != m){
            LOG << m->print() << std::endl;
        }else{
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
    #endif
    
    LOG << "End World" << std::endl;
    return 0;
}




