/* iaed-23 - ist1106505 - project2 */

/*
 * File: project2.c
 * Author: Rodrigo Salgueiro - ist1106505
 * Description: Public transport management system, written in C.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "project2.h"
#include "memory.h"

/* FUNCTIONS */

/* main function */
int main() {
	public_sys *transport = init_sys();
	while(command_receiver(transport)) {
	}
	destroy_sys(transport);
	return 0;
}

/* receives commands from the user */
int command_receiver(public_sys *transport) {
	char key_input = getchar();
	switch(key_input) {
		case 'q':
			/* ends the program */
			return 0;
		case 'c':
			route_func(transport);
			return 1;
		case 'p':
			stop_func(transport);
			return 1;
		case 'l':
			connection_func(transport);
			return 1;
		case 'i':
			intersection_func(transport);
			return 1;
		case 'r':
			route_del_func(transport);
			return 1;
		case 'e':
			stop_del_func(transport);
			return 1;
		case 'a':
			clear_sys_func(transport);
			return 1;
		default:
			/* unknown input */
			return 1;
	}
}

/* GENERAL FUNCTIONS */

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
void route_func(public_sys *transport) {
	char route_name[MAX_CMD_SIZE];
	route *match;
	/* mode of the route function */
	int FUNCTION_MODE = route_processing(route_name);
	/* if there are no arguments, the program will print all the routes */
	if (FUNCTION_MODE == NO_ARGS) {
		route_lister(transport);
	}
	else {
		match = existing_route_checker(transport, route_name);
		/* in case the route already exists, the program will
		 * print its stops */
		if (match != NULL) {
			route_stop_lister(FUNCTION_MODE, match);
		}
		/* else, it will create a new route with the specified name */
		else {
			route_creator(transport, route_name);
		}
	}
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
			printf(ERR_INCORR_SORT_OPTION);
			FUNCTION_MODE = UNDEFINED;
		}
		else {
			FUNCTION_MODE = NORMAL_MODE;
		}
	}
	return FUNCTION_MODE;
}

/* lists all the routes */
void route_lister(public_sys *transport) {
	route *current = transport->first_route;
	while (current != NULL) {
		if (current->stop_counter != 0) {
			printf("%s %s %s %d %.2f %.2f\n", current->name,
			get_first_stop(current)->name, get_last_stop(current)->name,
			current->stop_counter, current->cost, current->duration);
		}
		else {
			printf("%s %d %.2f %.2f\n", current->name,
		  	current->stop_counter, current->cost, current->duration);
		}
		current = current->next;
	}
}

/* lists the stops of the route */
void route_stop_lister(int FUNCTION_MODE, route *input_route) {
	link *current;
	/* index of the last stop of the scoped route */
	if (input_route->stop_counter > 0) {
		if (FUNCTION_MODE == NORMAL_MODE) {
			current = input_route->first_link;
			while (current != NULL) {
				printf("%s, ", current->first_stop->name);
				if (current->next == NULL) {
					printf("%s\n", current->last_stop->name);
				}
				current = current->next;
			}
		}
		if (FUNCTION_MODE == INVERSE_MODE) {
			current = input_route->last_link;
			while (current != NULL) {
				printf("%s, ", current->last_stop->name);
				if (current->previous == NULL) {
					printf("%s\n", current->first_stop->name);
				}
				current = current->previous;
			}
		}
	}
}

/* creates a route with the specified name */
void route_creator(public_sys *transport, char *name) {
	route *new_route = route_alloc(name);
	route *tmp;
	/* first case - setting up the list */
	if (transport->first_route == NULL) {
		transport->first_route = new_route;
	}
	/* the current last route "next" pointer will point to our new route,
	 * which will then take the last stop spot */
	else if (transport->last_route == NULL) {
		transport->first_route->next = new_route;
		transport->last_route = new_route;
		transport->last_route->previous = transport->first_route;
	}
	else {
		tmp = transport->last_route;
		transport->last_route->next = new_route;
		transport->last_route = new_route;
		transport->last_route->previous = tmp;
	}
	transport->route_counter++;
}

/* returns a pointer to the route which name matches with the input name.
 * if there's no matching route, returns NULL. */
route *existing_route_checker(public_sys *transport, char *input_name) {
	route *current = transport->first_route;
	while (current != NULL) {
		if (strcmp(current->name, input_name) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
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

/* returns a pointer to the first stop of the specified route */
stop *get_first_stop(route *input_route) {
	return input_route->first_link->first_stop;
}

/* returns a pointer to the last stop of the specified route */
stop *get_last_stop(route *input_route) {
	return input_route->last_link->last_stop;
}

/* STOPS */

/* handles the stop (s) command */
void stop_func(public_sys *transport) {
	char input_name[MAX_CMD_SIZE], latitude[MAX_CMD_SIZE],
	longitude[MAX_CMD_SIZE];
	/* status var that turns TRUE when a match for the input is found */
	/* mode of the stop function */
	int FUNCTION_MODE = stop_processing(input_name, latitude,
	longitude);

	switch(FUNCTION_MODE) {
		case NO_ARGS:
			stop_lister(transport);
			break;
		case STOP_NAME_ONLY:
			stop_finder(transport, input_name);
			break;
		case CREATE_MODE:
			stop_creator(transport, input_name, latitude, longitude);
			break;
	}
}

/* processes the user input, returning which argument mode the
 * user input fits in */
int stop_processing(char *stop_name, char *latitude, char *longitude) {
	char user_input[MAX_CMD_SIZE];
	int FUNCTION_MODE = UNDEFINED;
	input_clearer(user_input);
	FUNCTION_MODE = name_reader(user_input, stop_name);
	if (FUNCTION_MODE != NO_ARGS) {
		FUNCTION_MODE = coords_reader(latitude, longitude);
	}
	return FUNCTION_MODE;
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

/* creates a stop, allocating memory for it and placing it in the
 * linked list */
void stop_creator(public_sys *transport, char *name, char *latitude,
char *longitude) {
	stop *new_stop = stop_alloc(name, latitude, longitude);
	/* first case - setting up the list */
	if (transport->first_stop == NULL) {
		transport->first_stop = new_stop;
	}
	/* the current last stop "next" pointer will point to our new stop,
	 * which will then take the last stop spot */
	else if (transport->last_stop == NULL) {
		transport->first_stop->next = new_stop;
		transport->last_stop = new_stop;
		transport->last_stop->previous = transport->first_stop;
	}
	else {
		new_stop->previous = transport->last_stop;
		transport->last_stop->next = new_stop;
		transport->last_stop = new_stop;
	}
	transport->stop_counter++;
}

/* lists all the stops */
void stop_lister(public_sys *transport) {
	stop *current = transport->first_stop;
	while (current != NULL) {
		printf("%s: %16.12f %16.12f %d\n", current->name, current->latitude,
		current->longitude, current->route_counter);
		current = current->next;
	}
}

/* prints a stop matching with the input name */
void stop_finder(public_sys *transport, char *input_name) {
	stop *found = existing_stop_checker(transport, input_name);
	if (found != NULL) {
		printf("%16.12f %16.12f\n", found->latitude, found->longitude);
	}
	else {
		printf("%s: %s.\n", input_name, ERR_NO_SUCH_STOP);
	}
}

/* returns a pointer to the stop which name matches with the input name.
 * if there's no stop, returns NULL. */
stop *existing_stop_checker(public_sys *transport, char *input_name) {
	stop *current = transport->first_stop;
	while (current != NULL) {
		if (strcmp(current->name, input_name) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

/* CONNECTIONS */

/* handles the connections (l) command */
void connection_func(public_sys *transport) {
	char route_name[MAX_CMD_SIZE], first_stop[MAX_CMD_SIZE],
	last_stop[MAX_CMD_SIZE], cost[MAX_CMD_SIZE],
	duration[MAX_CMD_SIZE];
	connection_processing(route_name, first_stop, last_stop, cost,
	duration);
	if(connection_error_chk(transport, route_name, first_stop,
	last_stop, cost, duration) == 0) {
		connection_creator(transport, route_name, first_stop,
		last_stop, cost, duration);
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
int connection_error_chk(public_sys *transport, char *route_name,
char *first_stop, char *last_stop, char *cost, char *duration) {
	if (existing_route_checker(transport, route_name) == NULL) {
		printf("%s: %s.\n", route_name, ERR_NO_SUCH_LINE);
		return 1;
	}
	else if (existing_stop_checker(transport, first_stop) == NULL) {
		printf("%s: %s.\n", first_stop, ERR_NO_SUCH_STOP);
		return 1;
	}
	else if (existing_stop_checker(transport, last_stop) == NULL) {
		printf("%s: %s.\n", last_stop, ERR_NO_SUCH_STOP);
		return 1;
	}
	else if (atof(cost) < 0 || atof(duration) < 0) {
		printf(ERR_NEG_COST_DURATION);
		return 1;
	}
	return 0;
}

/* creates a new connection */
void connection_creator(public_sys* transport, char *route_name,
char *first_stop, char *last_stop, char *cost, char *duration) {
	double cost_d = atof(cost); /* double cost value */
	double duration_d = atof(duration); /* double duration value */
	route *input_route = existing_route_checker(transport, route_name);
	stop *input_fstop = existing_stop_checker(transport, first_stop);
	stop *input_lstop = existing_stop_checker(transport, last_stop);
	int FUNCTION_MODE = extreme_checker(input_fstop,
	input_lstop, input_route);

	if (FUNCTION_MODE != ERR_INVALID_LINK) {
		if (FUNCTION_MODE == INITIAL_CREATE_MODE) {
			initial_connection_creator(input_route,
			input_fstop, input_lstop, cost_d, duration_d);
		}
		else if (FUNCTION_MODE == LEFT_CREATE_MODE) {
			left_connection_creator(input_route,
			input_fstop, input_lstop, cost_d, duration_d);
		}
		else if (FUNCTION_MODE == NORMAL_CREATE_MODE ||
		FUNCTION_MODE == CIRCULAR_CREATE_MODE) {
			right_connection_creator(input_route,
			input_fstop, input_lstop, cost_d, duration_d,
			FUNCTION_MODE);
		}
		input_route->cost += cost_d;
		input_route->duration += duration_d;
   }
	else {
		printf(ERR_LINK_INVALID_ASSOCIATION);
	}
}


/* checks how the extremes of the route compare with the user
 * input, returning the function mode that fits what should be done */
int extreme_checker(stop *first_stop, stop *last_stop,
route *input_route) {
	if (input_route->stop_counter == 0) {
		return INITIAL_CREATE_MODE;
	}
	if (last_stop == input_route->first_link->first_stop) {
		if (first_stop == input_route->last_link->last_stop) {
			return CIRCULAR_CREATE_MODE;
		}
		else {
			return LEFT_CREATE_MODE;
		}
	}
	if (first_stop == input_route->last_link->last_stop) {
		return NORMAL_CREATE_MODE;
	}
	else {
		return ERR_INVALID_LINK;
	}
}

/* creates the initial connection - linking the two stops
 * with the route */
void initial_connection_creator(route *input_route, stop *first_stop,
stop *last_stop, double cost, double duration) {
	/* allocating space for the link and creating it */
	link *new_link = link_alloc(first_stop, last_stop, cost, duration);
	input_route->first_link = new_link;
	input_route->last_link = new_link;
	input_route->stop_counter += 2;
	/* updating counters and adding routes to the array */
	if (first_stop != last_stop) {
		last_stop->routes = extra_route_alloc(last_stop);
		last_stop->routes[last_stop->route_counter] = input_route;
		last_stop->route_counter++;
	}
	first_stop->routes = extra_route_alloc(first_stop);
	first_stop->routes[first_stop->route_counter] = input_route;
	first_stop->route_counter++;
}

/* creates a connection to the left of the route */
void left_connection_creator(route *input_route, stop *first_stop,
stop *last_stop, double cost, double duration) {
	link *new_link = link_alloc(first_stop, last_stop, cost, duration);

	input_route->first_link->previous = new_link;
	new_link->next = input_route->first_link;
	input_route->first_link = new_link;

	/* adding the route to the array of routes
	 * in the stops struct */
	if (route_already_added(first_stop, input_route) == 1) {
		first_stop->routes = extra_route_alloc(first_stop);
		first_stop->routes[first_stop->route_counter] = input_route;
		first_stop->route_counter++;
	}
	/* updating counters */
	input_route->stop_counter++;
}

/* creates a connection to the right of the route */
void right_connection_creator(route *input_route, stop *first_stop,
stop *last_stop, double cost, double duration, int FUNCTION_MODE) {
	/* the index of where to place the stop */
	link *new_link = link_alloc(first_stop, last_stop, cost, duration);

	input_route->last_link->next = new_link;
	new_link->previous = input_route->last_link;
	input_route->last_link = new_link;
	/* adding the route to the array of routes in the
	 * stops struct */
	if (route_already_added(last_stop, input_route) == 1) {
		last_stop->routes = extra_route_alloc(last_stop);
		last_stop->routes[last_stop->route_counter] = input_route;
		if (FUNCTION_MODE != CIRCULAR_CREATE_MODE) {
			last_stop->route_counter++;
		}
	}
	input_route->stop_counter++;
	/* the stop only has a new route linked if the
	 * route isn't circular */
}

/* checks if a route has already been listed in the stop */
int route_already_added(stop *stop_to_check, route *route_to_check) {
	unsigned int i;
	for (i = 0; i < stop_to_check->route_counter; i++) {
		if (stop_to_check->routes[i] == route_to_check) {
			return 0;
		}
	}
	return 1;
}

/* INTERSECTIONS */

/* prints all the intersections */
void intersection_func(public_sys *transport) {
	unsigned int i;
	stop *current = transport->first_stop;
	while (current != NULL) {
		if (current->route_counter > 1) {
			bubble_sort(current->routes, current->route_counter);
			printf("%s %d: ", current->name,
		  	current->route_counter);
			for (i = 0; i < current->route_counter - 1; i++) {
				printf("%s ", current->routes[i]->name);
			}
			printf("%s\n", current->routes[i]->name);
		}
		current = current->next;
	}
}

/* sorts an array of pointers to routes alphabetically */
void bubble_sort(route **array, int n) {
	int i, j;
	route *tmp;
	for (i = 0; i < n-1; i++) {
		for (j = 0; j < n-i-1; j++) {
			if (strcmp(array[j]->name, array[j+1]->name) > 0) {
				tmp = array[j];
				array[j] = array[j+1];
				array[j+1] = tmp;
			}
		}
	}
}

/* ROUTE DELETION */

/* handles the route deletion (r) command */
void route_del_func(public_sys *transport) {
	char user_input[MAX_CMD_SIZE];
	char route_name[MAX_CMD_SIZE];
	route *found;
	input_clearer(user_input);
	name_reader(user_input, route_name);
	found = existing_route_checker(transport, route_name);

	if (found == NULL) {
		printf("%s: %s.\n", route_name, ERR_NO_SUCH_LINE);
	}
	else {
		destroy_route(transport, found);
		transport->route_counter--;
	}
}

/* STOP DELETION */

/* handles the stop deletion (e) command */
void stop_del_func(public_sys *transport) {
	char user_input[MAX_CMD_SIZE];
	char stop_name[MAX_CMD_SIZE];
	stop *found;
	input_clearer(user_input);
	name_reader(user_input, stop_name);
	found = existing_stop_checker(transport, stop_name);

	if (found == NULL) {
		printf("%s: %s.\n", stop_name, ERR_NO_SUCH_STOP);
	}
	else {
		destroy_stop(transport, found);
	}
}

/* SYSTEM DELETION */

/* handles the delete system (a) command */
void clear_sys_func(public_sys *old_sys) {
	destroy_stops(old_sys);
	destroy_routes(old_sys);
}

