/* iaed-23 - ist1106505 - project1 */

/*
 * File: project1.h
 * Author: Rodrigo Salgueiro - ist1106505
 * Description: Header file for the public transport management system,
 * written in C.
*/

#ifndef MAIN_H
#define MAIN_H

/* general definitions */
#define MAX_ROUTES 200 +1
#define MAX_STOPS 10000 +1
#define MAX_CONNECTIONS 30000 +1
#define STARTING_ROUTE_COUNT 0
#define STARTING_STOP_COUNT 0

/* character definitions */
#define NL_STR "\n"
#define SPACE_STR " "
#define SPACE_CHAR ' '
#define QUOTE_STR "\""
#define QUOTE_CHAR '"'
#define STR_CLOSER "\0"
#define STR_CLOSER_CHAR '\0'

/* specific definitions */
#define FIRST_CHAR 0
#define INVERSE_SIZE 5 +1
#define NONE -1

/* function mode definitions */
#define UNDEFINED -1
#define NO_ARGS 1

/* stop definitions */
#define FIRST_STOP 0
#define STOP_NAME_ONLY 2
#define CREATE_MODE 3

/* route definitions */
#define NORMAL_MODE 2
#define INVERSE_MODE 3

/* connection definitions */
#define INITIAL_CREATE_MODE 1
#define NORMAL_CREATE_MODE 2
#define LEFT_CREATE_MODE 3
#define CIRCULAR_CREATE_MODE 4
#define ERR_INVALID_LINK 5

/* type definition */
#define MAX_ROUTE_NAME_LENGTH 20 +1
#define MAX_STOP_NAME_LENGTH 50 +1

typedef struct {
	char name[MAX_STOP_NAME_LENGTH];
	double latitude;
	double longitude;
  char routes[MAX_ROUTES][MAX_ROUTE_NAME_LENGTH];
	int route_counter;
} stop;

typedef struct {
	char name[MAX_ROUTE_NAME_LENGTH];
	int stops[MAX_STOPS];
	int stop_counter;
	double cost;
	double duration;
} route;

/* general functions */
int command_receiver();
void input_clearer(char *user_input);
int name_reader(char *user_input, char *name);
char *destructive_name_reader(char *user_input, char *name);

/* route functions */
void route_func();
void route_lister();
void route_stop_lister(int FUNCTION_MODE, int index);
void route_creator(char *name);
int route_processing(char *route_name);
int inverse_checker(char* inverschk);
int existing_route_checker(char *name_input);
char *index_to_stopname(int index);
char *route_first_stop(int route_index);
char *route_last_stop(int route_index);

/* stop functions */
void stop_func();
void stop_creator(char *name, char *latitude, char *longitude); 
int stop_processing(char *stop_name, char *latitude, char *longitude);
int existing_stop_checker(char *name_input); 
int coords_reader(char *latitude, char *longitude);

/* connection functions */
void connection_func();
void connection_processing(char *route_name, char *first_stop, 
char *last_stop,char *cost, char *duration);
int connection_error_chk(char *route_name, char *first_stop, 
char *last_stop, char *cost, char *duration);
void connection_creator(char *route_name, char *first_stop, 
char *last_stop, char *cost, char *duration);
void initial_connection_creator(int route_index, int first_stop_index,
int last_stop_index);
void left_connection_creator(int route_index, int first_stop_index);
void right_connection_creator(int route_index, int last_stop_index,
int FUNCTION_MODE); 
int extreme_checker(int first_stop_index, int last_stop_index, 
int route_index);

/* intersection functions */
void bubble_sort(char array[][MAX_ROUTE_NAME_LENGTH], int n);
void intersection_func();

#endif

