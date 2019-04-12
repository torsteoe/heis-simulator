/**
 * @file
 * @brief Implementation file for the lights module.
 */
#include "lights.h"
#include "malloc.h"


//////////////////////////////
//HELPER FUNCTION DECLARATIONS
//////////////////////////////

static void m_set_floor_indicator_lights(int last_floor_visited);
static void m_set_order_indicator_lights(int ** pp_orders);


//////////////////////////////
////      FUNCTIONS       ////
//////////////////////////////

void lights_update_lights(int last_floor_visited, int ** pp_orders) {
    m_set_floor_indicator_lights(last_floor_visited);
    m_set_order_indicator_lights(pp_orders);
}


//////////////////////////////
////   HELPER FUNCTIONS   ////
//////////////////////////////

void m_set_floor_indicator_lights(int last_floor_visited) {
    if (last_floor_visited!=-1) { //Safety measure: last_floor_visited should never be -1
        elev_set_floor_indicator(last_floor_visited);
    }
    
}

//Takes in array of all order lists. Iterates lists to set button lights.
void m_set_order_indicator_lights(int ** pp_orders) {
    
    int * up = pp_orders[0];
    int * down = pp_orders[1];
    int * panel = pp_orders[2];

    for (int i = 0; i<3; i++) {
        elev_set_button_lamp(BUTTON_CALL_UP, i, up[i]);
    }
    for (int i = 1; i<4; i++) {
        elev_set_button_lamp(BUTTON_CALL_DOWN, i, down[i]);
    }
    for (int i = 0; i<4; i++) {
        elev_set_button_lamp(BUTTON_COMMAND, i, panel[i]);
    }

    free(pp_orders);
    pp_orders = NULL;
}