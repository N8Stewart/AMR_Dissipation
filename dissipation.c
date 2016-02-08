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
	box *boxes = malloc(sizeof(box) * numGridBoxes);

	/*
	 * Grab each data line 1 by 1
	 */
	int i, j;
	for( i = 0; i < numGridBoxes; i++ ) {
		// Line 2: id
		scanf("%d", &boxes[i].id);
		// Line 3: coordinates
		dimensions *dims = &boxes[i].dims;
		scanf("%d %d %d %d", &(dims -> ul_y), &(dims -> ul_x), &(dims -> height), &(dims -> width));
		dims -> br_x = dims -> ul_x + dims -> width;
		dims -> br_y = dims -> ul_y + dims -> height;
        dims -> perimeter = (dims -> width) * 2 + (dims -> height) * 2;
		// Line 4: top neighbors
		neighbors *currNeighbor;
		currNeighbor = &boxes[i].n_top;
		scanf("%d", &(currNeighbor -> num));
		if( (currNeighbor -> num) > 0 ) {
			(currNeighbor -> ids) = malloc(sizeof(int) * (currNeighbor -> num));
			for( j = 0; j < (currNeighbor -> num); j++ ) {
				scanf("%d", (currNeighbor -> ids) + j);
			}
		}
		// Line 5: bottom neighbors
		currNeighbor = &boxes[i].n_bot;
		scanf("%d", &(currNeighbor -> num));
		if( (currNeighbor -> num) > 0 ) {
			(currNeighbor -> ids) = malloc(sizeof(int) * (currNeighbor -> num));
			for( j = 0; j < (currNeighbor -> num); j++ ) {
				scanf("%d", (currNeighbor -> ids) + j);
			}
		}
		// Line 6: left neighbors
		currNeighbor = &boxes[i].n_left;
		scanf("%d", &(currNeighbor -> num));
		if( (currNeighbor -> num) > 0 ) {
			(currNeighbor -> ids) = malloc(sizeof(int) * (currNeighbor -> num));
			for( j = 0; j < (currNeighbor -> num); j++ ) {
				scanf("%d", (currNeighbor -> ids) + j);
			}
		}
		// Line 7: right neighbors
		currNeighbor = &boxes[i].n_right;
		scanf("%d", &(currNeighbor -> num));
		if( (currNeighbor -> num) > 0 ) {
			(currNeighbor -> ids) = malloc(sizeof(int) * (currNeighbor -> num));
			for( j = 0; j < (currNeighbor -> num); j++ ) {
				scanf("%d", (currNeighbor -> ids) + j);
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
     * Time to do the math.
     * Compute the AMR Dissipation to convergence
     */
    
    
    /*
     * Free the memory of all boxes
     */
    for( i = 0; i < numGridBoxes; i++ ) {
        free(boxes[i].n_top.ids);
        free(boxes[i].n_bot.ids);
        free(boxes[i].n_left.ids);
        free(boxes[i].n_right.ids);
    }
    free(boxes);
    
	return 0;
}
