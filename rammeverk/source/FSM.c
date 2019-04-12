/**
 * @file
 * @brief Implementation file for the state machine module.
 */
#include "FSM.h"
#include "stdlib.h"
#include <stdio.h>


//////////////////////////////
////       VARIABLES      ////
//////////////////////////////

/**
  States for the state machine.
*/
typedef enum STATES {
    NOTMOVINGATFLOOR, 
    MOVINGDOWN,
    MOVINGUP,
    STOPSTATE,
    NOTMOVINGMIDDLE //mellom etasjer i ro.
} state;

static state now_state = NOTMOVINGMIDDLE;
static int m_timeout;
static elev_motor_direction_t previous_direction; //-1 for down, 1 for up


//////////////////////////////
//HELPER FUNCTION DECLARATIONS
//////////////////////////////

static int m_ordered_to_same_floor();
static int m_ordered_up();
static int m_ordered_down();
static int m_on_floor();
static int m_orders_exist();


//////////////////////////////
////       FUNCTIONS      ////
//////////////////////////////

int FSM_init() { 

// Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 0;
    }

    printf("Your elevator is ready.\n");

    elev_set_motor_direction(DIRN_DOWN);
    while (elev_get_floor_sensor_signal()==-1);
    elev_set_motor_direction(DIRN_STOP);
    now_state = NOTMOVINGATFLOOR;
    previous_direction = DIRN_DOWN;

    doors_change_state(0);
    timer_start();
    queue_reset_orders();

    return 1;
} 

void FSM_update_state() {


    switch (now_state) {
       
        case NOTMOVINGATFLOOR:
            m_timeout = timer_expired(); //saving timer_expired() value in case timer changes during function.
        
            elev_set_motor_direction(DIRN_STOP);
            doors_change_state(m_timeout); //closes/opens door based on timer.
            
            if (elev_get_stop_signal()) {
                now_state = STOPSTATE;
                break;
            }

            //Enters this if-block if order on this floor exists in any order_queue, resets timer and delets orders to this floor.
            else if (m_ordered_to_same_floor()) {
                now_state = NOTMOVINGATFLOOR; //can be removed but makes code easier to read.
                timer_start();
                queue_delete_floor_orders();
                break;
            }
   
            //enters this block when door has been open for 3 seconds.
            else if (m_timeout) {
                if (m_ordered_down()) {
                    now_state = MOVINGDOWN;
                    previous_direction = DIRN_DOWN;
                }
                
                else if (m_ordered_up()) {
                    now_state = MOVINGUP;
                    previous_direction = DIRN_UP;
                }
            }
  
            break;

        case MOVINGDOWN:
            elev_set_motor_direction(DIRN_DOWN);

            if (elev_get_stop_signal()) { 
                now_state = STOPSTATE;
            }

            else if (m_on_floor()) {

                if (queue_should_I_stop_at_floor(DIRN_DOWN)) {
                    now_state = NOTMOVINGATFLOOR;
                    timer_start();
                }

                //safety measure: The elevator should never be stuck in this state if no orders in this direction.
                else if (!queue_orders_in_direction(DIRN_DOWN)) {
                    now_state = NOTMOVINGATFLOOR;
                }
            }

            
            break;

        case MOVINGUP:
            
            elev_set_motor_direction(DIRN_UP);

            if (elev_get_stop_signal()) {
                now_state = STOPSTATE;
            }

            else if (m_on_floor()) {

                if (queue_should_I_stop_at_floor(DIRN_UP)) {
                    now_state = NOTMOVINGATFLOOR;
                    timer_start();
                }

                //safety measure: The elevator should never be stuck in this state if no orders in this direction.
                else if (!queue_orders_in_direction(DIRN_UP)) {
                    now_state = NOTMOVINGATFLOOR;
                }
            }
            break;

        //in this state as long as STOP button is pressed.
        case STOPSTATE:
            elev_set_motor_direction(DIRN_STOP);
            elev_set_stop_lamp(1);
            timer_start(); //resets timer.
            queue_reset_orders();   
            

            doors_change_state(!(m_on_floor())); //only open door if on floor.
            

            if (!elev_get_stop_signal() && m_on_floor()) {
                elev_set_stop_lamp(0);
                now_state = NOTMOVINGATFLOOR ;
            }

            else if (!elev_get_stop_signal() && !(m_on_floor()) ){
                elev_set_stop_lamp(0);
                now_state = NOTMOVINGMIDDLE;
            }

            break;

        case NOTMOVINGMIDDLE:

            if (elev_get_stop_signal()) {
                now_state = STOPSTATE;
            }
            else {
                if (m_ordered_down()) {
                    now_state = MOVINGDOWN;
                }  
                else if (m_ordered_up()) {
                    now_state = MOVINGUP;
                }

                //If ordered back to last floor visited, previous direction tells us if we are above or below order.
                //Previous direction only updated when leaving a floor.
                else if (m_ordered_to_same_floor()) {
                    if (previous_direction == DIRN_DOWN) {
                        now_state = MOVINGUP;
                    } else {
                        now_state = MOVINGDOWN;
                    }
                }
            }
            
            break;

    }
}


//////////////////////////////
////   HELPER FUNCTIONS   ////
//////////////////////////////

int m_ordered_to_same_floor() {
    if (m_on_floor()) {
        return (queue_should_I_stop_at_floor(DIRN_DOWN) || queue_should_I_stop_at_floor(DIRN_UP));
    }
    return (queue_get_priority_order() == queue_get_last_floor_visited());
}
int m_ordered_up() {
    return (m_orders_exist() && queue_get_priority_order() > queue_get_last_floor_visited());
}
int m_ordered_down() {
    return (m_orders_exist() && queue_get_priority_order() < queue_get_last_floor_visited());
}
int m_on_floor() {
    return (elev_get_floor_sensor_signal() != -1);
}
int m_orders_exist() {
    return (queue_get_priority_order() != -1);
}