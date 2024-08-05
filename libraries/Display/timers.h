#ifndef TIMERH
#define TIMERH
#include <vector>
#include "Arduino.h"

#define MILLIS 0
#define MICROS 1
#define NANOS 2

/**
 * @class timer
 * @brief A class that provides timing functionality in different units (milliseconds, microseconds, nanoseconds).
 */
class timer {
private:
  uint8_t _mode; //0 = millis, 1 = micros, 2 = nanos
  uint64_t _timer;

public:
  /**
   * @brief Constructs a timer object with the specified mode.
   * @param mode The mode of the timer (0 = milliseconds, 1 = microseconds, 2 = nanoseconds).
   */
  timer (const uint8_t& mode = 0):_mode(mode), _timer(0) {}

  /**
   * @brief Destructor for the timer object.
   */
  virtual ~timer (){}

  /**
   * @brief Starts the timer.
   */
  void start();

  /**
   * @brief Resets the timer to zero.
   */
  void reset();

  /**
   * @brief Reads the elapsed time in the current mode.
   * @return The elapsed time in the current mode.
   */
  double read();

  /**
   * @brief Reads the elapsed time in seconds.
   * @return The elapsed time in seconds.
   */
  double readSeconds();

  /**
   * @brief Reads the elapsed time in microseconds.
   * @return The elapsed time in microseconds.
   */
  double readMicroseconds();

  /**
   * @brief Reads the elapsed time in nanoseconds.
   * @return The elapsed time in nanoseconds.
   */
  double readNanoseconds();

  /**
   * @brief Reads the elapsed time in milliseconds.
   * @return The elapsed time in milliseconds.
   */
  double readMilliseconds();
};

/**
 * @brief A vector of timers.
 */
extern std::vector<timer> timers;

/**
 * @brief Adds a timer to the vector of timers.
 * @param mode The mode of the timer (0 = milliseconds, 1 = microseconds, 2 = nanoseconds).
 * @return The index of the timer in the vector.
 */
int addTimer(const uint8_t& mode);

/**
 * @brief Removes a timer from the vector of timers.
 * @param index The index of the timer in the vector.
 */
void removeTimer(const int& index);

#endif /* end of include guard: TIMERH */
