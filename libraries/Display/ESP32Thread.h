/*!
   \file ESP32Thread.h
   \brief header file to manage the threads in the ESP32
   \author Francesco Fioretti
   \date 04/10/2021
*/

#ifndef ESP32THREADH
#define ESP32THREADH



#include <Arduino.h> //TaskHandle_t
#include "soc/timer_group_struct.h" //TIMERGx class access
#include "soc/timer_group_reg.h" //TIMERGx class access
#include "freertos/FreeRTOS.h" //xTaskHandle
#include "freertos/task.h" //xTaskHandle
#include "timers.h" //timer class
#include <string> //std::string

#ifndef INCLUDE_vTaskDelete
#define INCLUDE_vTaskDelete true
#endif

#define DEFAULT_STACK_SIZE 10000
#define DEFAULT_PRIORITY 0

/*!
   \brief function to register the reset of watchdog timer (can be dangerous,
          if a processor is running the same function always, this processor
          cannot be used for other threads!)
*/
void feedTheDog(const uint8_t& core);




class Thread {
private:
  xTaskHandle _handle;
  int _core;
  unsigned int _stackDepth;
  int _priority;
  char* _name;
  bool _continuos;
  unsigned int _delay;
  void (*function)();
  unsigned long _executionTime;
  bool _running;
  timer _timer;
  bool _executionTimeRequested;
  bool _timerResetted;
  void task();

  static void taskfun(void* parm) {static_cast<Thread *>(parm)->task();}


public:
  Thread(void (*f)(), const bool& continuos,
                const unsigned long& delayMillis,
                const int& priority = DEFAULT_PRIORITY, std::string name = "",
                const int& core = -1,
                const unsigned int& stackDepth=DEFAULT_STACK_SIZE) :
    _core(core), _stackDepth(stackDepth), _priority(priority), _name((char*)name.c_str()),
    _continuos(continuos), _delay(delayMillis), function(f), _running(false),
    _timer(2), _executionTimeRequested(false), _timerResetted(false) {
      if (_continuos) {
        start();
      }
      _timer.start();
    }

  ~Thread(){
    closeTask();
  }

  void closeTask();

  void start();

  unsigned long getExecutionTime(){
    return _executionTime;
  }
  void setDelay(const int& delay){_delay = delay;}

  void suspend(){_running=false; vTaskSuspend(_handle);}
  void run(){_running=true; vTaskResume(_handle);}
};



#endif /* end of include guard: ESP32THREADH */
