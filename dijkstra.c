#include "dijkstra.h"


/*function that ties the whole algorithm together and runs it in the
	correct fashion, returning the results required. Goes a little
	beyond the general implementation of Dijkstra's algorithm and
	does the results checking to find the cheapest, viable location
	to travel, as well as backtracking to determine the predecessors.
	Finally, it returns what it found in a results_t struct defined above.*/
results_t dijkstra(place_t places[],int** costs,
			int num_places, preference_t preference) {
	priority_queue *pq;
	int dist[num_places+1];
	int pred[num_places +1];
	int source_index=-1;
	int i,j;
	q_element s[num_places];
	q_element temp;
	void *tmp; /*temp pointer for testing malloc */
	results_t result;
	/*initialise in case no match */
	result.place_ID=NO_MATCH;
	result.min_cost=INT_MAX;
	result.num_preds=0;
	
	/*work out the index of the requested city */
	/*confirmed working, case sensitively of course. */
	for(i=0;i<num_places;i++){
		if (strcmp(preference.source,places[i+1].city)==0) {
			source_index=i+1;
			break;
		}
	}
		
	/*initialise a priority queue which stores the index of the city,
	and is maintained in a heap, weighted on the cost from the source
	node*/
	/*test all mallocs to ensure they don't fail */
	tmp=malloc(sizeof(q_element)*num_places+sizeof(int)+
			sizeof(int)*num_places);
	if (!tmp) {
		printf("Failed to allocate memory \n");
		exit(EXIT_FAILURE);
	}
	pq=tmp;
	tmp=malloc(sizeof(q_element)*num_places);
	if (!tmp) {
		printf("Failed to allocate memory \n");
		exit(EXIT_FAILURE);
	}
	pq->q=tmp;
	tmp=malloc(sizeof(int)*num_places);
	if (!tmp) {
		printf("Failed to allocate memory \n");
		exit(EXIT_FAILURE);
	}
	pq->hash=tmp;
	/*initialise the data to be stored in the pq */
	for(i=0;i<num_places;i++) {
		temp.ID=i+1;
		/*initialise with costs from source */
		temp.cost=costs[source_index][i+1];
		s[i]=temp;
	}
	/*make sure the source gets pulled off the queue first. */
	s[source_index-1].cost=0;
	/*constructs heap using the data made above */
	make_heap(pq,s,num_places);
	
	/*check heap got made okay */
	if (DEBUG) {
		for(i=0;i<num_places;i++){
			printf("Min cost from city index %d is %d\n"
				,pq->q[i+1].ID,pq->q[i+1].cost);
		}
	}
	if (DEBUG) { 
	printf("source index is %d\n",source_index);
	/*sees if it read the costs correctly */
		for(i=0;i<num_places;i++) {
			for(j=0;j<num_places;j++) {
				printf("The cost from %s to %s is %d\n", 
					places[i+1].city,places[j+1].city,
					costs[i+1][j+1]);
			}
		}
	}
	
	/*initialise the dist,pred arrays */
	initialize(dist,pred,num_places+1,source_index);
	/*runs dijkstra's algorithm */
	run(pq,dist,pred,costs,source_index,preference,places,num_places);
	/*check if predecessor's were correctly determined*/
	if(DEBUG){
		for(i=0;i<=num_places;i++) {
			printf("The predecessor for %d is %d\n",i,pred[i]);
		}
	}
	/*okay, pred and dist arrays made, process results */
	result=results_processor(places,dist,preference,pred,num_places);
	return result;
					
}
/*function to initialise the distance and pred arrays */
void initialize(int dist[],int pred[],int num_vertices, int source) {
	int i;
	/*all costs except that to the source are initialised to INT_MAX*/
	for(i=0;i<=num_vertices;i++) dist[i] = INT_MAX;
	dist[source]=0;
	for(i=0;i<=num_vertices;i++) pred[i]=NO_PRED;
}


/*function that actually runs the important parts of dijkstra's algorithm */

void run(priority_queue *pq,int dist[],int pred[],int** costs, 
	int source_index, preference_t preferences, place_t places[],
							int num_places){

	int i;   /* counter */
	q_element temp; /*storage for what get's dequeued */
	
	/*keep going while queue isn't empty */
	while(pq->n>0) {
		/*take the root off the priority queue */
		temp=extract_min(pq);
		dist[temp.ID]=temp.cost;
		
		if(DEBUG){
		printf("Dist from source with identity %d is: %d \n",temp.ID ,
			temp.cost); 
		}
		
		
		for (i=0;i<num_places;i++) {
			/*safeguard from integer overflow */
			if(costs[temp.ID][i+1]!=INT_MAX){
			/*only updates if it will decrease the distance */	
				if(dist[temp.ID]+costs[temp.ID][i+1]
								<dist[i+1]) {
				/*calls update to ensure the priority queue
				  has the correct values and maintains the
				  heap property */
				update(i+1,temp.ID,pred,dist,pq,costs);
				}
			}
		}
	}
	/*okay dist/pred arrays populated */
	/*let's check out what we got, distance wise */
	if (DEBUG) {
		for(i=1;i<=num_places;i++) {
			printf("The min cost from %s to %s is: %d\n", 
				places[source_index].city,
					places[i].city, dist[i]);
		}
	}
			
}
/*function to check whether a particular place fulfills what the user
  requested/inputed. Returns 1 if the place is a match, 0 if it isn't. */
int check_match(int id, place_t places[], preference_t preferences) {
	/*have to compare a few things*/
	/*only bother checking continent is okay if it isn't 'any' */
	if (strcmp(preferences.continent,"any")) {
		if (strcmp(places[id].continent,preferences.continent)!=0) 
			return 0;
	}
	/*Only important to check if the preference matches the place if 
	  the user selected yes to cultural/outdoors interest required */
	if (strcmp(preferences.cultural,"Y")==0) {
		if (strcmp(places[id].cultural,"Y")!=0) return 0;
	}
	if (strcmp(preferences.outdoors,"Y")==0) {
		if (strcmp(places[id].outdoors,"Y")!=0) return 0;
	}
	/*it's a match! */
	return 1;
}

/*function to update the dist, pred arrays, as well as ensure the priority
  queue is up to scratch */
void update(int v,int current,int pred[], int dist[], priority_queue *pq,
		int **costs) {
	/*update dist/pred arrays */
	dist[v]=dist[current]+costs[current][v];
	pred[v]=current;
	/*update the priority queue in the right position, using its built in
	  hash table to ensure the correct index gets updated. This newly
	  updated node is then bubbled up to ensure the heap property is 
	  maintained */
	decrease_priority(pq,pq->hash[v],dist[v]);
	if (DEBUG) printf("The new distance to %d is: %d through %d \n",
							v,dist[v],pred[v]); 
}

/*function for processing the results. 
  finds the cheapest place (which fulfills the user inputted criteria) (if 
  there is one), then backtracks the predecessors (if there are any), 
  returning all this in a proprietry struct fit for this
  purpose. beyond the scope of dijkstra's algorithm */

results_t results_processor(place_t places[],int dist[],preference_t preference,
				int pred[], int num_places) {
	results_t result;  /*storage for to-be determined results */
	/*initialise in case no match */
	result.place_ID=NO_MATCH;
	result.min_cost=INT_MAX;
	result.num_preds=0;
	int i;
	/*process results to see if there was a match, and if so, to find
	  the cheapest such match */
	for (i=0;i<num_places;i++) {
		/*make sure not to include source by not allowing the
		  distance to be zero*/
		/*keep track of which place can be reached at the
		  lowest cost, given it fulfills the user preferences */  
		if(dist[i+1] < result.min_cost && dist[i+1]>0) {
			if(check_match(i+1,places,preference)){
				result.place_ID=i+1;
				result.min_cost=dist[i+1];
			}
		}
	}
	/*don't bother finding preds if no optimum place */
	if(result.place_ID==NO_MATCH) {
	/*algorithm's finished, return results (didn't find a suitable
	place */
	return result;
	}
	
	/*find preds*/
	i=result.place_ID;
	/*not quite worth re-allocating every time you have a new predecessor,
	  so just allocate enough room for the maximal number of predecessor.
	  This isn't such a big deal because it is just a simple array of ints
	  which won't use up any significant amount of memory on any modern 
	  computer */
	result.preds=malloc(sizeof(int)*num_places);
	/*make sure malloc didn't fail */
	if(!result.preds) {
		printf("Failed to allocate memory \n");
		exit(EXIT_FAILURE);
	}
	/*keep track of how many predecessors there are */
	result.num_preds=0;
	/*keep going until we arrive back to the source */
	while(pred[i]!=-1){
		result.preds[result.num_preds]=pred[i];
		i=pred[i];
		result.num_preds++;	
	}
	/*ensures the number of predecessors is returned alongside the other
	  data such as the destination, the minimum cost, and the identities
	  of all the predecessors */
	return result;

}
	
