/**
 * @file
 * @brief A library for managing the state machine
 */
#ifndef FSM_H__	/*include guard*/
#define FSM_H__

#include "elev.h"
#include "queue.h"
#include "timer.h"
#include "doors.h"


//////////////////////////////
////      FUNCTIONS       ////
//////////////////////////////

/**
  @brief Initialize elevator: Starts timer, initializes orders and moves elevator to initial position.
  @return @c 1 on success, @c 0 if failed initiation.
*/
int FSM_init();

/**
  @brief Perform action based on what state the elevator is in. Change state if required by elev.h or queue.h.
*/
void FSM_update_state();


#endif //#ifndef FSM_H__