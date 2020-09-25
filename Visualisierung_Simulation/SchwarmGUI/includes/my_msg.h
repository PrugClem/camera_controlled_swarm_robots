#ifndef MY_MSG_H_INCLUDED
#define MY_MSG_H_INCLUDED

#include "my_time.h"

std::string get_msg(const std::string& prefix)
{
    int s, m, h;
    get_time(2, s, m, h);
    return "[" + time_to_string(s, m, h) + "] " + "[" + prefix + "]: ";
}

#endif // MY_MSG_H_INCLUDED
