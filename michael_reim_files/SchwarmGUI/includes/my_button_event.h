#ifndef MY_BUTTON_EVENT_H_INCLUDED
#define MY_BUTTON_EVENT_H_INCLUDED

#include <Event.h>
#include <chrono>
#include <sstream>
#include <iostream>
#include "../GUI/gui_includes/gui_events.h"

class ButtonListener : public Listener
{
private:
    ButtonInteractEvent button_interact_event;

    virtual void init(void)
    {
        this->register_event(button_interact_event, reinterpret_cast<Listener::EventFunc>(on_button_counter));
        this->register_event(button_interact_event, reinterpret_cast<Listener::EventFunc>(on_button_color));
        this->register_event(button_interact_event, reinterpret_cast<Listener::EventFunc>(on_action));
        this->set_interval(std::chrono::milliseconds(5));
    }

public:
    ButtonListener(void)
    {
        this->init();
    }

    static void on_button_color(ButtonInteractEvent& event)
    {
        float button_color1[] =  {0.1f, 0.1f, 0.1f, 1.0f,
                                  0.1f, 0.1f, 0.1f, 1.0f,
                                  0.1f, 0.1f, 0.1f, 1.0f,
                                  0.1f, 0.1f, 0.1f, 1.0f};
        float button_color2[] =  {0.05f, 0.05f, 0.05f, 1.0f,
                                  0.05f, 0.05f, 0.05f, 1.0f,
                                  0.05f, 0.05f, 0.05f, 1.0f,
                                  0.05f, 0.05f, 0.05f, 1.0f};
        float button_text_color[] = {0.0, 1.0, 1.0, 1.0};
        if(event.get_action() == ButtonAction::CLICK_LEFT_DOWN)
            event.get_button().set_button_color(button_color2);
        if(event.get_action() == ButtonAction::CLICK_LEFT_UP)
            event.get_button().set_button_color(button_color1);
        event.get_button().set_text_color(button_text_color);
    }

    static void on_button_counter(ButtonInteractEvent& event)
    {
        static int cntr = 0;
        std::stringstream ss;
        if(event.get_action() == ButtonAction::CLICK_LEFT_DOWN)
        {
            ++cntr;
            ss << cntr;
            std::string str = event.get_button().get_text_value();
            str.push_back('X');
            event.get_button().set_text_value(str);
            
        }
        if(event.get_action() == ButtonAction::CLICK_RIGHT_DOWN)
        {
            --cntr;
            ss << cntr;
            std::string str = event.get_button().get_text_value();
            if(str.size() > 0)
                str.pop_back();
            event.get_button().set_text_value(str);
        }
    }

    static void on_action(ButtonInteractEvent& event)
    {
        #if 0
        if(event.get_action() == ButtonAction::HOVER_ON)
            std::cout << "HOVER ON" << std::endl;
        if(event.get_action() == ButtonAction::HOVER_OFF)
            std::cout << "HOVER OFF" << std::endl;
        if(event.get_action() == ButtonAction::CLICK_LEFT_DOWN)
            std::cout << "LEFT DOWN" << std::endl;
        if(event.get_action() == ButtonAction::CLICK_LEFT_UP)
            std::cout << "LEFT UP" << std::endl;
        if(event.get_action() == ButtonAction::CLICK_RIGHT_DOWN)
            std::cout << "RIGHT DOWN" << std::endl;
        if(event.get_action() == ButtonAction::CLICK_RIGHT_UP)
            std::cout << "RIGHT UP" << std::endl;
            #endif
    }
};

#endif // MY_BUTTON_EVENT_H_INCLUDED
