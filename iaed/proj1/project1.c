/* iaed-23 - ist1106505 - project1 */

/*
 * File: project1.c
 * Author: Rodrigo Salgueiro - ist1106505
 * Description: Public transport management system, written in C.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "project1.h"

/* GLOBAL VARIABLES */
route routes[MAX_ROUTES];
int route_count = STARTING_ROUTE_COUNT;
stop stops[MAX_STOPS];
int stop_count = STARTING_STOP_COUNT;

/* FUNCTIONS */

/* main function */
int main() {
	while(command_receiver()) {
	}
	return 0;
}

/* receives commands from the user */
int command_receiver() {
	char key_input = getchar();
	switch(key_input) {
		case 'q':
			/* ends the program */
			return 0;

		case 'c':
			route_func();
			return 1;

		case 'p':
			stop_func();
			return 1;

		case 'l':
			connection_func();
			return 1;

		case 'i':
			intersection_func();
			return 1;

		default:
			/* unknown input */
			return 1;
	}
}

/* clears the user input from \n and spaces */
void input_clearer(char *user_input) {
	fgets(user_input, BUFSIZ, stdin);
	/* removing the SPACE from the start of the input */
	memmove(user_input, user_input+1, strlen(user_input));
	/* removing \n from the end of the input */
	user_input[strcspn(user_input, NL_STR)] = STR_CLOSER_CHAR;
}

/* reads a name from the user input. else, returns NO_ARGS */
int name_reader(char *user_input, char *name) {
	char *name_tmp; /* temporary var to store the token for the name */
	if (user_input[FIRST_CHAR] == SPACE_CHAR) {
		memmove(user_input, user_input+1, strlen(user_input));
	}
	if (user_input[FIRST_CHAR] == QUOTE_CHAR) {
		name_tmp = strtok(user_input, QUOTE_STR);
		strcpy(name, name_tmp);
	}
	else if (strlen(user_input) != 0){
		name_tmp = strtok(user_input, SPACE_STR);
		strcpy(name, name_tmp);
	}
	else {
		return NO_ARGS;
	}
	return UNDEFINED;
}

/* does the same as name_reader, but returns the remaining input */
char *destructive_name_reader(char *user_input, char *name) {
	name_reader(user_input, name);
	return strtok(NULL, STR_CLOSER);
}

/* ROUTES */

/* handles the route (r) command */
void route_func() {
	char route_name[MAX_ROUTE_NAME_LENGTH];
	int route_index; 
	/* mode of the route function */
	int FUNCTION_MODE = route_processing(route_name);

	/* if there are no arguments, the program will print all the routes */
	if (FUNCTION_MODE == NO_ARGS) {
		route_lister();
	}
	else {
		/* in case the route already exists, the program will 
		 * print its stops */
		if ((route_index = existing_route_checker(route_name)) != NONE) {
			route_stop_lister(FUNCTION_MODE, route_index);
		}
		/* else, it will create a new route with the specified name */
		else {
			route_creator(route_name);
		}
	}
}

/* lists all the routes */
void route_lister() {
	int i;
	int last_stop; /* index of the last stop of the scoped route */
	for (i = 0; i<route_count; i++) {
		if (routes[i].stop_counter != 0) {
			last_stop = routes[i].stop_counter - 1;
			printf("%s %s %s %d %.2f %.2f\n", routes[i].name,
			index_to_stopname(routes[i].stops[FIRST_STOP]),
			index_to_stopname(routes[i].stops[last_stop]), 
			routes[i].stop_counter, routes[i].cost, 
			routes[i].duration);
		}
		else {
			printf("%s %d %.2f %.2f\n", routes[i].name, 
		  	routes[i].stop_counter, routes[i].cost, routes[i].duration);
		}
	}
}

/* lists the stops of the route */
void route_stop_lister(int FUNCTION_MODE, int index) {
	int i;
	int last_stop; /* index of the last stop of the scoped route */
	if (routes[index].stop_counter > 0) {
		if (FUNCTION_MODE == NORMAL_MODE) {
			last_stop = routes[index].stop_counter - 1;
			for(i = 0; i<last_stop; i++) {
				printf("%s, ", index_to_stopname(routes[index].
				stops[i]));
			}
			printf("%s\n", index_to_stopname(routes[index].
			stops[last_stop]));
		}
		if (FUNCTION_MODE == INVERSE_MODE) {
			last_stop = routes[index].stop_counter - 1;
			for (i = last_stop; i>0; i--) {
				printf("%s, ", index_to_stopname(routes[index].
				stops[i]));
			}
			printf("%s\n", index_to_stopname(routes[index].
		  	stops[FIRST_STOP]));
		}
	}
}

/* creates a route with the specified name */
void route_creator(char* name) {
	strcpy(routes[route_count].name, name);
	routes[route_count].stop_counter = 0;
	route_count++;
}

/* processes the user input, returning which argument mode
 * the user input fits in */
int route_processing(char *route_name) {
	char user_input[BUFSIZ];
	char *inverschk; /* temporary var to store user "inverse" input */
	int FUNCTION_MODE;

	input_clearer(user_input);
	FUNCTION_MODE = name_reader(user_input, route_name);

	if (FUNCTION_MODE != NO_ARGS) {
		inverschk = strtok(NULL, SPACE_STR);
		/* if the string has the same first 3 characters as INVERSE */
		if (inverschk != NULL && inverse_checker(inverschk) == 0) {
			FUNCTION_MODE = INVERSE_MODE;
		}
		else if (inverschk != NULL) {
			printf("incorrect sort option.\n");
			FUNCTION_MODE = UNDEFINED; 
		}
		else {
			FUNCTION_MODE = NORMAL_MODE;
		}
	}
	return FUNCTION_MODE;
}

/* checks if a user input string - inverschk - matches with at least 3 words
 * of inverso */
int inverse_checker(char *inverschk) {
	char *inverse[INVERSE_SIZE] = {"inv", "inve", "inver", 
	"invers", "inverso"};
	int i;
	for (i = 0; i < INVERSE_SIZE - 1; i++) {
		if (strcmp(inverse[i], inverschk) == 0) {
			return 0;
		}
	}
	return 1;
}

/* returns the index of the route with the same name */
int existing_route_checker(char *name_input) {
	int i;
	for (i = STARTING_ROUTE_COUNT; i<route_count; i++) {
		if (strcmp(routes[i].name, name_input) == 0) {
			return i;
		}
	}
	return NONE;
}

/* receives an index and returns the name of the respective stop */
char *index_to_stopname(int index) {
	return stops[index].name;
}

/* returns the name of the route's first stop */
char *route_first_stop(int route_index) {
	int first_stop_index = routes[route_index].stops[FIRST_STOP];
	return stops[first_stop_index].name;
}

/* returns the name of the route's last stop */
char *route_last_stop(int route_index) {
	int last_stop_index = routes[route_index].stops[routes[route_index].
	stop_counter];
	return stops[last_stop_index].name;
}

/* STOPS */

/* handles the stop (s) command */
void stop_func() {
	char stop_name[MAX_STOP_NAME_LENGTH], latitude[BUFSIZ], 
	longitude[BUFSIZ];
	int i;
	/* status var that turns TRUE when a match for the input is found */
	int MATCH_FOUND = 0; 
	/* mode of the stop function */
	int FUNCTION_MODE = stop_processing(stop_name, latitude, 
	longitude);

	switch(FUNCTION_MODE) {
		case NO_ARGS:
			for (i = 0; i<stop_count; i++) {
				printf("%s: %16.12f %16.12f %d\n", stops[i].name, 
				stops[i].latitude, stops[i].longitude, 
				stops[i].route_counter);
			}
			break;
		case STOP_NAME_ONLY:
			for(i = 0; i<stop_count; i++) {
				if (strcmp(stops[i].name, stop_name) == 0) {
					printf("%16.12f %16.12f\n", stops[i].latitude, 
					stops[i].longitude);
					MATCH_FOUND = 1;
				}
			}
			if (MATCH_FOUND == 0) {
				printf("%s: no such stop.\n", stop_name);
			}
			break;
		case CREATE_MODE:
			stop_creator(stop_name, latitude, longitude);
			break;
	}
}

/* creates a stop */
void stop_creator(char *name, char *latitude, char *longitude) {
	if (existing_stop_checker(name) == NONE) {
		strcpy(stops[stop_count].name, name);
		stops[stop_count].latitude = atof(latitude);
		stops[stop_count].longitude = atof(longitude);
		stops[stop_count].route_counter = 0;
		stop_count++;
	}
	else {
		printf("%s: stop already exists.\n", name);
	}
}

/* processes the user input, returning which argument mode the 
 * user input fits in */
int stop_processing(char *stop_name, char *latitude, char *longitude) {
	char user_input[BUFSIZ];
	int FUNCTION_MODE = UNDEFINED;

	input_clearer(user_input);
	FUNCTION_MODE = name_reader(user_input, stop_name);

	if (FUNCTION_MODE != NO_ARGS) {
		FUNCTION_MODE = coords_reader(latitude, longitude);	
	}
	return FUNCTION_MODE;
}

/* returns the index of the stop with the same name */
int existing_stop_checker(char *name_input) {
	int i;
	for (i = STARTING_STOP_COUNT; i<stop_count; i++) {
		if (strcmp(stops[i].name, name_input) == 0) {
			return i;
		}
	}
	return -1;
}

/* reads the coordinates of the stop. else, returns STOP_NAME_ONLY */
int coords_reader(char *latitude, char *longitude) {
	char *lat_tmp, *long_tmp; /* temp vars to store the tokens */
	lat_tmp = strtok(NULL, SPACE_STR);
	long_tmp = strtok(NULL, SPACE_STR);
	if (lat_tmp != NULL && long_tmp != NULL) {
		strcpy(latitude, lat_tmp);
		strcpy(longitude, long_tmp);
		return CREATE_MODE;
	}
	else {
		return STOP_NAME_ONLY;
	}
}

/* CONNECTIONS */

void connection_func() {
	char route_name[BUFSIZ], first_stop[BUFSIZ], last_stop[BUFSIZ], 
	cost[BUFSIZ], duration[BUFSIZ];
	connection_processing(route_name, first_stop, last_stop, cost,
	duration);
	if(connection_error_chk(route_name, first_stop, last_stop, cost, 
	duration) == 0) {
		connection_creator(route_name, first_stop, last_stop, cost,
		duration);
	}
}

/* processes the user input of the connection function */
void connection_processing(char *route_name, char *first_stop,
char *last_stop, char *cost, char *duration) {
	char user_input[BUFSIZ];
	char *input; /* temporary var to store remaining user inputs */
	input_clearer(user_input);

	input = destructive_name_reader(user_input, route_name);
	input = destructive_name_reader(input, first_stop);
	input = destructive_name_reader(input, last_stop);
	input = destructive_name_reader(input, cost);
	input = destructive_name_reader(input, duration);
}

/* checks if there are any errors in the function */
int connection_error_chk(char *route_name, char *first_stop, 
char *last_stop,char *cost, char *duration) {
	if (existing_route_checker(route_name) == NONE) {
		printf("%s: no such line.\n", route_name);
		return 1;
	}
	else if (existing_stop_checker(first_stop) == NONE) {
		printf("%s: no such stop.\n", first_stop);
		return 1;
	}
	else if (existing_stop_checker(last_stop) == NONE) {
		printf("%s: no such stop.\n", last_stop);
		return 1;
	}
	else if (atof(cost) < 0 || atof(duration) < 0) {
		printf("negative cost or duration.\n");
		return 1;
	}
	return 0;
}

/* creates a new connection */
void connection_creator(char *route_name, char *first_stop, 
char *last_stop, char *cost, char *duration) {
	double cost_d = atof(cost); /* double cost value */
	double duration_d = atof(duration); /* double duration value */
	int route_index = existing_route_checker(route_name);
	int first_stop_index = existing_stop_checker(first_stop);
	int last_stop_index = existing_stop_checker(last_stop);
	int FUNCTION_MODE = extreme_checker(first_stop_index, 
	last_stop_index, route_index);
	
	if (FUNCTION_MODE != ERR_INVALID_LINK) {
		if (FUNCTION_MODE == INITIAL_CREATE_MODE) {
			initial_connection_creator(route_index, first_stop_index,
			last_stop_index);
		}
		else if (FUNCTION_MODE == LEFT_CREATE_MODE) {
			left_connection_creator(route_index, first_stop_index);
		}
		else if (FUNCTION_MODE == NORMAL_CREATE_MODE || 
		FUNCTION_MODE == CIRCULAR_CREATE_MODE) {
			right_connection_creator(route_index, last_stop_index,
			FUNCTION_MODE);
		}
		routes[route_index].cost += cost_d;
		routes[route_index].duration += duration_d;
   }	
	else {
		printf("link cannot be associated with bus line.\n");
	}
}

/* creates the initial connection - linking the two stops 
 * with the route */
void initial_connection_creator(int route_index, int first_stop_index,
int last_stop_index) {
	/* adding the stops to the array of stops in the 
	 * route struct */
	routes[route_index].stops[FIRST_STOP] = first_stop_index;
	routes[route_index].stops[1] = last_stop_index;
	/* adding the routes to the array of routes in the 
	 * stops struct */
	strcpy(stops[first_stop_index].routes[stops
	[first_stop_index].route_counter], routes[route_index].name);
	strcpy(stops[last_stop_index].routes[stops
	[last_stop_index].route_counter], routes[route_index].name);
	/* updating counters */
	routes[route_index].stop_counter += 2;
	stops[first_stop_index].route_counter++;
	stops[last_stop_index].route_counter++;
}

void left_connection_creator(int route_index, int first_stop_index) {
	int i;
	/* the index of where to place the stop */
	int current_stop_index = routes[route_index].stop_counter;
	/* moving the array right to free up space for the
	 * first stop */
	for (i = current_stop_index; i > 0; i--) {
		routes[route_index].stops[i] = 
		routes[route_index].stops[i-1];
	}
	routes[route_index].stops[FIRST_STOP] = first_stop_index;
	/* adding the route to the array of routes 
	 * in the stops struct */
	strcpy(stops[first_stop_index].
	routes[stops[first_stop_index].
	route_counter], routes[route_index].name);
	/* updating counters */
	routes[route_index].stop_counter++;
	stops[first_stop_index].route_counter++;
}

void right_connection_creator(int route_index, int last_stop_index,
int FUNCTION_MODE) {
	/* the index of where to place the stop */
	int current_stop_index = routes[route_index].stop_counter;
	/* adding the last stop to the right side of 
	 * the array */
	routes[route_index].stops[current_stop_index] =
	last_stop_index;
	/* adding the route to the array of routes in the 
	 * stops struct */
	strcpy(stops[last_stop_index].routes[stops
	[last_stop_index].route_counter], routes
	[route_index].name);
	routes[route_index].stop_counter++;
	/* the stop only has a new route linked if the 
	 * route isn't circular */
	if (FUNCTION_MODE != CIRCULAR_CREATE_MODE) {
		stops[last_stop_index].route_counter++;
	}
}

/* checks how the extremes of the route compare with the user 
 * input, returning the function mode that fits what should be done */
int extreme_checker(int first_stop_index, int last_stop_index, 
int route_index) {
	if (routes[route_index].stop_counter == 0) {
		return INITIAL_CREATE_MODE;
	}
	if (last_stop_index == routes[route_index].stops[FIRST_STOP]) {
		if (first_stop_index == routes[route_index].stops
		[routes[route_index].stop_counter - 1]) {
			return CIRCULAR_CREATE_MODE;
		}
		else {
			return LEFT_CREATE_MODE;
		}
	}
	if (first_stop_index ==  
	routes[route_index].stops[routes[route_index].stop_counter - 1]) {
		return NORMAL_CREATE_MODE;
	}
	else {
		return ERR_INVALID_LINK;
	}
}

/* INTERSECTIONS */

/* sorts an array of strings alphabetically */
void bubble_sort(char array[][MAX_ROUTE_NAME_LENGTH], int n) {
	int i, j;
	char tmp[MAX_ROUTE_NAME_LENGTH];
	for (i = 0; i < n-1; i++) {
		for (j = 0; j < n-i-1; j++) {
			if (strcmp(array[j], array[j+1]) > 0) {
				strcpy(tmp, array[j]);
				strcpy(array[j], array[j+1]);
				strcpy(array[j+1], tmp);
			}
		}
	}
}

/* prints all the intersections */
void intersection_func() {
	int i, j;
	for (i = 0; i < stop_count; i++) {
		if (stops[i].route_counter > 1) {
			bubble_sort(stops[i].routes, stops[i].
			route_counter);
			printf("%s %d: ", stops[i].name, 
		  	stops[i].route_counter);
			for (j = 0; j < stops[i].route_counter-1; j++) {
				printf("%s ", stops[i].routes[j]);
			}
			printf("%s\n", stops[i].routes[stops[i].
		  	route_counter-1]);
		}
	}
}

