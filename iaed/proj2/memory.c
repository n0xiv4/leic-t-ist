/* iaed-23 - ist1106505 - memory */

/*
 * File: memory.c
 * Author: Rodrigo Salgueiro - ist1106505
 * Description: Memory management of the public transport
 * management system, written in C.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "memory.h"

/* FUNCTIONS */

/* PUBLIC SYSTEM MEMORY MANAGEMENT */

/* initializes the system and the memory needed for it */
public_sys *init_sys() {
	public_sys *transport = (public_sys *) safe_malloc
	(sizeof(public_sys)); 
	transport->first_stop = NULL;
	transport->last_stop = NULL;
	transport->stop_counter = 0;
	transport->first_route = NULL;
	transport->last_route = NULL;
	transport->route_counter = 0;
	return transport;
}

/* destroys the entire system */
void destroy_sys(public_sys *transport) {
	destroy_stops(transport);
	destroy_routes(transport);
	free(transport);	
}

/* ROUTES MEMORY MANAGEMENT */

/* allocates memory for a new route and returns it */
route *route_alloc(char *name) {
	/* allocating space for the route, for its name and
	 * for the array of pointers */
	route *new_route = (route *) safe_malloc(sizeof(route));
	new_route->name = (char *) safe_malloc
	(strlen(name) * sizeof(char) + 1);
	strcpy(new_route->name, name);
	new_route->first_link = NULL;
	new_route->last_link = NULL;
	/* assigning the values */
	new_route->stop_counter = 0;
	new_route->cost = 0;
	new_route->duration = 0;
	new_route->previous = NULL;
	new_route->next = NULL;
	return new_route;
}

/* (re)allocates memory for the array of pointers, with n being the
 * number of new elements memory shall be allocated for */
route **extra_route_alloc(stop* current) {
	unsigned int num = current->route_counter + 1;
	if (current->routes == NULL) {
		current->routes = (route **) safe_malloc(num * sizeof(route *));
	}
	else {
		current->routes = (route **) safe_realloc(current->routes, num * sizeof(route *));
	}
	return current->routes;
}

/* destroys a route and frees its memory */
void destroy_route(public_sys *sys, route *del) {
	reconnect_route_pointers(sys, del);
	rm_route_from_stops(sys, del);
	destroy_links(del);
	sys->route_counter--;
	/* free the memory */
	free(del->name);
	del->first_link = NULL;
	del->last_link = NULL;
	free(del);
}

/* fixes the previous and next pointers after a deleted route,
 * connecting the remaining ones */
void reconnect_route_pointers(public_sys *sys, route *del) {
	if (del->previous != NULL) {
		del->previous->next = del->next;
	}
	if (del->next != NULL) {
		del->next->previous = del->previous;
	}
	if (del == sys->first_route && del == sys->last_route) {
		sys->first_route = NULL;
		sys->last_route = NULL;
	}
	if (del == sys->first_route) {
		sys->first_route = del->next;
	}
	if (del == sys->last_route) {
		sys->last_route = del->previous;
	}
}

/* removes the del route from all the stops in the system */
void rm_route_from_stops(public_sys *sys, route *del) {
	stop *curr = sys->first_stop;
	unsigned int i, j;
	while (curr) {
		j = curr->route_counter;
		for (i = 0; i < curr->route_counter; i++) {
			if (curr->routes[i] == del) {
				for (j = i; j < curr->route_counter - 1; j++) {
					curr->routes[j] = curr->routes[j+1];
				}
				curr->route_counter--;
				break;
			}
		}
		curr->routes = safe_realloc(curr->routes, 
		j * sizeof(route *) + 1);
		curr = curr->next;
	}
}

/* destroys the linked list of links inside a route */
void destroy_links(route *del) {
	link *next_l = del->first_link;
	link *tmp_l;
	while (next_l != NULL) {
		tmp_l = next_l;
		next_l = next_l->next;
		free(tmp_l);
	}
}

/* destroys all the routes in the system, freeing the memory
 * used by them */
void destroy_routes(public_sys *sys) {
	route *next_r = sys->first_route;
	route *tmp_r;
	link *next_l, *tmp_l;
	
	while (next_r != NULL) {
		tmp_r = next_r;
		next_r = next_r->next;
		next_l = tmp_r->first_link;
		while (next_l != NULL) {
			tmp_l = next_l;
			next_l = next_l->next;
			free(tmp_l);
		}
		free(tmp_r->name);
		free(tmp_r);
	}
	sys->first_route = NULL;
	sys->last_route = NULL;
	sys->route_counter = 0;
}

/* STOPS MEMORY MANAGEMENT */

/* allocates memory for a new stop and returns it */
stop *stop_alloc(char *name, char *latitude, char *longitude) {
	/* allocating space for the stop and its name */
	stop *new_stop = (stop *) safe_malloc(sizeof(stop));
	new_stop->name = (char *) safe_malloc(strlen(name)*sizeof(char)+1);
	strcpy(new_stop->name, name);
	new_stop->routes = NULL;
	/* assigning the values */
	new_stop->route_counter = 0;
	new_stop->latitude = atof(latitude);
	new_stop->longitude = atof(longitude);
	new_stop->previous = NULL;
	new_stop->next = NULL;
	return new_stop;
}

/* destroys a route and frees its memory */
void destroy_stop(public_sys *sys, stop *del) {
	/* connect the previous and next elements */
	if (del->previous != NULL) {
		del->previous->next = del->next;
	}
	else {
		sys->first_stop = del->next;
	}
	if (del->next != NULL) {
		del->next->previous = del->previous;
	}
	rm_stop_from_connections(del);
	free(del->name);
	free(del->routes);
	free(del);
}

/* destroys all the stops in the system, freeing the memory
 * used by them */
void destroy_stops(public_sys *sys) {
	struct stop_node *next = sys->first_stop;
	struct stop_node *tmp;
	while (next != NULL) {
		tmp = next;
		next = next->next;	
		free(tmp->name);
		free(tmp->routes);
		free(tmp);	
   }
	sys->first_stop = NULL;
	sys->last_stop = NULL;
	sys->stop_counter = 0;
}

/* LINKS MEMORY MANAGEMENT */

/* allocates memory for a new link */
link *link_alloc(stop *first_stop, stop *last_stop, double cost, 
double duration) {
	link *new_link = (link *) safe_malloc(sizeof(link));
	new_link->first_stop = first_stop;
	new_link->last_stop = last_stop;
	new_link->cost = cost;
	new_link->duration = duration;
	new_link->previous = NULL;
	new_link->next = NULL;
	return new_link;
}

/* removes DEL from the linked list of links */
void rm_stop_from_connections(stop *del) {
	unsigned int i;
	route *curr_r;
	link *curr_l;
	for (i = 0; i < del->route_counter; i++) {
		curr_r = del->routes[i];
		curr_l = curr_r->first_link;
		while (curr_l != NULL) {
			if (curr_r->first_link == curr_l &&
			curr_l->first_stop == del) {
				curr_l = left_connection_destroyer(curr_r, curr_l);
			}
			else if (curr_r->last_link == curr_l &&
			curr_l->last_stop == del) {
				curr_l = right_connection_destroyer(curr_r, curr_l);
			}
			else if (curr_l->last_stop == del) {
				curr_l = middle_connection_destroyer(del, curr_r, curr_l);
			}
			else {
				curr_l = curr_l->next;
			}
		}
	}
}

/* destroys the links in the start of the route */
link *left_connection_destroyer(route *curr_r, 
link *curr_l) {
	link *tmp = NULL;
	if (curr_r->first_link->next != NULL) {
		curr_r->first_link = curr_l->next;
	}
	else {
		/* if there isn't any connection after the current
		 * one, we need to remove another stop from the 
		 * counter */
		curr_r->stop_counter--;
		curr_r->first_link = NULL;
	} 
	if (curr_r->last_link == curr_l) {
		curr_r->last_link = NULL;
	}
	curr_r->cost -= curr_l->cost;
	curr_r->duration -= curr_l->duration;
	curr_r->stop_counter--;
	tmp = curr_l;
	curr_l = curr_l->next;
	free(tmp);
	return curr_l;
}

/* destroys the links in the end of the route */
link *right_connection_destroyer(route *curr_r, 
link *curr_l) {
	if (curr_r->last_link->previous != NULL) {
		curr_r->last_link = curr_l->previous;
		curr_l->previous->next = NULL;
	}
	else {
		curr_r->stop_counter--;
		curr_r->last_link = NULL;
	}
	if (curr_r->first_link == curr_l) {
		curr_r->first_link = NULL;
	}
	curr_r->cost -= curr_l->cost;
	curr_r->duration -= curr_l->duration;
	curr_r->stop_counter--;
	free(curr_l);
	return NULL;
}

/* destroys the links in the middle that include DEL */
link *middle_connection_destroyer(stop *del, route *curr_r,
link *curr_l) {
	/* allocating empty link */
	link *new_link = link_alloc(NULL, NULL, 0, 0);

	curr_l = catch_opening_link(curr_l, new_link, curr_r);
	curr_l = catch_double_links(curr_l, new_link, curr_r, del);

	/* if no link is found after catching doubles, we should
	 * just stop looking for more */
	if (curr_l == NULL) {
		return NULL;
	}
	else {
		/* return link next to the last deleted one */
		return fixing_closer_link(curr_l, new_link, curr_r);
	}
}

/* catches the opening link - where the last stop of the link
 * matches DEL */
link *catch_opening_link(link *curr_l, link *new_link, route *curr_r) {
	link *tmp = NULL;
	new_link->previous = curr_l->previous;
	new_link->first_stop = curr_l->first_stop;
	new_link->cost += curr_l->cost;
	new_link->duration += curr_l->duration;

	/* removing reference to the current link, which will be
	 * deleted */
	if (curr_l == curr_r->first_link) {
		curr_r->first_link = new_link;
	}
	else {
		curr_l->previous->next = new_link;
	}
	tmp = curr_l;
	curr_l = curr_l->next;
	free(tmp);
	return curr_l;
}

/* catches double links - for instance, A-A - between other links,
 * where the inside stops match the one that needs to be deleted */
link *catch_double_links(link *curr_l, link *new_link, route *curr_r,
stop *del) {
	link *tmp = NULL;
	while (curr_l->first_stop == del && curr_l->last_stop == del) {
		new_link->cost += curr_l->cost;
		new_link->duration += curr_l->duration;
		tmp = curr_l;
		curr_l = curr_l->next;
		free(tmp);
		curr_r->stop_counter--;
		/* case where the route ends in a A-A link */
		if (curr_l == NULL) {
			if (new_link->previous != NULL) {
				new_link->previous->next = NULL;
				curr_r->last_link = new_link->previous;
			}
			else {
				curr_r->first_link = NULL;
				curr_r->last_link = NULL;
			}
			curr_r->cost -= new_link->cost;
			curr_r->duration -= new_link->duration;
			curr_r->stop_counter--;
			free(new_link);
			return NULL;
		}
	}
	return curr_l;
}

/* closes the fixed link - this happens when a link has the DEL
 * stop as first stop and a different stop as last stop */
link *fixing_closer_link(link *curr_l, link *new_link, route *curr_r) {
	link *tmp = NULL;
	new_link->last_stop = curr_l->last_stop;
	new_link->cost += curr_l->cost;
	new_link->duration += curr_l->duration;
	curr_r->stop_counter--;

	/* removing reference to the current link, which will be
	 * deleted */
	if (curr_l == curr_r->last_link) {
		curr_r->last_link = new_link;
		free(curr_l);
		return NULL;
	}
	else {
		new_link->next = curr_l->next;
		curr_l->next->previous = new_link;
		tmp = curr_l;
		curr_l = new_link->next;
		free(tmp);
		return curr_l;
	}
}

/* SAFE MEMORY ALLOCATION */

/* m_allocs memory sized size. if there is no memory, 
 * ERR_NO_MEMORY is printed */
void *safe_malloc(unsigned int size) {
	void *mem = malloc(size);
	if (mem != NULL) {
		return mem;
	}
	printf(ERR_NO_MEMORY);
	exit(EXIT_CODE_NO_MEMORY);
	return NULL;
}

/* reallocs memory sized size. if there is no memory,
 * ERR_NO_MEMORY is printed */
void *safe_realloc(void* ptr, unsigned int size) {
	void *mem = realloc(ptr, size);
	if (mem != NULL) {
		return mem;
	}
	printf(ERR_NO_MEMORY);
	exit(EXIT_CODE_NO_MEMORY);
	return NULL;
}

