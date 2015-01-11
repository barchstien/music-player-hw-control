#include "hw/PushButton.h"

PushButton::~PushButton(){}

int PushButton::process_read(int v){
    std::shared_ptr<ButtonEvent> e;
    if (v == no_press_value){
        e = std::shared_ptr<ButtonEvent> (new ButtonEvent(m_gpio_num, input::EVENT_RELEASED));
    }else{
        e = std::shared_ptr<ButtonEvent> (new ButtonEvent(m_gpio_num, input::EVENT_PRESSED));
    }
    m_coordinator->enqueMessage(e);
    
    return 0;
}
