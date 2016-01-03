#include "heap.h"


/*function which returns the index of the parent of the argument index n */
int pq_parent(int n) {

	if (n==1) return(-1);
	else return((int) n/2);   /* implicitly take floor(n/2) via typecasting
						*/
}
/*function which returns the index of the left leaf of the argument 
	parent index n */
int pq_young_child(int n) {
	return(2*n);
}

/*function which swaps two items in the queue, whilst keeping there
  indices up to date in the hash table associated with the pq */
void pq_swap(priority_queue *q,int p_1,int p_2) {
	q_element temp=q->q[p_1];
	/*update hash table */
	q->hash[q->q[p_1].ID]=p_2;
	q->hash[q->q[p_2].ID]=p_1;
	q->q[p_1]=q->q[p_2];
	q->q[p_2]=temp;
}

/*inserts a new element into the 'bottom' of the priority queue, and bubbles
  it up to the right position */
void pq_insert(priority_queue *q,q_element x)
{
	/*increase the size of the pq by 1 */
        q->n = (q->n) + 1;
        q->q[ q->n ] = x;
        /*send the new item to the right position in the pq, in order
          to maintain the heap property. */
        bubble_up(q, q->n);
    
}
/*function which sends 'up' a pq element to where it belongs in order
  to maintain the heap property in the priority queue*/
void bubble_up(priority_queue *q, int p) {
	
	if (pq_parent(p) == -1) return; /* at root of heap, so no parent */
	
	/*checks whether the argument index,p is smaller than the parent,
	and thus if it needs to be bubbled up 
	(for this is a minimum priority queue implementation) */
	if (q->q[pq_parent(p)].cost > q->q[p].cost) {
		/*swap the parent and the child */
		pq_swap(q,p,pq_parent(p));
		/*the element in question may still be smaller than its
		parent. confirm this via a recursive call/another attempt
		to bubble the element up the priority queue towards the root */
		bubble_up(q,pq_parent(p));
	}
}
/*function to initiate a priority queue */
void pq_init(priority_queue *q) {
	
	q->n=0;
}
/*function to extract and return the root/minimum element from a priority queue
  Once this is done, it takes the largest child, sets it to be the root, 
  then bubbles it down. This maintains the heap property as well as the shape
  of the argument priority queue */
q_element extract_min(priority_queue *q) {
	q_element min;  /*storage for element to be returned*/
	/*initialise to prevent a warning flag popping up */
	min.cost=INT_MAX;
	
	if (q->n <= 0) printf("Warning: empty priority queue. \n");
	else {
		/*stores the root value */
		min=q->q[1];
		/*sets the root to be the value of the largest/last child */
		q->q[1] = q->q[q->n];
		/*decrease the size of the heap by 1 */
		/* or consider it as recording the decrease in size by 1.. */
		q->n = q->n -1;
		/*bubble down the new root for the reasons detailed in
		the function description*/
		bubble_down(q,1);
	}
	/*return the original root from the priority queue, the element
	originally intended to be extracted and returned */
	return min;
}
/*function to check whether an element is too ahead in a priority queue, and 
  needs to be sent downwards in order to maintain the heap property*/
void bubble_down(priority_queue *q, int p) {
	int c;    	 /*child index */
	int i;    	 /*counter */
	int min_index;   /* index of lightest child */
	
	c=pq_young_child(p);
	min_index=p;
	
	/*deals with the children */
	for(i=0;i<=1;i++){
		if((c+i) <= q->n) {
			if (q->q[min_index].cost > 
					q->q[c+i].cost) 
				min_index = c+i;
		}
	}
	
	if (min_index !=p) {
			
		pq_swap(q,p,min_index);
		bubble_down(q,min_index);
	}
}
/*function to decrease the priority of an element, index, in the priority queue
  to the value of new, and 'bubble it up' in order to maintain the heap property
  */
void decrease_priority(priority_queue *q,int index,int new) {
	q->q[index].cost=new; 
	bubble_up(q,index);
}

/* more efficient make_heap than individually inserting each element 
   into the heap via pq_insert(). Takes as argument an empty priority queue, an
   array of all the elements to be inserted, and the size to initiate the
   priority queue to. The function then creates a priority queue which 
   satisfies the heap property and shape property*/

void make_heap(priority_queue *q, q_element s[], int n) {
	
	int i;    /*counter */
	
	q->n = n;
	/*insert all the places into the array */
	for(i=0; i<n; i++) {
		q->q[i+1] = s[i];
		/*keep track of where each node is in the pq via
			the hash table */
		q->hash[s[i].ID]=i+1;
	}
	
	for (i=q->n/2; i>=1; i--) {
	
		bubble_down(q,i);
	}
	/*calling bubble_down() restores the heap order of the arbitrary root 
	  element sitting on top of two sub-heaps. */
}
