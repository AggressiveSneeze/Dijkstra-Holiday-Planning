/*Backbone for Assignment 2 for COMP20003 undertaken at the University of
  Melbourne, in semester 2, 2014.
  
  Author: James Adams, Student no: 572541.
  
*/


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "dijkstra.h"

#define MAXLINELEN 256
#define MAXFIELDLEN 80

#define ARGERROR 3
#define FILEERROR 4
#define MEMERROR 5

#define DEBUG 0

/*function prototypes */

void add_to_array(char place_index[],char city[],char continent[],
		char cultural[],char outdoors[], place_t places[], 
		int places_read);

preference_t* user_input(place_t places[], int num_places);

void results_printer(place_t places[],results_t results);

preference_t* pref_maker(char source[], char continent[], char cultural[],
			char outdoors[]);