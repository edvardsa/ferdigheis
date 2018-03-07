

//extern int order_matrix[3][4];
/*
____SET AND GET FUNCTIONS____
*/
void set_current_floor(int floor);
int get_current_floor();
void set_next_floor(int floor);
int get_next_floor();
void set_current_dir(int dir);
int get_current_dir();
void set_last_dir(int dir);
int get_last_dir();
/*
___FUNCTIONS FOR SETTING NEXT FLOOR BASED ON CURRENT STATE___
*/
//checks if buttons are pressed. If they are, the order_matrix is updated.
int check_order(); 

//Checks all orders above, including current floor. 
//Prioritizes floor orders "up" and orders from the elevator panel.
//set next floor based on nearest order and returns 1
//returns 0 if no order above
int check_order_above(); 

//checks all order below, including current floor.
//prioritizes floor orders "down" and orders from the elevator panel.
//set next floor based on the most valid order below and returns 1
//returns 0 if no order below
int check_order_below();

//delete orders for a given floor
void delete_order(int floor);

//calls check_order_below() or check_order_below() based on current direction, last direction and current floor
void set_priority();

int sum_of_orders();
/*
____HELPFUNCTIONS FOR SYSTEMTESTING____
*/
void print_current();

void print_next();

void print_matrix();

/*
____TIMEDELAY FUNCTION____
*/
void time_delay(int number_of_seconds); //Hold-up function for the elevator door lamp. 
                                        //Stalls the program for number_of_seconds while checking if the stop button is pressed