/*
 * Name: Nate Stewart
 * Date: 02-08-16
 * Description: Hold constants and function prototypes for the dissipation computation
 */

#include "stewart_nate_structures.h"

#define DEFAULT_EPSILON 0.1
#define DEFAULT_AFFECT_RATE 0.1

/*
 * Calculate the contact distance between two boxes.
 * Box 1 is the reference box. Box 2 is its neighbor.
 * UL stands for upper left coordinate. BR stands for bottom right coordinate.
 */
int calculateContactDistance(int box1_ul, int box1_br, int box2_ul, int box2_br);

/*
 * Grab the input from stdin and load into the boxes strucutre. 
 * numGridBoxes: how many boxes are in 'boxes' parameter
 */
int getInput(simpleBox *boxes, int numGridBoxes);

/*
 * Transfer the raw data from the boxes structure into the calculated grid structure.
 * The benefit of this intermediate step is it reduces the calucations needed during the main dissipation convergence loop.
 * This is accomplished by calculating the contact distance and providing a temperature pointer. 
 * This bypasses the need of ids and locations of the different boxes
 */
void transferToGrid(gridBox *grid, simpleBox *boxes, int numGridBoxes);

/*
 * Traverse the temps array and load the max and min temperatures into maxTemp and minTemp. 
 * Requirement: numTemps > 0
 */
void getMinMax(double *temps, int numTemps, double *maxTemp, double *minTemp);

/*
 * Function to compute and return the value of the provided gridBox object 
 */
double computeDSV(gridBox *box, float affectRate);

/*
 * Entry point for the pthreads (lab2)
 */
void *threadEntry(void *storage);

