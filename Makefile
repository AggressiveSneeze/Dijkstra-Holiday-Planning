# Basic Makefile for COMP20003 AlgDS assignment 2, semester 2, 2014
#
# Directs compilation of single program holiday
#
#Author: James Adams
#
#
# This file organization can be changed, but the Makefile need to be
# changed correspondingly


holiday: holiday.o dijkstra.o heap.o
	gcc -o holiday holiday.o dijkstra.o heap.o

			
holiday.o: holiday.c holiday.h
	gcc -c -Wall holiday.c	

dijkstra.o: dijkstra.c dijkstra.h
	gcc -c -Wall dijkstra.c

heap.o: heap.c heap.h
	gcc -c -Wall heap.c
	

