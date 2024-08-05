#ifndef TIMERSCPP
#define TIMERSCPP

#include "timers.h"
#ifdef ESP32
  #define CLOCK_NANOSEC_TIME 4
#endif

// A vector to store timer objects
std::vector<timer> timers;

/**
 * @brief Starts the timer by resetting it.
 */
void timer::start(){
  reset();
}

/**
 * @brief Resets the timer based on the mode.
 */
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

/**
 * @brief Reads the elapsed time in the current mode.
 * @return The elapsed time in the current mode.
 */
double timer::read(){
  switch (_mode) {
    case 0:
      return millis() - _timer;
    case 1:
      return micros() - _timer;
    case 2:
      #ifdef ESP32
        return (xthal_get_ccount() - _timer) * CLOCK_NANOSEC_TIME;
      #else
        return 0;
      #endif
  }
  return -1;
}

/**
 * @brief Adds a timer to the vector of timers.
 * @param mode The mode of the timer (0 = milliseconds, 1 = microseconds, 2 = nanoseconds).
 * @return The index of the timer in the vector.
 */
int addTimer(const uint8_t& mode){
  timers.emplace_back(mode);
  return timers.size() - 1;
}

/**
 * @brief Removes a timer from the vector of timers.
 * @param index The index of the timer in the vector.
 */
void removeTimer(const int& index){
  timers.erase(timers.begin() + index);
}

/**
 * @brief Reads the elapsed time in seconds.
 * @return The elapsed time in seconds.
 */
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
  return read() / factor;
}

/**
 * @brief Reads the elapsed time in milliseconds.
 * @return The elapsed time in milliseconds.
 */
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

/**
 * @brief Reads the elapsed time in microseconds.
 * @return The elapsed time in microseconds.
 */
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

/**
 * @brief Reads the elapsed time in nanoseconds.
 * @return The elapsed time in nanoseconds.
 */
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