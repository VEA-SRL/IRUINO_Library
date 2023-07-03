#ifndef TIMERH
#define TIMERH
#include <vector>
#include "Arduino.h"


class timer {
private:
  uint8_t _mode; //0 = millis, 1 = micros, 2 = nanos
  uint64_t _timer;

public:
  timer (const uint8_t& mode = 0):_mode(mode), _timer(0) {}
  virtual ~timer (){}

  void start();

  void reset();

  double read();

  double readSeconds();

  double readMicroseconds();

  double readNanoseconds();
  double readMilliseconds();

};

extern std::vector<timer> timers;

int addTimer(const uint8_t& mode);

void removeTimer(const int& index);

#endif /* end of include guard: TIMERH */
