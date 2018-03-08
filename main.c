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
    printf("Kom forbi init\n");
    print_current();
    while (1) {
        if (elev_get_floor_sensor_signal() == get_next_floor()){
            stop_floor();
        }
        if(elev_get_stop_signal()){
            stop_button();
        }
        print_matrix();
        print_current();
        print_next();
        printf("%d \n", sum_of_orders());
        if(sum_of_orders() > 0){
            move();
        }
        printf("current dir: %d \n",get_current_dir());
        printf("last dir: %d \n",get_last_dir());
    }
    return 0;
}

