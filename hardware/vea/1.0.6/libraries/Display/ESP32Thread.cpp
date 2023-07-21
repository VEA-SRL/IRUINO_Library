/*!
   \file ESP32Thread.cpp
   \brief description file to manage the threads in the ESP32
   \author Francesco Fioretti
   \date 04/10/2021
*/

#ifndef ESP32THREADCPP
#define ESP32THREADCPP

#include "ESP32Thread.h"

/*!
   \brief function to register the reset of watchdog timer (can be dangerous,
          if a processor is running the same function always, this processor
          cannot be used for other threads!)
*/
void feedTheDog(const uint8_t& core){
  switch (core) {
    // feed dog 0
    case 0:
      TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE; // write enable
      TIMERG0.wdt_feed=1;                       // feed dog
      TIMERG0.wdt_wprotect=0;                   // write protect
      break;

    // feed dog 1
    case 1:
      TIMERG1.wdt_wprotect=TIMG_WDT_WKEY_VALUE; // write enable
      TIMERG1.wdt_feed=1;                       // feed dog
      TIMERG1.wdt_wprotect=0;                   // write protect
      break;
  }
}


void Thread::start(){
  if (_core!=-1) {
    xTaskCreatePinnedToCore(&taskfun, _name, _stackDepth, this, _priority, &_handle, _core);
  }else{
    xTaskCreate(&taskfun, _name, _stackDepth, this, _priority, &_handle);
  }
  delay(500);
}

void Thread::closeTask(){
  #if INCLUDE_vTaskDelete
    vTaskDelete(_handle);
  #endif
    return;
}

void Thread::task(){
  if (!_running) {
    suspend();
  }
  while(true){
    _timer.reset();
    //do function
    this->function();
    //delete task if requested
    if (!_continuos) {
      closeTask();
    }
    //feed watchdog or wait the requested time
    if(_delay<=10){
      feedTheDog(xPortGetCoreID());
      if (_delay > 0) {
        delay(_delay);
      }
    }else{
      vTaskDelay(pdMS_TO_TICKS( 10 )/portTICK_PERIOD_MS);
    }
    _executionTime = _timer.read();

  }
}

#endif /* end of include guard: ESP32THREADCPP */
