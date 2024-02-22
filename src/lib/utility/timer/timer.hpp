#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <chrono>
#include <iostream>

namespace mytm{

using namespace std::chrono;

struct Time
{
  uint Hours = 0;
  uint Mins  = 0;
  uint Secs  = 0;
};

class Timer
{
private:
  using time_type = std::chrono::time_point< std::chrono::high_resolution_clock>;
public:
  Timer(){};
  ~Timer(){};

  void Tick();
  void Tock();

  Time Get_Time() const {return _Time;};

private:
  Time _Time;
  time_type _StartTimepoint;
};


}



#endif