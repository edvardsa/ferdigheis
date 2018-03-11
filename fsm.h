#ifndef _INCLUDE_FSM_H_
#define _INCLUDE_FSM_H_
// Headerfile for the statemachine


//runs the elevator upwards until it reaches a floor. If it is already in a floor, does nothing
void start();

//moves the elevator up or down based on where next_floor is compared to current floor
// sets motordirection "UP" or "DOWN"
void move();

//stops the elevator when it reaches next floor, lights the doorlamp for 3 seconds and sets idle state if sum_orders = 0
void stop_floor();

//initialized by stop button. Deletes all orders and sets door open lamp if the elevator is in a floor.
// sets idle state
void stop_button();

// idle state. Check orders while sum_orders = 0
void idle();

#endif
