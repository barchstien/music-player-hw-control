#ifndef _SWITCH_BUTTON_H_
#define _SWITCH_BUTTON_H_

#include "hw/Button.h"

/** a push button
It can be pressed for a time t to trigger an LONG_PRESS event
It also have a default value when not pressed */
class SwitchButton : public Button{
public:
    /** Create a Switch Button
    @param gpio_pin read from this pin
    @param pull_resistor <0 is pull down, >0 is pull up, 0 is no pull */
    SwitchButton(int gpio_n, std::shared_ptr<module> c, int pull_resistor=0)
        : Button(gpio_n, c, pull_resistor)
    {}
    //~SwitchButton();
    virtual int process_read(int v);
    
protected:
    std::shared_ptr<ButtonEvent> last_event;
};

#endif
