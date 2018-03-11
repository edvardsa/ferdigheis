#include "time.h"
#include "logic.h"
#include "elev.h"
#include <stdio.h>
#include <assert.h>
#include "channels.h"
#include "io.h"
#include "fsm.h"

/*
Global variables for the logic module
*/
int order_matrix[3][4] ={
//	 1 2 3 4   // Floor    
	{0,0,0,0}, // Up 
	{0,0,0,0}, // Down
	{0,0,0,0}  // Panel
	
};

int current_floor = 0;

int next_floor = 0;

int current_dir = 0;

int last_dir = 0;

/*
Set and get functions for the global variables
*/
void set_current_floor(int floor){
	current_floor = floor;
}

int get_current_floor(){
	return current_floor;
}

void set_next_floor(int floor){
	next_floor = floor;
}

int get_next_floor(){
	return next_floor;
}

void set_current_dir(int dir) {
	current_dir = dir;
}

int get_current_dir(){
	return current_dir;
}

void set_last_dir(int dir){
	last_dir = dir;
}

int get_last_dir(){
	return last_dir;
}

int check_order(){
	for (int floor = 0; floor < N_FLOORS; floor++){
	    for (int button = 0; button <= BUTTON_COMMAND; button++) {
	        if (floor == 0 && button == 1) continue;
	        if (floor == 3 && button == 0) continue;  //ignoring the undefined button values
	        if (order_matrix[button][floor] == 0 ){
	        	if (elev_get_button_signal(button,floor) == 1){
	        		order_matrix[button][floor] = 1;
	        		elev_set_button_lamp(button, floor, 1);
	        		return 1;
	        	}
	        }
	    }
	}
    return 0;
}


int check_order_above(){
	for (int floor = get_current_floor(); floor < N_FLOORS; floor++) {
		if(order_matrix[0][floor] == 1 || order_matrix[2][floor] == 1)  {
			set_next_floor(floor);
			return 1;
		}
	}
	for (int floor = N_FLOORS-1; floor > get_current_floor(); floor--){
		if(order_matrix[1][floor] == 1){ //checks down-button, starting from the top floor
			set_next_floor(floor);
			return 1;
		}
	}
	return 0;	
}

int check_order_below(){
	for (int floor = get_current_floor(); floor >= 0; floor--) {
		if(order_matrix[1][floor] == 1 || order_matrix[2][floor] == 1) {
			set_next_floor(floor);
			return 1;
		}
	}
	for (int floor = 0; floor < get_current_floor(); floor++){
		if(order_matrix[0][floor] == 1 ){ //checks up-button, starting from the first floor
			set_next_floor(floor);
			return 1;
		}
	}
	return 0;	
}

void delete_order(int floor){
	for (int order_type = 0; order_type <= BUTTON_COMMAND; order_type++){
		if ((floor == 0 && order_type == 1) || (floor == 3 && order_type == 0) || floor == -1) continue;
		order_matrix[order_type][floor] = 0;
		elev_set_button_lamp(order_type, floor, 0);
	}
}


void time_delay(int number_of_seconds){
    clock_t start_time = clock();
    while ((clock() - start_time)/CLOCKS_PER_SEC < number_of_seconds){ //delays the elevator while updating the order_matrix and checking the stop-button
    	check_order();
        if (elev_get_stop_signal()){
            break;
        }
    }
}


void set_priority(){ //updates next_floor based on the most favorable order given current_floor, last_dir and current_dir
	if (get_current_dir() == DIRN_UP){
		check_order_above();
		return;
	}
	else if (get_current_dir() == DIRN_DOWN){
		check_order_below();
		return;
	}
	else if (get_last_dir() == DIRN_UP && get_current_floor() != 3){ //prioritze orders above, before optionally checking orders below
		if (check_order_above()==1){ 
			return;
		}
		else {
			check_order_below(); 
			return;
		}
	}
	else if (get_last_dir() == DIRN_DOWN && get_current_floor() != 0){ //prioritze orders below, before optionally checking order above
		if (check_order_below()==1){
		return;
		}
		else {
			check_order_above();
			return;
		}
	}
	else if (get_current_floor() == 0 && get_current_dir() == DIRN_STOP){
		check_order_above();
		return;
	}
	else if (get_current_floor() == 3 && get_current_dir() == DIRN_STOP){
		check_order_below();
		return;
	}
}

void print_current(){
	printf("Current floor: %d",get_current_floor());
}

void print_next(){
	printf("Next floor: %d \n",get_next_floor());
}

void print_matrix(){
	printf("\n");
	for(int i = 0; i < 3; i++){
		for (int j= 0; j <4; j++){
			printf(" %d", order_matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int sum_of_orders(){
	int sum = 0;
	for(int i = 0; i < 3; i++){
		for (int j= 0; j <4; j++){
			sum += order_matrix[i][j];
		}
	}
	return sum;
}
