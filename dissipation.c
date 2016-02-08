/*
 * Name: Nate Stewart
 * Date: 02-09-15
 * Description: program to solve the AMR dissipation problem without parallel processing.
 */

/*
 * Include the default libraries
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * Include the storage container for this class
 */
#include "dissipation.h"

/*
 * Hook into main program.
 */
int main( int argc, char **argv ) {
	
	/*
	 * Populate epsilon and affect_rate variables, checking if they were supplied as args. Otherwise, use defaults.
	 */
	float epsilon, affect_rate;
	if( argc > 1 ) {
		if( argc == 3 ) {
			epsilon = atof(argv[1]);
			affect_rate = atof(argv[2]);
		} else {
			printf("Invalid command line arguments. Either supply both epsilon and affect_rate or neither.\n");
			printf("%s [<epsilon> <affect_rate>]\n", argv[0]);
			return -1;
		}
	} else {
		epsilon = DEFAULT_EPSILON;
		affect_rate = DEFAULT_AFFECT_RATE;
	}

	/*
	 * Grab the header line of the grid data file
	 */
	int numGridBoxes, numRows, numCols;
	scanf("%d %d %d", &numGridBoxes, &numRows, &numCols);
	simpleBox *boxes = malloc(sizeof(simpleBox) * numGridBoxes);

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
		printf("Failed to read checksum on last line.\n");
		return -1;
	}
	printf("All lines read correctly\n");
   
	/*
	 * Compute the contact distances for all boxes
	 */
	gridBox *grid = malloc(sizeof(gridBox) * numGridBoxes);
	for( i = 0; i < numGridBoxes; i++) {
		grid[i].temp = boxes[i].temp;
		grid[i].perimeter = (boxes[i].dims.height + boxes[i].dims.width) * 2;
		
		// Calculate how many neighbors there are and allocate memory for neighbor temps and CDs	
		int numNeighbors = boxes[i].nei[0].num + boxes[i].nei[1].num + boxes[i].nei[2].num + boxes[i].nei[3].num;
		grid[i].neiTemps = malloc(sizeof(double) * numNeighbors);
		grid[i].neiCD = malloc(sizeof(int) * numNeighbors);
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
				currBox = &boxes[(currNeighbor -> ids)[k]];
				grid[i].neiTemps[neiTempIndex] = currBox -> temp; // Populate the temperature for the gridBox neighbor 
				// Slightly different algorithm depending on whether the neighbor is left/right or top/bot
				if( j < 2 ) { // 0,1 is top and bottom
					grid[i].neiCD[neiTempIndex] = calculateContactDistance(boxes[i].dims.ul_x, boxes[i].dims.br_x, (currBox -> dims).ul_x, (currBox -> dims).br_x);
				} else { // 2,3 is left and right
					grid[i].neiCD[neiTempIndex] = calculateContactDistance(boxes[i].dims.ul_y, boxes[i].dims.br_y, (currBox -> dims).ul_y, (currBox -> dims).br_y);
				}
			}
		}
		

	}

    /*
     * Free the memory of all boxes
     */
    for( i = 0; i < numGridBoxes; i++ ) {
        for( j = 0; j < 4; j++) {
			free(boxes[i].nei[j].ids);
		}
    }
    free(boxes);
    
	/*
     * Time to do the math.
     * Compute the AMR Dissipation to convergence
     */
   	
	return 0;
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




