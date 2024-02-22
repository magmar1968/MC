#include "timer.hpp"

namespace mytm{


void Timer::Tick()
{
    _StartTimepoint = high_resolution_clock::now();
}

void Timer::Tock()
{
    auto EndTimepoint = high_resolution_clock::now();

    auto start = time_point_cast<milliseconds>(_StartTimepoint).time_since_epoch().count();
    auto end   = time_point_cast<milliseconds>(EndTimepoint).time_since_epoch().count();

    auto ms = end - start;

    _Time.Secs = ms/1000;
    ms         -=_Time.Secs * 1000;
    _Time.Mins = _Time.Secs / 60;
    _Time.Secs -= _Time.Mins * 60;
    _Time.Hours = _Time.Mins / 60;
    _Time.Mins -= _Time.Hours * 60;
}




}