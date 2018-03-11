#include "elev.h"
#include "fsm.h"
#include "logic.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");
    start();
    print_current();
    while (1) {
        if (get_current_floor() == get_next_floor()){
            stop_floor();
        }
        if(elev_get_stop_signal()){
            stop_button();
        }
 
        if(sum_of_orders() > 0){
            move();
        }
    }
    return 0;
}

