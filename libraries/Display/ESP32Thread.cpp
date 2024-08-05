/*!
   \file ESP32Thread.cpp
   \brief description file to manage the threads in the ESP32
   \author Francesco Fioretti
   \date 04/10/2021
   
*/

#ifndef ESP32THREADCPP
#define ESP32THREADCPP

#include "ESP32Thread.h"

SemaphoreHandle_t xMutex = xSemaphoreCreateMutex(); ///< mutex to protect the access critical section

/*!
   \brief function to register the reset of watchdog timer (can be dangerous,
          if a processor is running the same function always, this processor
          cannot be used for other threads!)
   \param core The core number to reset the watchdog timer for.
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

/// @brief takes the semaphore with the maximum timeout
/// @param mutex The semaphore handle to take.
void takeSemaphore(SemaphoreHandle_t& mutex){
  xSemaphoreTake(mutex, portMAX_DELAY);
}

/// @brief release the semaphore
/// @param mutex The semaphore handle to release.
void giveSemaphore(SemaphoreHandle_t& mutex){
  xSemaphoreGive(mutex);
}

/// @brief function to initialize the thread on the core
void Thread::start(){
  if (_core != -1) {
    xTaskCreatePinnedToCore(&taskfun, _name, _stackDepth, this, _priority, &_handle, _core);
  } else {
    xTaskCreate(&taskfun, _name, _stackDepth, this, _priority, &_handle);
  }
  _initialized = true;
  delay(500);
}

/// @brief function to delete the thread
void Thread::closeTask(){
  #if INCLUDE_vTaskDelete
    vTaskDelete(_handle);
  #endif
  return;
}


/// @brief function that will be executed by the thread
void Thread::task(){
  // only one time
  #pragma region start_of_task
    // if the thread is not running, suspend it
    if (!_running) {
      suspend();
    }
    if (!_hasWDTCapabilities) {
      esp_task_wdt_delete(NULL);
    } else {
      esp_task_wdt_add(NULL);
    }
  #pragma endregion start_of_task

  // run the task
  while (true) {
    if (_executionTimeRequested) {
      _timer.reset();
      _timerResetted = true;
    }
    
    
    // execute the function
    this->function();
    // delete task if requested
    if (!_continuos) {
      closeTask();
    }
    
    // reset the watchdogs
    feedTheDog(xPortGetCoreID());
    if (esp_task_wdt_status(NULL) == ESP_OK) {
      esp_task_wdt_reset();
    }
    // delay Task if necessary
    if (_delay > 1) {
      _timeToWait = _delay - _timer.readMilliseconds();
    } else {
      _timeToWait = _delay;
    }

    if (_timeToWait > 0) {
      delay(_timeToWait);
    }
    if (_executionTimeRequested) {
      if (_timerResetted) {
        _executionTime = _timer.read() - OFFSET_NANOSEC_EXECUTION;
        _timerResetted = false;
        _executionTimeRequested = false;
        _executionTimeAvaible = true;
      }
    }
    
  }
}

#endif /* end of include guard: ESP32THREADCPP */