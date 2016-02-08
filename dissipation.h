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
 * Struct to store upper left and bottom right coordinates of the box.
 * Also stores the height and width for calculations
 */
typedef struct {
	int ul_x, ul_y, br_x, br_y;
	int height, width;
} coordinates;

/*
 * Store the number of boxes that are neighbors and the ids of those boxes
 */
typedef struct {
	int num; // Number of boxes that are neighbors
	int *ids;
} neighbors;

/*
 * Simplistic storage container for all of the information regarding a box
 */
typedef struct {
	int id;
	coordinates coords;
	neighbors n_top, n_bot, n_left, n_right;
	double temp;

} box;

