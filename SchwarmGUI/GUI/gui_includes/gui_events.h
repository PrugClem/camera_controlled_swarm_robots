#ifndef GUI_EVENTS_H_INCLUDED
#define GUI_EVENTS_H_INCLUDED

#include "gui_base.h"
#include "gui_handler.h"
#include "button.h"
#include <Event.h>
#include <deque>

#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    #include <mutex>
    using std::mutex;
#else
    #include <mingw.mutex.h>
    using mingw_stdthread::mutex;
#endif

using namespace GUI;

class ElementActionEvent : public Event<ElementActionEvent>
{
    friend class GUI::ElementHandler;

private:
    mutex event_mutex;

    static void push(GUI::ElementEventInfo info)
    {
        for(void* instance : get_instances())
        {
            ElementActionEvent* event = reinterpret_cast<ElementActionEvent*>(instance);
            if(event->event_queue.size() < 16)
            {
                event->event_mutex.lock();
                event->event_queue.push_back(info);
                event->event_mutex.unlock();
            }
        }
    }
protected:
    std::deque<GUI::ElementEventInfo> event_queue;

    virtual bool event_trigger(void)    {return this->event_queue.size() > 0;}
    virtual void reset(void)
    {
        if(this->event_queue.size() > 0)
        {
            this->event_mutex.lock();
            this->event_queue.pop_front();
            this->event_mutex.unlock();
        }
    }

public:
    GUI::Element& get_element(void)                     {return *this->event_queue.at(0).element;}
    int get_action(void)                                {return this->event_queue.at(0).action;}
    float get_cursor_pos_x(void) const noexcept         {return this->event_queue.at(0).cursor_pos_x;}
    float get_cursor_pos_y(void) const noexcept         {return this->event_queue.at(0).cursor_pos_y;}
};

class ButtonActionEvent : public ElementActionEvent
{
protected:
    virtual bool event_trigger(void)
    {
        if(this->event_queue.size() > 0)
        {
            if(this->event_queue.at(0).element->get_type() == GUI::Element::Type::BUTTON)
                return true;
        }
        return false;
    }
public:
    GUI::Element& get_element(void) = delete;
    GUI::Button& get_button(void)   {return *((GUI::Button*)this->event_queue.at(0).element);}
};

class ButtonInteractEvent : public ButtonActionEvent
{
protected:
    virtual bool event_trigger(void)
    {
        if(this->event_queue.size() > 0)
        {
            int action = this->event_queue.at(0).action;
            if
            (
                this->event_queue.at(0).element->get_type() == GUI::Element::Type::BUTTON &&
                (
                    action == GUI::ButtonAction::HOVER_ON ||
                    action == GUI::ButtonAction::HOVER_OFF ||
                    action == GUI::ButtonAction::CLICK_LEFT_DOWN ||
                    action == GUI::ButtonAction::CLICK_LEFT_UP ||
                    action == GUI::ButtonAction::CLICK_RIGHT_DOWN ||
                    action == GUI::ButtonAction::CLICK_RIGHT_UP
                )
            )
            {
                return true;
            }
        }
        return false;
    }
};

class ButtonHoverEvent : public ButtonInteractEvent
{
    virtual bool event_trigger(void)
    {
        if(this->event_queue.size() > 0)
        {
            int action = this->event_queue.at(0).action;
            if
            (
                this->event_queue.at(0).element->get_type() == GUI::Element::Type::BUTTON &&
                (
                    action == GUI::ButtonAction::HOVER_ON ||
                    action == GUI::ButtonAction::HOVER_OFF
                )
            )
            {
                return true;
            }
        }
        return false;
    }
};

class ButtonClickEvent : public ButtonInteractEvent
{
protected:
    virtual bool event_trigger(void)
    {
        if(this->event_queue.size() > 0)
        {
            int action = this->event_queue.at(0).action;
            if
            (
                this->event_queue.at(0).element->get_type() == GUI::Element::Type::BUTTON &&
                (
                    action == GUI::ButtonAction::CLICK_LEFT_DOWN ||
                    action == GUI::ButtonAction::CLICK_LEFT_UP ||
                    action == GUI::ButtonAction::CLICK_RIGHT_DOWN ||
                    action == GUI::ButtonAction::CLICK_RIGHT_UP
                )
            )
            {
                return true;
            }
        }
        return false;
    }
};

#endif // GUI_EVENTS_H_INCLUDED
