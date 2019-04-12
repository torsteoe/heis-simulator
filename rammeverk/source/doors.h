/**
 * @file
 * @brief A simple library for controlling the doors of the elevator
 * 
 */
#ifndef DOORS_H__	/*include guard*/
#define DOORS_H__


#include "elev.h"


//////////////////////////////
////      FUNCTIONS       ////
//////////////////////////////

/**
  @brief Change state of doors (open/closed).
  @param[in] close @c 1 to close door, @c 0 to open.
*/
void doors_change_state(int close);




#endif //#ifndef DOORS_H__