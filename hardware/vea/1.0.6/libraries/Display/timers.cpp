#ifndef TIMERSCPP
#define TIMERSCPP

#include "timers.h"
#ifdef ESP32
  #define CLOCK_NANOSEC_TIME 4
#endif
std::vector<timer> timers;

void timer::start(){
  reset();
}

void timer::reset(){
  switch (_mode) {
    case 0:
      _timer = millis();
      break;
    case 1:
      _timer = micros();
      break;
    case 2:
      #ifdef ESP32
        _timer = xthal_get_ccount();
      #endif
      break;
  }
}

double timer::read(){
  switch (_mode) {
    case 0:
      return millis() - _timer;
    case 1:
      return micros() - _timer;
    case 2:
      #ifdef ESP32
        return (xthal_get_ccount() - _timer)*CLOCK_NANOSEC_TIME;
      #else
        return 0;
      #endif
  }
  return -1;
}


int addTimer(const uint8_t& mode){
  timers.emplace_back(mode);
  return timers.size() - 1;
}
void removeTimer(const int& index){
  timers.erase(timers.begin() + index);
}

double timer::readSeconds(){
  double factor = 1;
  switch (_mode)
  {
  case 0:
    factor = 1000.0;
    break;
  case 1:
    factor = 1000000.0;
    break;
  case 2:
    factor = 1000000000.0;
    break;
  }
  return read()/factor;
}
double timer::readMilliseconds()
{
  double factor = 1;
  switch (_mode)
  {
  case 0:
    factor = 1;
    break;
  case 1:
    factor = 1000.0;
    break;
  case 2:
    factor = 1000000.0;
    break;
  }
  return read() / factor;
}

double timer::readMicroseconds()
{
  double factor = 1;
  switch (_mode)
  {
  case 0:
    factor = 0.001;
    break;
  case 1:
    factor = 1.0;
    break;
  case 2:
    factor = 1000.0;
    break;
  }
  return read() / factor;
}
double timer::readNanoseconds()
{
  double factor = 1;
  switch (_mode)
  {
  case 0:
    factor = 0.000001;
    break;
  case 1:
    factor = 0.001;
    break;
  case 2:
    factor = 1.0;
    break;
  }
  return read() / factor;
}

#endif /* end of include guard: TIMERSCPP */
