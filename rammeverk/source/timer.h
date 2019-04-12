/**
 * @file
 * @brief A simple library for doing operations with timer
 */
#ifndef TIMER_H__	/*include guard*/
#define TIMER_H__




//////////////////////////////
////      FUNCTIONS       ////
//////////////////////////////

/**
  @brief Reset timer.
*/
void timer_start();

/**
  @brief Check if time passed since timer was reset is more than 3 seconds.
  @return @c 1 if timer is out, @c 0 otherwise.
*/
int timer_expired();

#endif //#ifndef TIMER_H__