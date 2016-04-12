#ifndef RANDOM16807_H
#define RANDOM16807_H

/**
 * @brief 16807 psudo-random number generator
 * @param init
 *  + init=0, continue iteration using the previous seed
 *  + init<0, reset the seed using current time
 *  + init>0, reset the seed using init
 * @return psudo-random number
 */
double myrand(int init=0);

#endif  //RANDOM16807_H
