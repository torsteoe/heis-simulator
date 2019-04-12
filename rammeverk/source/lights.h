/**
 * @file
 * @brief A simple library for doing operations on lights.
 */
#ifndef LIGHTS_H__	/*include guard*/
#define LIGHTS_H__


#include "elev.h"


//////////////////////////////
////      FUNCTIONS       ////
//////////////////////////////

/**
  @brief Updates all lights except door lamp and stop lamp. 
  @param[in] last_floor_visited floor last visited or current floor if elevator on floor, used to set floor indicator lamp.
  @param[in, out] pp_orders pointer to dynamically allocated array containing all orders. Used to set button lights. Freed at end of function. 
*/
void lights_update_lights(int last_floor_visited, int ** pp_orders);


#endif //#ifndef LIGHTS_H__