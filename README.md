# Dijkstra-Holiday-Planning
A holiday planning program in C implementing Dijkstra's algorithm with a priority queue. 
Completed for COMP20003: Semester 1, 2014 at the University of Melbourne.

Design document:

Author: James Adams

Bullet point run through of the how the program works:

* Compile via make holiday

* Run via "./holiday places costs" where places and costs are the files which contain the relevant records.

* Take the number of places from the first line of the places file. This allows the program to initialise an array of places of the correct size, as well as dynamically allocate a 2D array of the correct size for the costs between every place.

* This 2D array is then used to store the costs from all the different sources to destinations that are now read.

* Program will then ask for user input as directed in the spec, and then use dijsktra's algorithm to solve for the min cost from the source to all other places, keeping track of paths via the preds[] array in the process.

* After this, the program will cycle through the distances array to try and find the cheapest place which satisfies all the user requirements. If there are no applicable locations, the program will print a message to that effect and then promptly exit.

* Otherwise, the predecessors will now be backtracked so that a path from the source to the cheapest applicable destination can be found.

* As per the spec, the cheapest location, its cost, and the path from the source to the aforementioned destination is now printed to stdout, and the program exits.


Discussion of algorithms used:

This program uses Dijkstra's algorithm on a priority queue, implemented via a binary heap. The data structure used as the 'node' of the pq is a shaved off version of the place node, only containing the place index and the cheapest cost to source, which is used as the 'priority' value for the pq. 

The choice to use Dijkstra's algorithm was quite straightforward, as it satisifes the requirements of the spec quite completely. We essentially have a weighted graph, without any negative or variable edges or negative cycles, which is exactly what Dijkstra's algorithm was designed for. 

Another choice of shortest path graph algorithms could have been the Floyd-Warshall algorithm, but it has numerous drawbacks:

* It does not find the details of the paths (by not keeping track of predecessors).
* It goes above and beyond the requirements of the spec by calculating the lengths of the shortest paths between ALL pairs of vertices.
* Its complexity is significantly worse than Dijsktra's algorithm, having best and worst case performance of V^3, where V is the number of vertices. Dijsktra's on the other hand, is O((V+E) ln(V)) (using a heap), which is orders of magnitude better than the performance of the Floyd-Warshall algorithm. The choice was not a hard one.

Continuing, my implementation of Dijkstra's used a binary heap to implement the priority queue. There wasn't much choice involved here, as a heap is without a doubt the most efficient way to implement a priority queue, as it provides all the functionality required from Dijkstra's. Potentially a Fibonacci heap or a Pairing heap could have been better in some situations, but I imagine an implementation of either of these data structures would have been beyond the scope of the course.

My only difficulty with implementing Dijsktra's algorithm was creating the decrease_priority() function (for the pq) in an efficient manner. This was because the index of the heap (elements are stored in an array which maintains the heap property) will not necessarily be the same as the index of the place (in the context of the place file index numbers), which makes decreasing a priority (i.e reducing the min cost to a place in the pq, as required by Dijkstra's) a little bit difficult. This was overcome by introducing a hash map to the priority queue structure, which nicely keeps track of where every place is within the heap array, by storing in the place_index'th index of the hash table its index within the heap.

Complexity analysis of my program:
V= number of places
E= number of costs/edges between places

Time-wise (using key comparisons as the most expensive operation) (Worst-case):

Honestly, the only thing that contributes to the complexity calculation of my program (if only considering key comparisons) is the use of Dijsktra's algorithm, which is implemented in the stock-standard way via a PQ using a heap. 

If each element had been inserted into the heap using the pq_insert() function, construction of the heap would be O(VlogV) according to Skiena, but using the make_heap() function, it is done in linear time, O(V).

Each extract_min() call is O(logV), and this call is made V times, so this adds VlogV.
Each decrease_priority() call is also O(logV) because it shifts the heap to maintain the heap condition, and this is done E times, so this adds ElogV to the complexity.

The comparison between the dist array and the user inputted preferences would be O(V), while the backtracking of predecessors would be constant time O(1)

Overall, the sum total of the time complexity would be:
V+VlogV +ElogV +V. This is O((V+E)logV) because the linear terms are irrelevant (from the definition of big O (...) ). So the overall complexity is indeed O((V+E)logV) in worst-case, the same as the literature on Dijsktra's.

Using a Fibonacci heap would have reduced this to O(E+VlogV), a little bit better, but it wasn't really feasible to implement, as mentioned above, and I don't believe this subject calls for it.

Space-wise:

Places stored in array: O(V)*sizeof(place_t)
Costs stored in 2d array: O(V^2)

Dijsktra's uses a heap: O(V)
		    pred array: O(V)
		    dist array: O(V)

Optimum place choosing:
	Potentially O(V) if we have to store every single vertex as a predecessor, but only storing ints for that, because only the index is recorded in the returned results_t struct.

I guess space-wise the complexity is O(V^2), but the space-usage is non linear, because some of the above take up more room if they're storing a place_t struct rather than an int, which makes the space-complexity a little more fiddly to state precisely.


Design decisions:

The choice of data structures to use was quite involved actually. It was very easy to choose a heap and to make that sort of major decign decision, but it was probably the smaller decisions which were more difficult to make.

PQ element choice was initially place_t, but eventually I saw that it was silly to store the places both in an array and in the pq, so it was decided in the end to only store the min cost and the id, and then use the ID to get the place details from the original array.

The implementation of the hash table into the pq data structure was definitely needed, but in the beginning I was trying to work out if the pq would work without it, and I was also a bit worried that it would be hard to implement. It actually wasn't very hard, and only involved an extra 4 lines of code to implement the hash table.

A few extra data structs were defined to deal with sending things back and forth between functions, such as preference_t and results_t, but there weren't really any decisions involved in making them, as they just fulfilled a need.


The breakdown of the program into holiday,dijkstra and heap was made at the start and seemed the most logical way to go about it. holiday.c would be responsible for all the user input/command line input as well as calling dijkstra's and facilitating the correct output.

dijsktra.c would be responsible for implementing the algorithm,and processing the results (in hindsight, the results processing should have been done in holiday, but it was much easier not to have to send the pred and dist arrays back to holiday, and just do it through a function within dijkstra). 

heap.c would be responsible for implementing all the necessary functions for constructing and maintaining a priority queue using a heap.

If I was redoing the project, I would shift the results processing over to holiday, as well as the move the definition of the place_t node from heap to holiday, but otherwise I'm quite content with how the file separation worked out.

N.B I left all the if(DEBUG) error printings within the source code because 
a) they don't interfere with the readability of the code, besides making the functions longer.
b) This program is a prototype for an actual purpose. If it is expected that others will build on it eventually, they may want to make use of the DEBUG statements.



Definitely a fun project.









