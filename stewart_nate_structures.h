/*
 * Name: Nate Stewart
 * Date: 02-07-16
 * Description: Storage container for all structures used in the computation 
 */

/*
 * Struct to store upper left and bottom right coordinates of the box.
 * Also stores the height and width for calculations.
 */
typedef struct {
	int ul_x, ul_y, br_x, br_y;
	int height, width;
} dimensions;

/*
 * Store the number of boxes that are neighbors and the ids of those boxes for use when constructing contact matrix.
 */
typedef struct {
	int num; // Number of boxes that are neighbors
	int *ids;
} neighbors;

/*
 * Simplistic storage container for all of the information regarding a box which is read in from the data files.
 */
typedef struct {
	int id;
	dimensions dims;
	neighbors nei[4]; // 0 = top, 1 = bot, 2 = left, 3 = right
	double temp;
} simpleBox;

/*
 * Storage container for the dissipation calculation. Only need to store temps, perimeter and contact distance.
 * This is constructed after all data is read from the data file.
 */ 
typedef struct {
	double temp;
	int perimeter;
	int edgeContact;
	int numNeighbors;
	double **neiTemps; // Neighbor temps
	int *neiCD; // Neighbor contact distances
} gridBox;

/*
 * Storage container to be passed into the pthreads.
 * Stores pointers to the gridBox array, the newTemps array, and the affectRate.
 */
typedef struct {
	int numGridBoxes;
	double *newTemps;
	float affectRate;
	gridBox *grid;
} threadStorage;

