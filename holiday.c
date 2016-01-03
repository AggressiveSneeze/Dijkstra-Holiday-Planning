#include "holiday.h"



int
main(int argc, char *argv[]) {
	
	FILE  *fp;
	int num_places; /*storage for the number of places to be read from
				command line input */
				
	int places_read=0,costs_read=0; /*counters for error checking purposes*/
	char line[MAXLINELEN];/*storage for when a line is read from a command
				line argument */
	char city[MAXFIELDLEN],continent[MAXFIELDLEN], cultural[MAXFIELDLEN];
	char outdoors[MAXFIELDLEN];
	char place_index[MAXFIELDLEN];
	
	int source_no=0,destination_no=0,cost=0; /*storage for reading the costs
							file */
	preference_t *preference=NULL; /*eventual storage for user inputted
					preferences */
	
	int** costs; /*storage for costs matrix (2D dynamically allocated
							array ) */
	int x,y; /*counters */
	results_t results; /*storage for the results from running Dijkstra's. */

	/*usage explanation/ensures correct command line arguments */
	if(argc != 3)
	{   	printf("Usage: holiday places costs where places and costs ");
		printf(" are the input files. \n");
		exit(ARGERROR);
	}          
	
	/* open places file for reading*/
	if( (fp=fopen(argv[1],"r")) ==NULL) {
		printf("Error opening file %s\n", argv[1]);
		exit(FILEERROR);
	}
	
	/* read from places file  */
	
	/*ensures file isn't empty*/
	if((fgets(line,sizeof(line),fp)) ==NULL) {
		printf("Error, file %s is empty\n",argv[1]);
		exit(FILEERROR);
	}
	/* get the number of places from first line */
	sscanf(line,"%d",&num_places);
	
	/*initialise an array of places of correct size now that we know it */
	place_t places[num_places+1];

	/*Initiate the distance array too */
	/*malloc the 'y' dimension */
	costs= malloc(sizeof(int*) * num_places+1);
	/*test malloc */
	if(!costs) {
		printf("Failed to allocate memory \n");
		exit(EXIT_FAILURE);
	}
		
	/*iterate over 'y' dimension*/
	for(y=0;y<num_places+1;y++){
		/*malloc the 'x' dimension, and test in case of failure*/
		costs[y] = malloc(sizeof(int) * num_places+1);
		if (!costs[y]) {
			printf("Failed to allocate memory \n");
			exit(EXIT_FAILURE);
		}

		/*iterate over the 'x' dimension */
		for(x=0;x<num_places+1;x++){
			/*initialize to INT_MAX */
			if(y!=x) costs[y][x]=INT_MAX;
			/*distance from node to self*/
			else costs[y][x]=0;
		}
	}
	
	/*reads the places file */
	while(fgets(line, sizeof(line), fp)) {
		
		/* check the line actually got read correctly */
		if(DEBUG) printf("READ LINE %d:  %s", places_read+1,line);
		
		/* get fields into buffers*/
		if(sscanf(line,"%s %s %s %s %s", place_index, city,
				continent,cultural,outdoors)!=5) {
			printf("Incorrect number of fields in places file, "); 
			printf("record number %d\n", places_read++);
			exit(EXIT_FAILURE);
		}
		
		/* do something with the data */
		/*convert place_index string to int */
		if(DEBUG) printf("The place_index is %s\n",place_index);
		add_to_array(place_index,city,continent,cultural,outdoors,
					places,places_read);
		/*increment places read */
		
		places_read++;
			
	}
	
	/*make sure we read the same number of records that it said at the top
	  of the file there would be */
	if(places_read!=num_places) {
		printf("Discrepency between the number of records specified");
		printf("in the first line, and the number of records read.");
		printf("Exiting.\n");
		exit(FILEERROR);
	}
	/*all the places read in, and stored in places */
	/* done with the places files, time to open the costs file for reading*/
	
	if( (fp=fopen(argv[2],"r")) ==NULL) {
		printf("Error opening file %s\n", argv[2]);
		exit(FILEERROR);
	}
	
	/*reads the costs file */
	
	while (fgets(line,sizeof(line), fp)) {
		/*increment costs read */
		costs_read++;
		/* check the line actually got read correctly */
		if(DEBUG) printf("READ LINE %d:  %s", costs_read,line);
		/* get fields into buffers */
		if(sscanf(line,"%d %d %d", &source_no,&destination_no,
							&cost)!=3) {
			printf("Incorrect number of fields in costs file, ");
			printf("record number %d\n", costs_read++);
			exit(EXIT_FAILURE);
		}
		/*do something with the data */
		costs[source_no][destination_no]=cost;
		/*the line below can be decommented to force symmetry for 
		  a non symmetrical cost file. */
		/*costs[destination_no][source_no]=cost;*/
		if (DEBUG) printf("The distance from %d to %d is %d \n",
					source_no,destination_no, 
					costs[source_no][destination_no]);
							
	}
	
	/*If costs file was empty, costs_read will never get incremented */
	if(costs_read==0) {
		printf("Costs file was empty!Exiting! \n");
		exit(FILEERROR);
	}
	
	/*okay, data read, and stored.
	places in places[].
	costs in costs[][].
	*/
	/* user input time */
	preference=user_input(places,num_places);
	/*run dijkstra's algorithm!  */
	results=dijkstra(places,costs,num_places,*preference);
	/*print the correct output using the results of running dijkstra's */
	results_printer(places,results);
	
	/*this is technically the end of the program. Please note: Given this is
	a once-only program, there is no need to free() all the memory used
	by mallocs and initialisations, as it will not be running in the 
	background consuming memory */
	
	return 0;
}


/* adds a new place's info into the array at the correct index */
void add_to_array(char place_index[],char city[],char continent[],
		char cultural[],char outdoors[], place_t places[], 
		int places_read) {
	place_t *temp_place=malloc(sizeof(place_t));
	if (!temp_place) {
		printf("Failed to allocate memory \n");
		exit(EXIT_FAILURE);
	}
	/*convert the string to its integer equivalent, then store */
	temp_place->placeID=atoi(place_index);	
	/* allocate enough room for strings within the place node */
	temp_place->city= (char *)malloc(sizeof(char) * (strlen(city)+1));
	temp_place->continent= (char *)malloc(sizeof(char) * 
						(strlen(continent)+1));
	if (!(temp_place->continent)) {
		printf("Failed to allocate memory \n");
		exit(EXIT_FAILURE);
	}
	temp_place->cultural=(char *)malloc(sizeof(char) * 
						(strlen(cultural)+1));
	if (!(temp_place->cultural)) {
		printf("Failed to allocate memory \n");
		exit(EXIT_FAILURE);
	}
	temp_place->outdoors=(char *)malloc(sizeof(char) * 
						(strlen(outdoors)+1));
	if(!(temp_place->outdoors)) {
		printf("Failed to allocate memory \n");
		exit(EXIT_FAILURE);
	}
	/* copy the fields into the temp place node */
	strcpy(temp_place->city,city);
	strcpy(temp_place->continent,continent);
	strcpy(temp_place->cultural,cultural);
	strcpy(temp_place->outdoors,outdoors);
	/*add the place to the places array */
	places[places_read+1]=*temp_place;
}

/*Function to prompt the user for input and store their holiday preferences */
preference_t* user_input(place_t places[],int num_places) {
	char starting_point[MAXFIELDLEN], preferred_cont[MAXFIELDLEN];
	char cultural_req[MAXFIELDLEN],outdoor_req[MAXFIELDLEN];
	int comp=1;
	int i=0,error=0; /*counter*/
	preference_t *preference;
	
	/*explanation of these while loops: 
	  if the error counter has been incremented, the prompt has been said
	  but the user made a mistake with input, so tell them that and try
	  again. if the comp value is 0, it means the input is okay and we can
	  stop trying*/
	printf("Enter your starting point: \n");
	while(comp!=0) {
		if(error>0) 
			printf("No city exists with that name. Try again:\n");
		/*read the city from stdin */
		fgets(starting_point, MAXFIELDLEN, stdin);
		/*remove the trailing \n */
		starting_point[strlen(starting_point)-1]='\0';
		for (i=0;i<num_places;i++){
			comp=strcmp(starting_point,places[i+1].city);
			if (comp==0) break;
		}
		error++;
	}
	comp=1;
	error=0;
	printf("Enter the continent you prefer [Australia, Asia");
	printf(",NorthAmerica,SouthAmerica,Europe,Africa,Antarctica] \n");
	while(comp!=0) {
		if(error>0){
			printf("No such continent exists with that name.");
			printf(" Try again:\n");
		}
		/*read the continent from stdin */
		fgets(preferred_cont, MAXFIELDLEN, stdin);
		/*compare first to blank input */
		comp=strcmp(preferred_cont,"\n");
		if(!comp) {
			/*it's blank, so any continent is okay */
			strcpy(preferred_cont,"any");
			printf("You have not selected a desired continent.");
			printf("Any continent will be allowed. \n");
			break;
		}
		/*otherwise remove the trailing \n */
		preferred_cont[strlen(preferred_cont)-1]='\0';
		/*not worth dynamically allocating an array of continent names
		  just copy and paste, change continent name */
		comp=strcmp(preferred_cont,"Australia");
		if (!comp) break;
		comp=strcmp(preferred_cont,"Asia");
		if (!comp) break;
		comp=strcmp(preferred_cont,"NorthAmerica");
		if (!comp) break;
		comp=strcmp(preferred_cont,"SouthAmerica");
		if (!comp) break;
		comp=strcmp(preferred_cont,"Europe");
		if (!comp) break;
		comp=strcmp(preferred_cont,"Africa");
		if (!comp) break;
		comp=strcmp(preferred_cont,"Antarctica");
		if (!comp) break;
		error++;
	}
	comp=1;
	error=0;
	printf("Cultural interest required?  [Y/N]\n");
	while(comp!=0) {
		if(error>0) {
			printf("Please enter Y or N please: \n");
		}
		/*read from stdin */
		fgets(cultural_req, MAXFIELDLEN, stdin);
		/*remove the trailing \n */
		cultural_req[strlen(cultural_req)-1]='\0';
		/*If it's either Y or N, that's okay */
		if (!strcmp(cultural_req,"Y") || !strcmp(cultural_req,"N")){
			comp=0;
			break;
		}
		/*if return is hit, that's fine, default is "N" */
		if (!strcmp(cultural_req,"")) {
			strcpy(cultural_req,"N");
			printf("You have autoselected %s\n",cultural_req);
			comp=0;
			break;
		}
		error++;
	}
	comp=1;
	error=0;
	printf("Outdoor activities required? [Y/N] \n");
	while(comp!=0) {
		if(error>0) {
			printf("Please enter Y or N please: \n");
		}
		/*read from stdin */
		fgets(outdoor_req, MAXFIELDLEN, stdin);
		/*remove the trailing \n */
		outdoor_req[strlen(outdoor_req)-1]='\0';
		/*If it's either Y or N, that's okay */
		if (!strcmp(outdoor_req,"Y") || !strcmp(outdoor_req,"N")) {
			comp=0;
			break;
		}
		
		/*if return is hit, that's fine, default is "N" */
		if (!strcmp(outdoor_req,"")) {
			strcpy(outdoor_req,"N");
			printf("You have autoselected %s\n",outdoor_req);
			comp=0;
			break;
		}
		error++;
	}
	/*store the user input in a defined data structure */
	preference=pref_maker(starting_point,preferred_cont,cultural_req,
							outdoor_req);
	return preference;
}
/*function to handle the correct output being printed to stdout */
void results_printer(place_t places[],results_t results) {
	int i;
	/*flag in case there were no suitable matches */
	if (results.place_ID==-1) {
		printf("Your preferences could not be accomodated. \n");
		return;
	}
	/*print the correct output to stdout for stage 1*/
	printf("%s   $%d \n",places[results.place_ID].city,results.min_cost);
	
	/*print the correct output to stdout for stage 2 */
	for (i=results.num_preds-1;i>=0;i--) {
		printf("%s ",places[results.preds[i]].city);
	}
	printf("%s\n",places[results.place_ID].city);
	return;
}


/*makes a preference node from the details sent as arguments
   and returns a pointer to it */
preference_t* pref_maker(char source[], char continent[], char cultural[],
			char outdoors[]){
        /*allocate enough memory for everything */
	preference_t *temp=malloc (sizeof(preference_t));
	/*don't have to assign to a temp pointer, exiting anyway */
	if (!temp) {
		printf("Failed to allocate memory \n");
		exit(EXIT_FAILURE);
	}
	temp->source=(char *)malloc(sizeof(char)*strlen(source)+1);
	if (!(temp->source)) {
		printf("Failed to allocate memory \n");
		exit(EXIT_FAILURE);
	}
	temp->continent=(char *)malloc(sizeof(char)*strlen(continent)+1);
	if (!(temp->continent)) {
		printf("Failed to allocate memory \n");
		exit(EXIT_FAILURE);
	}
	temp->cultural=(char *)malloc(sizeof(char)*strlen(cultural)+1);
	if (!(temp->cultural)) {
		printf("Failed to allocate memory \n");
		exit(EXIT_FAILURE);
	}
	temp->outdoors=(char *)malloc(sizeof(char)*strlen(outdoors)+1);
	if (!(temp->outdoors)) {
		printf("Failed to allocate memory \n");
		exit(EXIT_FAILURE);
	}
	/*copy across the user input to the preference struct */
	strcpy(temp->source,source);
	strcpy(temp->continent,continent);
	
	strcpy(temp->cultural,cultural);
	strcpy(temp->outdoors,outdoors);
	
	if (DEBUG) {
		printf("The source is: %s \n",temp->source);
		printf("The continent is: %s \n",temp->continent);
		printf("The cultural is: %s \n",temp->cultural);
		printf("The outdoors is: %s \n",temp->outdoors);
	}
	/*return the preference struct */
	return temp;
}
