/*Collection of functions and data structures which implement a priority queue,
   using a binary heap as the underlying data structure.
   
   Created for the purpose of completing Assignment 2 for COMP20003 undertaken
   at the University of Melbourne, in Semester 2, 2014.
   
   
   The implemention loosely follows that of "The Algorithm Design Manual" 
   						(2nd Edition, S.Skiena,2008) 
   but with a number of adjustments, and a few extra functions.
   
   Author: James Adams, Student no: 572541.
   
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define DEBUG 0

/*Defined data structures */

/*struct for storing all the info regarding a place/city/node */
typedef struct {
	int placeID;
	char *city;
	char *continent;
	char *cultural;
	char *outdoors;
} place_t;
/*element to be used in the priority queue */
typedef struct {
	int ID;
	int cost;
} q_element;

/*struct for the priority queue itself! */
typedef struct {
	q_element *q;
	int n;
	int *hash; /*will be used to keep track of where elements are
			in the pq, to make sure the decrease_priority function
			can behave correctly */
} priority_queue;


/*function prototypes: */
int pq_parent(int n);
int pq_young_child(int n);
void pq_insert(priority_queue *q, q_element x);
void bubble_up(priority_queue *q, int p);
void pq_init(priority_queue *q);
q_element extract_min(priority_queue *q);
void bubble_down(priority_queue *q, int p);
void make_heap(priority_queue *q, q_element s[], int n);
void pq_swap(priority_queue *q,int p_1,int p_2);
void decrease_priority(priority_queue *q,int index,int new);


