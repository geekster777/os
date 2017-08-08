/**
  * A wrapper around the clock and timer interrupts. Allows you to register
  * timed interrupts, and access the date time.
  */
#include<stdint.h>

#ifndef _CLOCK_H_
#define _CLOCK_H_

typedef struct datetime {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
} datetime;

/**
  * Name:   clock_init
  * Args:   None
  * Return: None
  *
  * Initializes the timer for callbacks
  */
void clock_init();

/**
  * Name:   clock_get_datetime
  * Args:   None
  * Return: datetime - A struct containing the current time.
  *
  * Gives a structure containing the current time as of the function call.
  */
datetime clock_get_datetime();

/**
  * Name:   clock_delay
  * Args:   uint32_t - The number of milliseconds to wait for
  * Return: None
  *
  * Returns from this function after the allotted milliseconds have been 
  * waited. This is a blocking function.
  */
void clock_delay(uint32_t ms);

/**
  * Name:   clock_register_callback
  * Args:   uint32_t - The milliseconds to wait before calling the callback
  *         void f() - The function to be called after waiting
  * Return: uint16_t - The id of the callback for use in removal later
  * 
  * Calls the supplied callback continuously in intervals of provided ms.
  */
uint16_t clock_register_callback(uint32_t ms, void (*callback)());

/**
  * Name:   clock_remove_callback
  * Args:   uint16_t - The id of the callback to be removed
  * Return: None
  *
  * Removes a callback given a supplied id. The id comes from the 
  * clock_register_callback function. This is useful if you only want a callback
  * to be run a set number of times.
  */
void clock_remove_callback(uint16_t id);

#endif
