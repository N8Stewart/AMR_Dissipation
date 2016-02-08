/*
 * Name: Nate Stewart
 * Date: 02-09-16
 * Description: Storage container for all relevant constants, structs, etc.
 */

/*
 * Constants for the epsilon and affect rate values in case a value is not supplied in the command line
 */
#define DEFAULT_EPSILON 0.1
#define DEFAULT_AFFECT_RATE 0.1

/*
 * Calculate the contact distance between two boxes.
 * Box 1 is the reference box. Box 2 is its neighbor.
 * UL stands for upper left coordinate. BR stands for bottom right coordinate.
 */
int calculateContactDistance(int box1_ul, int box1_br, int box2_ul, int box2_br);

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
	double *neiTemps; // Neighbor temps
	int *neiCD; // Neighbor contact distances
} gridBox;
