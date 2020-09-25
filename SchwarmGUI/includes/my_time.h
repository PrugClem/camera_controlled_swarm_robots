#ifndef MY_TIME_H_INCLUDED
#define MY_TIME_H_INCLUDED

#include <ctime>
#include <sstream>

void get_time(int delay, int& s, int& m, int& h)
{
    // get time
    __time64_t t;
    _time64(&t);

    // decode time
    s = t % 60;
    m = (t / 60) % 60;
    h = ((t / 3600) + delay) % 24;
}

std::string time_to_string(int s, int m, int h)
{
    std::stringstream ss;
    ss << ((h < 10) ? "0" : "") << h << ":" << ((m < 10) ? "0" : "") << m << ":" << ((s < 10) ? "0" : "") << s;
    return ss.str();
}

#endif // MY_TIME_H_INCLUDED
