/*
 * Name: Nate Stewart
 * Date: 02-08-16
 * Description: Hold constants and function prototypes for the dissipation computation
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

