
#include "util.h"
#include "hw_interface/PushButton.h"
#include "hw_interface/SwitchButton.h"
#include "hw_interface/Samsung_20T202DA2JA.h"
#include "hw_interface/LedControl.h"

#include "hw_interface/RPi_GPIO.h"
#include "mpc_control.h"

#include <chrono>
#include <vector>
#include <thread>

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

/** pin map */
//many are useless, and will be remove with upcoming HW layout
#define GPIO_RELAY_0 17
#define GPIO_RELAY_1 27
#define GPIO_RELAY_POWER 22
#define GPIO_FRONT_LED 12

#define GPIO_AUX_1 24
//#define GPIO_AUX_2 23
#define GPIO_ONOFF 6
#define GPIO_BUTTON_UP 25
#define GPIO_BUTTON_DOWN 16
//#define GPIO_SWITCH_NET_MODE 5
#define GPIO_SWITCH_MODE 4

#define GPIO_VALUE_ON 0

using namespace std;

/*
TODO
    _ resume current playlist with current track and current time !!!
    _ detect when web UI was used since last switch action (compare current to Webradio ??)

*/

enum state {
    OFF,
    WEB_CTRL,
    BOX_CTRL,
    AUX_IN
};
state current_state;
int value_GPIO_ONOFF = -1;
int value_GPIO_SWITCH_MODE = -1;
int value_GPIO_AUX_1 = -1;
std::string last_current;

shared_ptr<Samsung_20T202DA2JA> screen;

std::string title;
shared_ptr<ThreadSafeQ<shared_ptr<Message> > > bus(new ThreadSafeQ<shared_ptr<Message> >());

void process_message(shared_ptr<Message> m){
    shared_ptr<ButtonEvent> e;
    switch (m->label){
        case GPIO_ONOFF:
            e = dynamic_pointer_cast<ButtonEvent>(m);
            value_GPIO_ONOFF = e->value;
            if (GPIO_VALUE_ON == e->value){
                LOG << "ON" << endl;
                screen->clear();
                screen->on();
                //MpcControl::play();
                //default is Web Control
                /*if (BOX_CTRL == current_state){
                    screen->write("Box Control", true);
                }else{
                    screen->write("Web Control", true);
                }*/
                //default wake mode is BOX_CTRL, so set to WEB_CTRL coz it will be inversed
                current_state = WEB_CTRL;
                //send fake message from switch 
                shared_ptr<ButtonEvent> e(new ButtonEvent(GPIO_SWITCH_MODE, input::EVENT_VALUE_0, 0));
                bus->enque(e);
                
                RPi_GPIO::write(GPIO_RELAY_POWER, 1);
                this_thread::sleep_for(chrono::milliseconds(250));
                //screen->clear();
                title = "";
            }else{
                LOG << "OFF" << endl;
                MpcControl::stop();
                screen->write("Stand By", true);
                RPi_GPIO::write(GPIO_RELAY_POWER, 0);
                this_thread::sleep_for(chrono::milliseconds(1000));
                screen->clear();
                screen->off();
            }
            break;
        case GPIO_AUX_1:
            e = dynamic_pointer_cast<ButtonEvent>(m);
            value_GPIO_AUX_1 = e->value;
            LOG << "aux 1" << endl;
            //yes ! value is switched
            if (GPIO_VALUE_ON != e->value){
                MpcControl::pause();
                screen->write("Aux", true);
                LOG << "input : aux" << endl;
                RPi_GPIO::write(GPIO_RELAY_0, 0);
                this_thread::sleep_for(chrono::milliseconds(250));
                RPi_GPIO::write(GPIO_RELAY_1, 0);
                this_thread::sleep_for(chrono::milliseconds(250));
            }else{
                MpcControl::play();
                screen->clear();
                if (WEB_CTRL == current_state){
                    screen->write("Web Control", true);
                }else{
                    screen->write("Box Control", true);
                }
                LOG << "input : DAC" << endl;
                RPi_GPIO::write(GPIO_RELAY_0, 0);
                this_thread::sleep_for(chrono::milliseconds(250));
                RPi_GPIO::write(GPIO_RELAY_1, 1);
                this_thread::sleep_for(chrono::milliseconds(250));
                this_thread::sleep_for(chrono::milliseconds(500));
                screen->clear();
            }
            break;
        case GPIO_SWITCH_MODE:
            e = dynamic_pointer_cast<ButtonEvent>(m);
            if (-1 == value_GPIO_SWITCH_MODE){
                //ignored, part of start-up routine
                value_GPIO_SWITCH_MODE = e->value;
                LOG << "switch_mode IGNORED" << endl;
                MpcControl::load_current_playlist();
            }else{
                //inverse active mode
                if (WEB_CTRL == current_state){
                    MpcControl::save_current_playlist();
                    MpcControl::clear();
                    MpcControl::load_all_radio_as_playlist();
                    MpcControl::play();
                    MpcControl::repeat(true);
                    current_state = BOX_CTRL;
                    screen->write("Box Control", true);
                    this_thread::sleep_for(chrono::milliseconds(1000));
                    screen->clear();
                }else if (BOX_CTRL == current_state){
                    MpcControl::clear();
                    MpcControl::load_current_playlist();
                    MpcControl::play();
                    MpcControl::repeat(false);
                    current_state = WEB_CTRL;
                    screen->write("Web Control", true);
                    this_thread::sleep_for(chrono::milliseconds(1000));
                    screen->clear();
                }
                /*}/*else{
                    //init to WEB_CTRL mode, coz don't know what happened
                    current_state = WEB_CTRL;
                }*/
                value_GPIO_SWITCH_MODE = e->value;
                LOG << "switch_mode" << endl;
            }
            break;
        case GPIO_BUTTON_UP:
            e = dynamic_pointer_cast<ButtonEvent>(m);
            if (input::EVENT_RELEASED == e->type){
                LOG << "up" << endl;
                Util::exec_read_bash("mpc prev");
            }
            break;
        case GPIO_BUTTON_DOWN:
            e = dynamic_pointer_cast<ButtonEvent>(m);
            if (input::EVENT_RELEASED == e->type){
                LOG << "down" << endl;
                Util::exec_read_bash("mpc next");
            }
            break;
        
    }
}

int main(int argc, char *argv[]){
    LOG << "-------------------------\nStarting music-player-hw-control " << endl;
    
    MpcControl::stop();
    
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
    /*LOG << "Amp power : on" << endl;
    RPi_GPIO::write(GPIO_RELAY_POWER, 1);
    this_thread::sleep_for(chrono::milliseconds(250));
    
    //set dac as input
    LOG << "input : DAC" << endl;
    RPi_GPIO::write(GPIO_RELAY_0, 0);
    this_thread::sleep_for(chrono::milliseconds(250));
    RPi_GPIO::write(GPIO_RELAY_1, 1);
    this_thread::sleep_for(chrono::milliseconds(250));*/
    
    //led
    //doesn't seam to work... hw problem ?
    /*shared_ptr<LedControl> led(new LedControl(GPIO_FRONT_LED, 1500, 50));
    led->start();
    this_thread::sleep_for(chrono::milliseconds(1000));
    led->enable_blink();*/
    //led->set_on_no_blink();
    
    //screen
    screen = shared_ptr<Samsung_20T202DA2JA>(new Samsung_20T202DA2JA(0));
    screen->start();
    
    //gpio_tests();
    #if 1
    //button array, used pause/resume sample and save CPU cycles/energy
    shared_ptr<vector<shared_ptr<Button> > > button_vector(new vector<shared_ptr<Button> >());
    
    //shared_ptr<ThreadSafeQ<shared_ptr<Message> > > bus(new ThreadSafeQ<shared_ptr<Message> >());
    
    ////buttons
    //pull up
    shared_ptr<PushButton> button_up(new PushButton(GPIO_BUTTON_UP, bus, 1, 1));
    button_vector->push_back(button_up);
    //pull up
    shared_ptr<PushButton> button_down(new PushButton(GPIO_BUTTON_DOWN, bus, 1, 1));
    button_vector->push_back(button_down);
    //no pull
    shared_ptr<SwitchButton> switch_aux_small(new SwitchButton(GPIO_AUX_1, bus));
    button_vector->push_back(switch_aux_small);
    //pull up
    shared_ptr<SwitchButton> switch_onoff(new SwitchButton(GPIO_ONOFF, bus, 1));
    button_vector->push_back(switch_onoff);
    //pull up
    shared_ptr<SwitchButton> switch_mode(new SwitchButton(GPIO_SWITCH_MODE, bus, 1));
    button_vector->push_back(switch_mode);
    
    
    std::string title_tmp;
    
    //wait for all buttons to start
    this_thread::sleep_for(chrono::milliseconds(500));
    LOG << "----------------------" << endl;
    
    shared_ptr<Message> m = 0;
    while (true){
        m = bus->deque();
        while (0 != m){
            LOG << m->print() << endl;
            process_message(m);
            m = bus->deque();
        }
        
        //check title played by mpd
        title_tmp = MpcControl::current();
        if (0 != title.compare(title_tmp)){
            //new title
            LOG << "new title" << endl;
            title = title_tmp;
            //debug cut string with space
            istringstream iss(title);
            vector<string> words;
            copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter(words)
            );
            stringstream line_1, line_2;
            int cnt = 0;
            for (int i=0; i<words.size(); i++){
                //LOG << "-----" << i << words[i] << endl;
                if ((cnt + 1 + words[i].size()) < 20){
                    line_1 << words[i];
                    if ((i+1) < words.size()){
                        line_1 << " ";
                    }
                    cnt += words[i].size() + 1;
                }else{
                    //avoid to enQ in line_1 if next word shorter
                    cnt = 21;
                    line_2 << words[i];
                    if ((i+1) < words.size()){
                        line_2 << " ";
                    }
                }
            }
            LOG << "1:" << line_1.str() << endl;
            LOG << "2:" << line_2.str() << endl;
            
            
            screen->clear();
            screen->on();
            
            screen->write_line(0, line_1.str(), 2);
            if (line_2.str().size() <= 20){
                screen->write_line(1, line_2.str(), 2);
            }else{
                screen->scroll_on_2nd_line(line_2.str());
            }
        }
        
        this_thread::sleep_for(chrono::milliseconds(50/*5*/));
    }
    #endif
    
    LOG << "End World" << endl;
    return 0;
}




