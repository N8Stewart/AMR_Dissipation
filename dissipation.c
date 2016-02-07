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
	int num_grid_boxes, num_rows, num_cols;
	scanf("%d %d %d", &num_grid_boxes, &num_rows, &num_cols);
	box *boxes = malloc(sizeof(box) * num_grid_boxes);

	/*
	 * Grab each data line 1 by 1
	 */
	int i, j;
	for( i = 0; i < num_grid_boxes; i++ ) {
		// Line 2: id
		scanf("%d", &boxes[i].id);
		// Line 3: coordinates
		coordinates *coords = &boxes[i].coords;
		scanf("%d %d %d %d", &(coords -> ul_y), &(coords -> ul_x), &(coords -> height), &(coords -> width));
		coords -> br_x = coords -> ul_x + coords -> width;
		coords -> br_y = coords -> ul_y + coords -> height;
		// Line 4: top neighbors
		neighbors *currNeighbor = &boxes[i].n_top;
		scanf("%d", &(currNeighbor -> num));
		if( (currNeighbor -> num) > 0 ) {
			(currNeighbor -> ids) = malloc(sizeof(int) * (currNeighbor -> num));
			for( j = 0; j < (currNeighbor -> num); j++ ) {
				scanf("%d", (currNeighbor -> ids) + j);
			}
			for( j = 0; j < (currNeighbor -> num); j++ ) {
				printf("%d\n", *((currNeighbor -> ids) + j));
			}
			
		}

	}
	

	/*
	 * Ensure the last line is '-1'. If so, the data has been read in correctly (most likely)
	 */


	return 0;
}
