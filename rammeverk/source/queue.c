/**
 * @file
 * @brief Implementation file for the queue module.
 */
#include "queue.h"
#include "malloc.h"

//////////////////////////////
////       VARIABLES      ////
//////////////////////////////

const int ORDER_SIZE = 4;

static int last_floor_visited = -1;
static int up_orders[ORDER_SIZE]; //Each position represents a floor, 0 if no order, 1 if order.
static int down_orders[ORDER_SIZE]; //Each position represents a floor, 0 if no order, 1 if order.
static int panel_orders[ORDER_SIZE]; //Each position represents a floor, 0 if no order, 1 if order.

//Each position represents priority in queue. Default -1, otherwise floor number.
static int priority_orders[ORDER_SIZE]; 


//////////////////////////////
//HELPER FUNCTION DECLARATIONS
//////////////////////////////

static void m_add_up_orders();
static void m_add_down_orders();
static void m_add_panel_orders();
static void m_add_priority_orders(int floor);


//////////////////////////////
////       FUNCTIONS      ////
//////////////////////////////

int ** queue_get_orders() {
    int ** pp_orders = (int **) malloc(3*sizeof(up_orders));
    pp_orders[0] = up_orders;
    pp_orders[1] = down_orders;
    pp_orders[2] = panel_orders;
    return pp_orders;
}

int queue_get_last_floor_visited() {
    return last_floor_visited;
}

int queue_get_priority_order() {
    return priority_orders[0];
}

int queue_should_I_stop_at_floor(int direction) {
    int floor = elev_get_floor_sensor_signal();

    if (floor == -1) { //safety measure in case of call between floors.
        return 0;
    }

    if (priority_orders[0] == floor) { //checks if floor first in priority list.
        return 1;
    }

    //checks if in appropriate lists depending on direction.
    if (direction==DIRN_DOWN) { 
        return (down_orders[floor] || panel_orders[floor]); 
    }
    else  { //direction is up
        return (up_orders[floor] || panel_orders[floor]);
    }

}

void queue_delete_floor_orders() {
    int floor = last_floor_visited;
        
    up_orders[floor] = 0;
    down_orders[floor] = 0;
    panel_orders[floor] = 0;
    
    //iterates priority_orders:
    //if floor in priority_orders all other floors are moved one spot up in priority.
    for (int i = 0; i<ORDER_SIZE; i++) {
        if (priority_orders[i] == floor) {
            for (int idx = i; idx<ORDER_SIZE-1; idx++) {
                priority_orders[idx] = priority_orders[idx+1];
            }
            priority_orders[ORDER_SIZE-1] = -1;
            break;
        
        }
    }
    
}

int queue_orders_in_direction(int direction) {

    int orders_exist = 0;

    //iterates the priority list for given direction, and sees if orders exist in that direction.
    for (int priority_idx = 0; priority_idx<ORDER_SIZE; priority_idx++) {
        if (direction == DIRN_DOWN) {
            for (int floor = 0; floor<last_floor_visited; floor++) {
                orders_exist += (priority_orders[priority_idx]==floor);
            }
        } else {
            for (int floor = last_floor_visited+1; floor<ORDER_SIZE; floor++) {
                orders_exist += (priority_orders[priority_idx]==floor);
            } 
        }
    }
    
    return (orders_exist>0);
}

void queue_update_orders() {

    int current_floor = elev_get_floor_sensor_signal();

    if (current_floor != -1){ //only update last_floor_visited if on floor.
        last_floor_visited = current_floor;
    }
    
    m_add_down_orders();
    m_add_up_orders();
    m_add_panel_orders();
    //priority_orders updated in the three functions above
}

void queue_reset_orders() {
    for (int i = 0; i<ORDER_SIZE; i++) {
            up_orders[i] = 0;
            down_orders[i] = 0;
            panel_orders[i] = 0;
            priority_orders[i] = -1;
    }
}

void queue_print_orders() {

    printf("Here come the orders: \n") ;

    printf("Uporders: \n");
    for (int i = 0; i<ORDER_SIZE-1; i++) {
        printf("%d: %d,\t", i, up_orders[i]); 
    }

    printf("\ndown_orders: \n\t");
    for (int i = 1; i<ORDER_SIZE; i++) {
        printf("%d: %d,\t", i, down_orders[i]); 
    }

    printf("\npanel_orders: \n");
    for (int i = 0; i<ORDER_SIZE; i++) {
        printf("%d: %d,\t", i, panel_orders[i]); 
    }

    printf("\npriority_orders: \n");
    for (int i = 0; i<ORDER_SIZE; i++) {
        printf("%d: %d,\t", i, priority_orders[i]); 
    }
    printf("\n");

}


//////////////////////////////
////   HELPER FUNCTIONS   ////
//////////////////////////////

//checks button signal for every floor and updates up_orders respectively.
void m_add_up_orders() {
    for (int i = 0; i<ORDER_SIZE-1; i++) {
        if (elev_get_button_signal(BUTTON_CALL_UP, i)) {
            up_orders[i] = 1;
            m_add_priority_orders(i);
        }    
    }
}

//checks button signal for every floor and updates down_orders respectively.
void m_add_down_orders() {
    for (int i = 1; i<ORDER_SIZE; i++) {
        if (elev_get_button_signal(BUTTON_CALL_DOWN, i)) {
            down_orders[i]=1;
            m_add_priority_orders(i);
        }
    }
}

//checks button signal for every floor and updates panel_orders respectively.
void m_add_panel_orders() {
    for (int i = 0; i<ORDER_SIZE; i++) {
        if (elev_get_button_signal(BUTTON_COMMAND, i)) {
            panel_orders[i] = 1;
            m_add_priority_orders(i);
        }
        
    }
}

//receives a floor, checks if in list, adds if not.
void m_add_priority_orders(int floor) {
    //Iterate priority_orders, break if floor in list.
    //Add floor to priority list at first empty spot (first spot where value == -1), then break.
    for (int i = 0; i<ORDER_SIZE;i++) {
        if (priority_orders[i] == floor) {
            break;
        }
        if (priority_orders[i] == -1) {
            priority_orders[i] = floor;
            break;
        }
    }
}
