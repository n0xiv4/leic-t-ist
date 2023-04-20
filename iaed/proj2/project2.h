/* iaed-23 - ist1106505 - project2 */

/*
 * File: project2.h
 * Author: Rodrigo Salgueiro - ist1106505
 * Description: Header file for the public transport management system,
 * written in C.
*/

#ifndef MAIN_H
#define MAIN_H
#include "memory.h"

#define MAX_CMD_SIZE 65535 +1

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

/* macros */
#define ERR_INCORR_SORT_OPTION "incorrect sort option.\n"
#define ERR_NO_SUCH_LINE "no such line"
#define ERR_NO_SUCH_STOP "no such stop"
#define ERR_NEG_COST_DURATION "negative cost or duration.\n"
#define ERR_LINK_INVALID_ASSOCIATION "link cannot be associated with bus line.\n"

/* prototypes */
int command_receiver(public_sys *transport);

void input_clearer(char *user_input);
int name_reader(char *user_input, char *name);
char *destructive_name_reader(char *user_input, char *name);
 
void route_func(public_sys *transport);
int route_processing(char *route_name);
void route_lister(public_sys *transport);
void route_stop_lister(int FUNCTION_MODE, route *input_route);
void route_creator(public_sys *transport, char *name);
route *existing_route_checker(public_sys *transport, char *input_name);
int inverse_checker(char *inverschk);
stop *get_first_stop(route *input_route);
stop *get_last_stop(route *input_route);

void stop_func(public_sys *transport);
int stop_processing(char *stop_name, char *latitude, char *longitude);
int coords_reader(char *latitude, char *longitude);
void stop_creator(public_sys *transport, char *name, char *latitude, char *longitude);
void stop_lister(public_sys *transport);
void stop_finder(public_sys *transport, char *input_name);
stop *existing_stop_checker(public_sys *transport, char *input_name);

void connection_func(public_sys *transport);
void connection_processing(char *route_name, char *first_stop,
char *last_stop, char *cost, char *duration);
int connection_error_chk(public_sys *transport, char *route_name,
char *first_stop, char *last_stop, char *cost, char *duration);
void connection_creator(public_sys* transport, char *route_name,
char *first_stop, char *last_stop, char *cost, char *duration);
int extreme_checker(stop *first_stop, 
stop *last_stop, route *input_route);
void initial_connection_creator(route *input_route,
stop *first_stop, stop *last_stop, double cost, double duration);
void left_connection_creator(route *input_route,
stop *first_stop, stop *last_stop, double cost, double duration);
void right_connection_creator(route *input_route,
stop *first_stop, stop *last_stop, double cost, double duration, 
int FUNCTION_MODE);
int route_already_added(stop *stop_to_check, route *route_to_check);

void intersection_func(public_sys *transport);
void bubble_sort(route **array, int n);

void route_del_func(public_sys *transport);
void stop_del_func(public_sys *transport);
void clear_sys_func(public_sys *old_sys);

#endif
