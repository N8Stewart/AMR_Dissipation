/*
 * Name: Nate Stewart
 * Date: 03-06-15
 * Description: program to solve the AMR dissipation problem without parallel processing.
 */

/*
 * Include the default libraries
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

/*
 * Include the definitions and structures 
 */
#include "stewart_nate_dissipation.h"

// Had to make these global so they can be accessed by all threads
double maxTemp, minTemp;
double *threadMaxTemp, *threadMinTemp; // Each thread will record their own Min/Max to be aggregated 
double epsilon, affectRate;
int iter;
int numThreads;
// Declare a global barrier for the threads
pthread_barrier_t barrier;
pthread_barrierattr_t barrier_attr;

/*
 * Hook into main program.
 */
int main( int argc, char **argv ) {

	/*
	 * Variables used for time and clock
	 */
	time_t startTime, endTime;
	clock_t clockTime;

	int i, j;
	if( argc == 4 ) {
		epsilon = atof(argv[1]);
		affectRate = atof(argv[2]);
		numThreads = atoi(argv[3]);
	} else {
		printf("Invalid command line arguments. Please supply all parameters in the below format.\n");
		printf("%s [<epsilon> <affect_rate> <num_threads>]\n", argv[0]);
		return -1;
	}

	/*
	 * Declare the pthreads on the heap
	 */
	pthread_t *threads = malloc(sizeof(pthread_t) * numThreads);
	threadMaxTemp = malloc(sizeof(double) * numThreads);
	threadMinTemp = malloc(sizeof(double) * numThreads);
	/*
	 * Read the data file into the boxes structure 
	 */
	int numGridBoxes, numRows, numCols;
	scanf("%d %d %d", &numGridBoxes, &numRows, &numCols); // header file
	simpleBox *boxes = malloc(sizeof(simpleBox) * numGridBoxes);
	if(getInput(boxes, numGridBoxes) != 0) { // The rest of the data file
		printf("Failed to read checksum on last line.\n");
		return -1;	
	}
 
	/*
	 * Create the grid and transfer the useful information from the boxes into the grid
	 */
	gridBox *grid = malloc(sizeof(gridBox) * numGridBoxes);
	transferToGrid(grid, boxes, numGridBoxes);

    /*
     * Free the memory of all simple boxes
     */
    for( i = 0; i < numGridBoxes; i++ ) {
        for( j = 0; j < 4; j++) {
			free(boxes[i].nei[j].ids);
		}
    }
    free(boxes);
    
	/* 
	 * Compute the max/min temps for initial values
	 */
   	double *newTemps = malloc(sizeof(double) * numGridBoxes);	
	iter = 0; // number of iterations
	
	/*
	 * Insert the temps in the grid structure with the newTemps array
	 * Grab the max and min temperatures
	 */
	for( i = 0; i < numGridBoxes; i++) {
		newTemps[i] = grid[i].temp;
	}
	getMinMax(newTemps, numGridBoxes, &maxTemp, &minTemp);

	/*
	 * Construct the storage box for all threads
	 * We will be blocking the gridboxes instead of cyclic distribution.
	 * For example: 
	 * 		Thread 1: compute grid boxes [0 to (numGridBoxes / numThreads)]
	 */
	threadStorage *storage = malloc(sizeof(threadStorage) * numThreads);
	{
		int start;
		int spacing = numGridBoxes / numThreads;
		for( i = 0, start = 0; i < numThreads; i++, start += spacing ) {
			storage[i].id = i;
			storage[i].newTemps = &newTemps[start];
			storage[i].grid = &grid[start];
			storage[i].numGridBoxes = numGridBoxes / numThreads;
		}
		// Very last thread must pick up the remainder if numGridBoxes/numThreads is not an even integer
		storage[numThreads - 1].numGridBoxes += numGridBoxes - (spacing * numThreads);
	}

	/*
	 * Construct the barrier
	 */
	if( pthread_barrier_init(&barrier, &barrier_attr, numThreads) ) {
		fprintf(stdout, "Unable to create barrier.\n");
		return -1;
	}

	/*
     * Time to do the math.
     * Compute the AMR Dissipation to convergence
     */
	time(&startTime);
	clockTime = clock();
	
	/* 
	 * Spawn off all threads
	 */
	for( i = 0; i < numThreads; i++){
		int errno = pthread_create(&threads[i], NULL, threadEntry, &storage[i]);
		if( errno ) {
			fprintf(stdout, "Iteration %5d | Error creating thread %3d| ERROR: %5d\n", iter, i, errno);
		}
	}

	/* 
	 * Join all threads
	 */
	for( i = 0; i < numThreads; i++){
		int errno = pthread_join(threads[i], NULL);
		if( errno ) {
			fprintf(stdout, "Iteration %5d | Error joining thread %3d| ERROR: %5d\n", iter, i, errno);
		}
	}

	/*
	 * Stop the timers
	 */
	time(&endTime);
	clockTime = clock() - clockTime;

	printf("*********************************************************************\n");
	printf("dissipation converged in %d iterations,\n", iter);
	printf("\twith max DSV\t= %lf and min DSV\t= %lf\n", maxTemp, minTemp);
	printf("\taffect rate\t= %lf;\tepsilon\t= %lf\n", affectRate, epsilon);
	printf("\n");
	printf("elapsed convergence loop time\t(clock): %lu\n", clockTime);
	printf("elapsed convergence loop time\t (time): %.f\n", difftime(endTime, startTime));
	printf("*********************************************************************\n");

    /*
     * Free the memory of all grid boxes, the temporary 'newTemps' variable, the threads, and the storage box
     */
    for( i = 0; i < numGridBoxes; i++ ) {
		free(grid[i].neiTemps);
		free(grid[i].neiCD);
    }
    free(grid);
	free(newTemps);
	free(threads);
	free(storage);
	// Free the thread Min/Max
	free(threadMinTemp);
	free(threadMaxTemp);
	// Destroy the barrier
	pthread_barrier_destroy(&barrier);

	return 0;
}

void *threadEntry(void *storage) {

	int i;
	threadStorage *ts = (threadStorage *)storage;
	int numGridBoxes = ts -> numGridBoxes;

	while( (maxTemp - minTemp) > (epsilon * maxTemp) ) {
	
		// Compute new temps
		for( i = 0; i < numGridBoxes; i++ ) {
			(ts -> newTemps[i]) = computeDSV(&(ts -> grid[i]), affectRate);
		}

		// Calculate the min/Max for the current subset
		getMinMax(ts -> newTemps, numGridBoxes, &(threadMaxTemp[ts -> id]), &(threadMinTemp[ts -> id]));
		
		// Wait for all threads after calculating DSV's for the current set
		int ret = pthread_barrier_wait(&barrier);

		// If we are on the master thread 'THREAD_BARRIER_SERIAL_THREAD'
		if( ret ) {
			double trash; // Trash value 
			// Aggregate thread min/max temperatures
			getMinMax(threadMaxTemp, numThreads, &maxTemp, &trash);
			getMinMax(threadMinTemp, numThreads, &trash, &minTemp);

			iter++;
		}
		
		// Update the temps in the grid structure with the newTemps array
		for( i = 0; i < numGridBoxes; i++) {
			(ts -> grid[i]).temp = ts -> newTemps[i];
		}
		
		// Wait for the Min/Max to be calculated before assessing if DSV's have converged.
		pthread_barrier_wait(&barrier);

	}

	return NULL;

}

double computeDSV(gridBox *box, double affectRate) {

	int i;
	double currentTemp = box -> temp;
	double sumTemp = (box -> edgeContact) * currentTemp;
	// Sum all of the neighbor affects
	for( i = 0; i < (box -> numNeighbors); i++) {
		sumTemp += *(box -> neiTemps)[i] * (box -> neiCD)[i]; 
	}
	double avgTemp = sumTemp / (box -> perimeter);
		
	// Compute new temp and update max/min
	return (currentTemp - (currentTemp - avgTemp) * affectRate);
}

void getMinMax(double *temps, int numTemps, double *maxTemp, double *minTemp) {
	*maxTemp = temps[0]; *minTemp = temps[0];
	int i;
	for( i = 1; i < numTemps; i++ ) {
		double register checkTemp = temps[i];
		if( checkTemp > *maxTemp ) {
			*maxTemp = checkTemp;
		}
		if( checkTemp < *minTemp ) {
			*minTemp = checkTemp;
		}
	}
}

int getInput(simpleBox *boxes, int numGridBoxes) {
	
	/*
	 * Grab each data line 1 by 1
	 */
	int i, j, k;
	for( i = 0; i < numGridBoxes; i++ ) {
		
		// Line 2: id
		scanf("%d", &boxes[i].id);
	
		// Line 3: coordinates
		dimensions *dims = &boxes[i].dims;
		scanf("%d %d %d %d", &(dims -> ul_y), &(dims -> ul_x), &(dims -> height), &(dims -> width));
		dims -> br_x = dims -> ul_x + dims -> width;
		dims -> br_y = dims -> ul_y + dims -> height;
		
		// Line 4-7: Neighbors
		neighbors *currNeighbor;
		for( j = 0; j < 4; j++ ) {
			currNeighbor = &boxes[i].nei[j];
			scanf("%d", &(currNeighbor -> num));
			if( (currNeighbor -> num) > 0 ) {
				(currNeighbor -> ids) = malloc(sizeof(int) * (currNeighbor -> num));
				for( k = 0; k < (currNeighbor -> num); k++ ) {
					scanf("%d", (currNeighbor -> ids) + k);
				}
			}
			
		}
		
		// Line 8: temperature
		scanf("%lf", &boxes[i].temp);
	}

	/*
	 * Ensure the last line is '-1'. If so, the data has been read in correctly (most likely)
	 */
	int lastLine;
	scanf("%d", &lastLine);
	if ( lastLine != -1) {
		return -1;
	}

	return 0;
}

void transferToGrid(gridBox *grid, simpleBox *boxes, int numGridBoxes) {
	
	/*
	 * Allocate memory and populate generic fields for all gridBox elements
	 */
	int i, j, k;
	for( i = 0; i < numGridBoxes; i++) {
		grid[i].temp = boxes[i].temp;
		grid[i].perimeter = (boxes[i].dims.height + boxes[i].dims.width) * 2;
		grid[i].edgeContact = grid[i].perimeter;
		
		// Calculate how many neighbors there are and allocate memory for neighbor temps and CDs	
		int numNeighbors = boxes[i].nei[0].num + boxes[i].nei[1].num + boxes[i].nei[2].num + boxes[i].nei[3].num;
		grid[i].neiTemps = malloc(sizeof(double *) * numNeighbors);
		grid[i].neiCD = malloc(sizeof(int) * numNeighbors);
		grid[i].numNeighbors = numNeighbors;
	}

	/*
	 * Calculate contact distances and point to the correct temperature inside gridbox
	 */
	for( i = 0; i < numGridBoxes; i++ ) {
		// Pointers to alleviate repetitive typing
		neighbors *currNeighbor;
		simpleBox *currBox;
		// Index of the current neighbor to be put in the current gridBox
		int neiTempIndex = 0;
		// Loop through all of the neighbors and calculate the contact distances
		for( j = 0; j < 4; j++ ) {
			currNeighbor = &boxes[i].nei[j];
			for( k = 0; k < (currNeighbor -> num); k++, neiTempIndex++) {
				// Grab the current simple box and the current dimensions of that box
				int currId = (currNeighbor -> ids)[k];
				currBox = &boxes[currId];
				// Slightly different algorithm depending on whether the neighbor is left/right or top/bot
				int contactDistance;
				if( j < 2 ) { // 0,1 is top and bottom
					contactDistance = calculateContactDistance(boxes[i].dims.ul_x, boxes[i].dims.br_x, (currBox -> dims).ul_x, (currBox -> dims).br_x);
				} else { // 2,3 is left and right
					contactDistance = calculateContactDistance(boxes[i].dims.ul_y, boxes[i].dims.br_y, (currBox -> dims).ul_y, (currBox -> dims).br_y);
				}
				grid[i].neiCD[neiTempIndex] = contactDistance;
				grid[i].neiTemps[neiTempIndex] = &grid[currId].temp;
				grid[i].edgeContact -= contactDistance; // Subtract box CD from the overall CD
			}
		}
	}
}

int calculateContactDistance(int box1_ul, int box1_br, int box2_ul, int box2_br) {
	if( box1_ul <= box2_ul ) {
		if( box1_br >= box2_br ) {
			return box2_br - box2_ul;
		} else { // box1_br < box2_br
			return box1_br - box2_ul;
		}
	} else { // box1_ul > box2_ul
		if( box1_br <= box2_br ) {
			return box1_br - box1_ul;
		} else { // Box1_br > box2_br
			return box2_br - box1_ul;
		}
	}
}




