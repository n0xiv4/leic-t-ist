/* iaed-23 - ist1106505 - memory */

/*
 * File: memory.h
 * Author: Rodrigo Salgueiro - ist1106505
 * Description: Header file for the memory management of the 
 * public transport management system, written in C.
*/

#ifndef _MEMORY_H_
#define _MEMORY_H_

#define ERR_NO_MEMORY "No memory.\n"
#define EXIT_CODE_NO_MEMORY 1

struct stop_node {
	char *name;
	struct route_node **routes;
	unsigned int route_counter;
	double latitude, longitude;
	struct stop_node *previous, *next;
};

struct route_node {
	char *name;
	struct link_node *first_link, *last_link;
	unsigned int stop_counter;
	double cost, duration;
	struct route_node *previous, *next;
};

struct link_node {
	struct stop_node *first_stop, *last_stop;
	double cost, duration;
	struct link_node *previous, *next;
};

typedef struct {
	struct stop_node *first_stop, *last_stop;
	struct route_node *first_route, *last_route;
	unsigned int stop_counter, route_counter;
}public_sys;

typedef struct stop_node stop;
typedef struct route_node route;
typedef struct link_node link;

/* prototypes */
public_sys *init_sys();
void destroy_sys(public_sys *transport);

route *route_alloc(char *name);
route **extra_route_alloc(stop* current);
void destroy_route(public_sys *sys, route *del);
void reconnect_route_pointers(public_sys *sys, route *del);
void rm_route_from_stops(public_sys *sys, route *del);
void destroy_links(route *del);
void destroy_routes(public_sys *transport);

stop *stop_alloc(char *name, char *latitude, char *longitude);
void destroy_stop(public_sys *sys, stop *del);
void destroy_stops(public_sys *transport);

link *link_alloc(stop *first_stop, stop *last_stop, double cost,
double duration);
void rm_stop_from_connections(stop *del);
link *left_connection_destroyer(route *curr_r, 
link *curr_l);
link *right_connection_destroyer(route *curr_r, 
link *curr_l);
link *middle_connection_destroyer(stop *del, route *curr_r,
link *curr_l);
link *catch_opening_link(link *curr_l, link *new_link, route *curr_r);
link *catch_double_links(link *curr_l, link *new_link, route *curr_r,
stop *del);
link *fixing_closer_link(link *curr_l, link *new_link, route *curr_r);

void *safe_malloc(unsigned int size);
void *safe_realloc(void* ptr, unsigned int size);

#endif
