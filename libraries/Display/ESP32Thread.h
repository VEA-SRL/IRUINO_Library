/*!
   \file ESP32Thread.h
   \brief header file to manage the threads in the ESP32
   \author Francesco Fioretti
   \date 04/10/2021
*/

#ifndef ESP32THREADH
#define ESP32THREADH

#include <Arduino.h> // TaskHandle_t
#include "soc/timer_group_struct.h" // TIMERGx class access
#include "soc/timer_group_reg.h" // TIMERGx class access
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "timers.h"
#include <esp_task_wdt.h>

#ifndef INCLUDE_vTaskDelete
#define INCLUDE_vTaskDelete true
#endif

#ifndef INCLUDE_vTaskPrioritySet
#define INCLUDE_vTaskPrioritySet true
#endif

#define DEFAULT_STACK_SIZE 10000
#define DEFAULT_PRIORITY 0

#define CORE_0 0
#define CORE_1 1

#define MAX_PRIORITY 24


#define OFFSET_NANOSEC_EXECUTION 800 ///< offset in nanoseconds to consider when the execution time is measured

extern SemaphoreHandle_t xMutex; ///< mutex to protect the access critical section

/*!
   \brief function to register the reset of watchdog timer of one CPU, reset the watchdog of idle task
          (can be dangerous, if a processor is running the same function always, this processor
          cannot be used for other threads!)
   \param core The core number to reset the watchdog timer for.
*/
void feedTheDog(const uint8_t& core);

/// @brief takes the semaphore with the maximum timeout
/// @param mutex The semaphore handle to take.
void takeSemaphore(SemaphoreHandle_t& mutex);

/// @brief release the semaphore
/// @param mutex The semaphore handle to release.
void giveSemaphore(SemaphoreHandle_t& mutex);

/// @brief class to manage the threads in the ESP32
class Thread {
private:
  xTaskHandle _handle; ///< Handle for the FreeRTOS task
  int _core; ///< Core on which the task will run
  unsigned int _stackDepth; ///< Stack size for the task
  int _priority; ///< Priority of the task
  char* _name; ///< Name of the task
  bool _continuos; ///< Whether the task is continuous
  unsigned int _delay; ///< Delay between task executions in microseconds
  void (*function)(); ///< Function to be executed by the task
  unsigned long _executionTime; ///< Execution time of the task
  bool _running; ///< Whether the task is currently running
  timer _timer; ///< Timer object to measure execution time
  bool _executionTimeRequested; ///< Whether execution time measurement is requested
  bool _timerResetted; ///< Whether the timer has been reset
  bool _hasWDTCapabilities; ///< Whether the task has watchdog timer capabilities
  void task(); ///< The task function

  /// @brief Static function to start the task
  /// @param parm Pointer to the Thread object
  static void taskfun(void* parm) {static_cast<Thread *>(parm)->task();}

  int _timeToWait; ///< Time to wait between task executions

  bool _initialized; ///< Whether the task has been initialized

  bool _executionTimeAvaible; ///< Whether the execution time is available

public:
  /// @brief Constructor for the Thread class
  /// @param f Function to be executed by the task
  /// @param continuos Whether the task is continuous
  /// @param delayMillis Delay between task executions in milliseconds
  /// @param priority Priority of the task
  /// @param name Name of the task
  /// @param core Core on which the task will run
  /// @param stackDepth Stack size for the task
  /// @param hasWDTCapabilities Whether the task has watchdog timer capabilities
  Thread(void (*f)(), const bool& continuos,
                const unsigned long& delayMillis,
                const int& priority = DEFAULT_PRIORITY, std::string name = "",
                const int& core = -1,
                const unsigned int& stackDepth=DEFAULT_STACK_SIZE,bool hasWDTCapabilities = false):
    _core(core), _stackDepth(stackDepth), _priority(priority), _name((char*)name.c_str()),
    _continuos(continuos), _delay(delayMillis), function(f), _running(false),_initialized(false),_executionTimeAvaible(false),
    _timer(NANOS), _executionTimeRequested(false), _timerResetted(false), _hasWDTCapabilities(hasWDTCapabilities), _timeToWait(delayMillis){ {
      if (_continuos) {
        start();
      }
      _timer.start();
    }
    }

  /// @brief Destructor for the Thread class
  ~Thread(){
    closeTask();
  }

  /// @brief Closes the task
  void closeTask();

  /// @brief Starts the task
  void start();

  /// @brief Gets the execution time of the task
  /// @return The execution time of the task
  unsigned long getExecutionTime(){
    if (_executionTimeAvaible) {
      _executionTimeAvaible = false;
      return _executionTime;
    }
    return 0;
  }

  /// @brief Checks if the execution time is available
  /// @return Whether the execution time is available
  bool executionTimeIsAvailable(){return _executionTimeAvaible;}

  /// @brief Sets the flag to measure the execution time of the task
  void setExecutionTimeRequested(){_executionTimeRequested = true;}


  /// @brief Sets the delay between task executions
  /// @param delay The delay in milliseconds
  void setDelay(const int& delay){_delay = delay;}

  /// @brief Suspends the task
  void suspend(){_running=false; vTaskSuspend(_handle);}

  /// @brief Resumes the task
  void run(){_running=true; vTaskResume(_handle);}

  /// @brief set the priority of the task
  void setPriority(const int& priority){if(_initialized){_priority = priority;vTaskPrioritySet(_handle, _priority);}}
};

#endif /* end of include guard: ESP32THREADH */