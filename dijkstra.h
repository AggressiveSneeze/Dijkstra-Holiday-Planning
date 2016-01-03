/* Collection of functions and data structures which implement Dijkstra's 
algorithm for the calculation of minimum path on a graph, using a priority 
queue stored in a heap as the underlying data structure.
   
   Created for the purpose of completing Assignment 2 for COMP20003 undertaken
   at the University of Melbourne, in Semester 2, 2014.
   
   Author: James Adams, Student no: 572541.
   
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "heap.h"
#define DEBUG 0
#define NO_PRED -1
#define NO_MATCH -1

/*Defined data structures */

/*struct for storing user input/travel preferences */
typedef struct {
	char *source;
	char *continent;
	char *cultural;
	char *outdoors;
		
} preference_t;

/*struct for returning the results of the algorithm */
typedef struct {
	int place_ID;
	int min_cost;
	int *preds;
	int num_preds;
} results_t;

/*function prototypes */
results_t dijkstra(place_t places[],int** costs,
			int num_places, preference_t preference); 
void initialize(int dist[],int pred[],int num_vertices, int source);
void run(priority_queue *pq,int dist[],int pred[],int** costs, 
	int source_index, preference_t preferences, place_t places[],
		int num_places);

int check_match(int id, place_t places[], preference_t preferences);

void update(int v,int current,int pred[], int dist[], priority_queue *pq,
		int **costs);
results_t results_processor(place_t places[],int dist[],preference_t preference,
				int pred[],int num_places);

