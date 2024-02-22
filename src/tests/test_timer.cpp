#include "../lib/utility/timer/timer.hpp"
#include <chrono>
#include <thread>
// using namespace std::chrono;

int main()
{
    return 0;
    using namespace mytm;
    Timer timer;

    timer.Tick();
    std::this_thread::sleep_for(milliseconds(62500));
    timer.Tock();
    Time time = timer.Get_Time();
    std::cout << "Hour: " << time.Hours << std::endl
              << "Mins: " << time.Mins  << std::endl
              << "secs: " << time.Secs  << std::endl;

}