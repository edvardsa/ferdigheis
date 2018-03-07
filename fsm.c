
// her kommer koden knytt til tilstandsmaskinen
#include "fsm.h"
#include "logic.h"
#include "elev.h"
#include <stdio.h>

#include "channels.h"
#include "io.h"


void move(){
    if(get_next_floor()== get_current_floor()){
        set_current_dir(DIRN_STOP);
        elev_set_motor_direction(DIRN_STOP);
        return;
    }

    else if (get_next_floor() > get_current_floor()){
        set_current_dir(DIRN_UP);
        elev_set_motor_direction(DIRN_UP);    
    }

    else if(get_next_floor() < get_current_floor()){
        set_current_dir(DIRN_DOWN);
        elev_set_motor_direction(DIRN_DOWN);    
    }

    while(elev_get_floor_sensor_signal() != get_next_floor()) {
        if (elev_get_stop_signal() == 1){
            return;
        }
        
        if (elev_get_floor_sensor_signal() == 0 ){
            set_current_floor(0);
            elev_set_floor_indicator(0);
        }
        else if (elev_get_floor_sensor_signal() == 1 ){
            set_current_floor(1);
            elev_set_floor_indicator(1);
        }
        else if (elev_get_floor_sensor_signal() == 2 ){
            set_current_floor(2);
            elev_set_floor_indicator(2);
        }
        else if (elev_get_floor_sensor_signal() == 3 ){
            set_current_floor(3);
            elev_set_floor_indicator(3);
        }
        

        check_order();
        set_priority();
       
    }
    set_priority();
    set_last_dir(get_current_dir());
    set_current_dir(DIRN_STOP);
    if (elev_get_floor_sensor_signal() != -1){
        set_current_floor(elev_get_floor_sensor_signal());
        elev_set_floor_indicator(elev_get_floor_sensor_signal());
    }
    
}


void start(){
    if(elev_get_floor_sensor_signal() == -1) {
            elev_set_motor_direction(DIRN_UP);
        }
        while(1){
            if (elev_get_floor_sensor_signal() != -1){
                set_current_floor(elev_get_floor_sensor_signal());
                set_next_floor(get_current_floor());
                set_current_dir(DIRN_STOP);
                set_last_dir(DIRN_UP);
                elev_set_motor_direction(DIRN_STOP);
                elev_set_floor_indicator(elev_get_floor_sensor_signal());
                break;
            }
        }
}


void stop_floor(){
    elev_set_motor_direction(DIRN_STOP);
    elev_set_door_open_lamp(1);
    delete_order(get_current_floor());
    time_delay(3);
    elev_set_door_open_lamp(0);
    while (sum_of_orders() == 0){
        if (elev_get_stop_signal()){
            return;
        }
        check_order();
    }
    set_priority();
}


void stop_button() {
    set_last_dir(get_current_dir());
    set_current_dir(DIRN_STOP);
    elev_set_motor_direction(DIRN_STOP);
    set_next_floor(get_current_floor());
    for (int floor = 0; floor < N_FLOORS; floor++){ //delete all orders
        delete_order(floor);
    }
    while(elev_get_stop_signal()){ //set light while button is pressed
        elev_set_stop_lamp(1);
    }
    elev_set_stop_lamp(0);
    if(elev_get_floor_sensor_signal() != -1) {
        elev_set_door_open_lamp(1);
        time_delay(3);
        elev_set_door_open_lamp(0);

    }
    while(sum_of_orders() == 0) {
        check_order();
    }
    set_priority();
    if (get_current_floor() == get_next_floor()){
        if (get_last_dir() == DIRN_UP && elev_get_floor_sensor_signal() == -1 && get_current_floor() != 3){
            set_current_floor(get_current_floor() +1);
            }
        else if (get_last_dir() == DIRN_DOWN && elev_get_floor_sensor_signal() == -1 && get_current_floor() != 0){
            set_current_floor(get_current_floor() - 1);
        }
    }
}

