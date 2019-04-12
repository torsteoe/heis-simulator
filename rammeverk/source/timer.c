/**
 * @file
 * @brief Implementation file for the timer module.
 */
#include "timer.h"
#include <time.h>

//////////////////////////////
////      VARIABLES       ////
//////////////////////////////

const double SECONDS = 3;
static clock_t clock_var; //Variable that will be used for storing clock ticks since program start.


//////////////////////////////
////      FUNCTIONS       ////
//////////////////////////////

void timer_start() {
    //set to current clock ticks since program started.
    clock_var = clock(); 
}
int timer_expired() {
    //make new variable containing ticks since program start.
    clock_t clock_now = clock();

    //compare with clock_var to know if three seconds have passed.
    int difference = (clock_now-clock_var);

    //every 72nd minute, clock variable will overflow. As a safety measure we will open door and reset clock. Worst case: door open for 6 seconds every 72nd minute.
    if (difference<0) {
        clock_var = clock();
        return 0;
    }

    return (difference>=SECONDS*CLOCKS_PER_SEC); 
}