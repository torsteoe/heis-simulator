/**
 * @file
 * @brief A library for accessing queues and
 * doing operations on queue
 */
#ifndef QUEUE_H__   /*include guard*/
#define QUEUE_H__


#include "elev.h"
#include <stdio.h>


//////////////////////////////
////      FUNCTIONS       ////
//////////////////////////////

/**
  @brief Fetches array of order_lists.
  @return pointer to dynamically allocated array containing all orders.
*/
int ** queue_get_orders();


/**
  @brief Fetches previous or current floor if elevator on floor.
  @return floor ranging from @c 0 to @c 3.
*/
int queue_get_last_floor_visited();

/**
  @brief Fetches priority order, which is the first floor ordered that has still not been visited since order.
  @return floor ranging from @c 0 to @c 3.
*/
int queue_get_priority_order();


/**
  @brief Check if elevator should stop at floor.
  @param[in] direction -1 for down, 1 for up.
  @return @c 1 to signal stop, @c 0 to signal no stop
*/
int queue_should_I_stop_at_floor(int direction);

/**
  @brief Delete orders at current floor.
*/
void queue_delete_floor_orders();

/**
  @brief Iterate buttons and update orders accordingly.
*/
void queue_update_orders();

/**
  @brief Print all orders in nice format to console. For debugging purposes.
*/
void queue_print_orders(); 

/**
  @brief Set all order lists to default state.
*/
void queue_reset_orders();

/**
  @brief Check if there are orders in direction that elevator will stop at. Safety measure in case elevator should be in a moving state with no orders to stop it.
  @param[in] direction @c -1 for down, @c 1 for up.
  @return @c 1 to signal orders exist, @c 0 to signal no orders.
*/
int queue_orders_in_direction(int direction); 


#endif //#ifndef QUEUE_H__
